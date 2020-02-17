/*
 * Copyright (c) 2015 - 2020 Jolla Ltd.
 * Copyright (c) 2020 Open Mobile Platform LLC.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
 */

#include "provider_power_udev.h"

#include <QtDebug>

// ssu-sysinfo
#include <ssusysinfo.h>

namespace {

bool logging_enabled = false;
#define log_info(msg) if (logging_enabled) qInfo() << msg
#define log_debug(msg) if (logging_enabled) qDebug() << msg
#define log_warning(msg) if (logging_enabled) qWarning() << msg

long battery_mAh()
{
    ssusysinfo_t *sysinfo = ssusysinfo_create();
    if (sysinfo) {
        const QByteArray deviceModel = QByteArray(ssusysinfo_device_model(sysinfo)).toUpper();

        // Publicly supported models listed on jolla.com/zendesk:

        if (deviceModel == "I3113" || deviceModel == "I3123"            // Sony Xperia 10 - Single SIM
                || deviceModel == "I4113" || deviceModel == "I4193") {  // Sony Xperia 10 - Dual SIM
            return 2870;
        }
        if (deviceModel == "I3213" || deviceModel == "I3223"            // Sony Xperia 10 Plus - Single SIM
                || deviceModel == "I4213" || deviceModel == "I4293") {  // Sony Xperia 10 Plus - Dual SIM
            return 3000;
        }

        if (deviceModel == "H3113" || deviceModel == "H3123" || deviceModel == "H3133"  // Sony Xperia XA2 - Single SIM
                || deviceModel == "H4113" || deviceModel == "H4133") {                  // Sony Xperia XA2 - Dual SIM
            return 3300;
        }
        if (deviceModel == "H3213" || deviceModel == "H3223"            // Sony Xperia XA2 Ultra - Single SIM
                || deviceModel == "H4213" || deviceModel == "H4233"     // Sony Xperia XA2 Ultra - Dual SIM
                || deviceModel == "H3413"                               // Sony Xperia XA2 Plus - Single SIM
                || deviceModel == "H4413" || deviceModel == "H4493") {  // Sony Xperia XA2 Plus - Dual SIM
            return 3580;
        }

        if (deviceModel == "F5121"              // Sony Xperia X - Single SIM
                || deviceModel == "F5122") {    // Sony Xperia X - Dual SIM
            return 2620;
        }

        if (deviceModel == "X27") {     // Gemini PDA
            return 4220;
        }
    }

    // Jolla 1 default value
    return 2200;
}

// mAh * mV
const long energy_full_default = battery_mAh() * 3800;

// mA * mV / (sec per hour)
const long denergy_max_default = energy_full_default / 3600;

// uV
const long reasonable_battery_voltage_min = 1200000; // NiCd/NiMH
const long reasonable_battery_voltage_max = 48000000; // 48V
const long battery_charging_voltage_default = 4200000; // std Li-ion
// based on marketing Vnominal=3.7V (instead of real 3.6V) for
// batteries with Vcharging=4.2V
const double v_charging_to_nominal = 4200000 / (3700000 / 1000);

long calculate_nominal_voltage_liion(double charging_voltage)
{
    return long(charging_voltage * 1000 / v_charging_to_nominal);
}

}

BatteryInfo::BatteryInfo(BatteryContextPropertyProvider *provider)
    : m_provider(provider)
    , last_energy_change_time(0)
    , energy_now(energy_full_default)
    , time_to_full(0)
    , time_to_low(3600)
    , power(denergy_max_default)
    , nominal_voltage_(3800000)
    , energy_full_(energy_full_default)
    , denergy_max_(denergy_max_default)
    , denergy_(6, 10)
    , calculate_energy_(&BatteryInfo::calculate_energy_current)
    , get_energy_now_(&BatteryInfo::get_energy_now_from_capacity_)
    , current_sign_(1)
{
}

void BatteryInfo::setup()
{
    auto try_use_charge = [this]() {
        log_info("Trying to use charge info to get energy");
        auto has_charge = has_filesystem_attr("charge_full") && has_filesystem_attr("charge_now");
        if (!has_charge)
            return false;

        auto charge_full = attr_long_direct("charge_full");
        if (charge_full <= 0) {
            log_warning("Incorrect charge_full" << charge_full);
            return false;
        }

        auto tech_name = attr_bytes_direct("technology");
        if (tech_name != "Li-ion")
            log_warning("Technology" << tech_name
                       << "is not supported yet, assuming Li-ion");

        get_energy_now_ = &BatteryInfo::get_energy_now_from_charge_;
        long charging_voltage = attr_long_direct("voltage_ocv");
        if  (charging_voltage) {
            if (charging_voltage >= reasonable_battery_voltage_max
                || charging_voltage <= reasonable_battery_voltage_min) {
                log_warning("Suspicious OCV voltage, do not trust it");
                charging_voltage = battery_charging_voltage_default;
            }
        } else {
            log_warning("No info about charging voltage, using default");
            charging_voltage = battery_charging_voltage_default;
        }
        // nominal voltage is calculated based on empiric constant. In
        // real life it should be taken from the battery spec
        nominal_voltage_ = calculate_nominal_voltage_liion(charging_voltage);

        energy_full_ = (charge_full / 1000) * (nominal_voltage_ / 1000);
        return true;
    };

    bool has_iv = has_filesystem_attr("current_now") && has_filesystem_attr("voltage_now");
    bool has_energy = has_filesystem_attr("energy_now") && has_filesystem_attr("energy_full");

    if (has_iv) {
        log_info("I and V are available, using to calculate P");
        calculate_energy_ = &BatteryInfo::calculate_energy_current;
        if (attr_bytes_direct("model_name") == "INTN0001") {
            // this driver uses -I when discharging
            current_sign_ = -1;
        }
    } else {
        log_info("No I and/or V, calculating consumed power from dE");
        calculate_energy_ = &BatteryInfo::calculate_energy_power_now;
    }

    // choose energy info source
    if (has_energy) {
        log_info("Using energy_full(_now)");

        energy_full_ = m_provider->energyFull();
        if (energy_full_ > 0) {
            get_energy_now_ = &BatteryInfo::get_energy_now_directly_;
        } else {
            log_warning("Read wrong energy_full value" << energy_full_);
            has_energy = false;
        }
    }
    if (!(has_energy || try_use_charge())) {
        log_warning("There is no energy/charge info available from kernel"
                   << ", live with fake info based on charge percentage");
        if (has_filesystem_attr("capacity")) {
            energy_full_ = energy_full_default;
            get_energy_now_ = &BatteryInfo::get_energy_now_from_capacity_;
        } else {
            log_warning("Even capacity values are not supplied! Fix power_supply driver!");
            // TODO throw exeception?
            return;
        }
    }
    log_info("Battery full energy is set to " << energy_full_);
    energy_now.set((this->*get_energy_now_)());
    last_energy_change_time.set(::time(nullptr));
}

void BatteryInfo::set_energy(long v)
{
    energy_now.set(v);
    if (energy_now.changed())
        last_energy_change_time.set(::time(nullptr));
}

void BatteryInfo::set_denergy_now(long de)
{
    auto enow = m_provider->energy();
    log_debug("dE=" << de);
    if (!de)
        return;
    denergy_.push(de);
    if (de < 0) {
        denergy_max_ = - de;
        de = denergy_.average();
        // be pessimistic
        if (de < 0 && -de > denergy_max_)
            denergy_max_ = - de;
        log_debug("dEavg=" << de);
        // hour - 3600s
        auto et = de < 0 ? - enow / de * 36 / 10 : 0;
        time_to_low.set(et);
        time_to_full.set(0);
    } else {
        de = denergy_.average();
        // hour - 3600s
        auto et = de > 0 ? (energy_full_ - enow) / de * 36 / 10 : 0;
        time_to_low.set(0);
        time_to_full.set(et);
    }
    power.set(-de * 1000); // mW -> uW
}

void BatteryInfo::calculate_energy_power_now()
{
    auto dt = last_energy_change_time.last() - last_energy_change_time.previous();
    auto de = m_provider->energy() - energy_now.previous();
    if (dt && de)
        set_denergy_now(de / dt);
}

void BatteryInfo::calculate_energy_current()
{
    auto i = m_provider->current(), v = m_provider->voltage();
    auto p = (i / -1000) * (v / 1000) / 1000;
    log_debug("Calc power I*V (" << i << "*" << v << ")=" << p);
    set_denergy_now(p);
}

void BatteryInfo::calculate()
{
    set_energy((this->*get_energy_now_)());
    (this->*calculate_energy_)();
}

long BatteryInfo::get_energy_now_directly_()
{
    return m_provider->energy();
}

long BatteryInfo::get_energy_now_from_charge_()
{
    auto c = m_provider->charge();
    return (c > 0
            ? ((c / 1000) * (nominal_voltage_ / 1000))
            : m_provider->energy());
}

long BatteryInfo::get_energy_now_from_capacity_()
{
    auto c = m_provider->capacity();
    auto capacity_is_valid = c >= 0 && c <= 100;
    return (capacity_is_valid
            ? energy_full_ * c / 100
            : m_provider->energy());
}

bool BatteryInfo::has_filesystem_attr(const QString &attrName)
{
    return !(m_provider->filePathForBatteryData(attrName).isEmpty());
}

long BatteryInfo::attr_long_direct(const QString &attrName)
{
    return has_filesystem_attr(attrName)
            ? m_provider->readLong(m_provider->filePathForBatteryData(attrName))
            : -1;
}

QByteArray BatteryInfo::attr_bytes_direct(const QString &attrName)
{
    return has_filesystem_attr(attrName)
            ? m_provider->readLine(m_provider->filePathForBatteryData(attrName)).trimmed()
            : QByteArray();
}
