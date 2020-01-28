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

#ifndef BATTERY_PROVIDER_POWER_UDEV
#define BATTERY_PROVIDER_POWER_UDEV

#include "batterycontextpropertyprovider.h"

#include <thread>
#include <memory>
#include <array>
#include <functional>
#include <time.h>
#include <stdlib.h>

#include <QString>

template <typename T>
class ChangingValue
{
public:
    ChangingValue(T const & initial)
        : prev_(initial), now_(initial)
    {}

    ChangingValue(T && initial)
        : prev_(std::move(initial)), now_(prev_)
    {}

    ChangingValue(ChangingValue<T> && from)
        : prev_(std::move(from.prev_)), now_(std::move(from.now_))
    {}

    ChangingValue(ChangingValue<T> const &from)
        : prev_(from.prev_), now_(from.now_)
    {}

    virtual ~ChangingValue() {}

    ChangingValue& operator = (ChangingValue<T> const &from)
    {
        ChangingValue<T> tmp(from);
        std::swap(*this, tmp);
        return *this;
    }

    bool changed() const { return now_ != prev_; }

    T last() { return now_; }
    T const & last() const { return now_; }
    T previous() { return prev_; }
    T const & previous() const { return prev_; }

    void set(T const &v) { now_ = v; }
    void set(T &&v) { now_ = std::move(v); }

    template <typename FnT>
    void on_changed(FnT fn)
    {
        if (changed())
            fn(prev_, now_);
    }

    template <typename FnT>
    void on_changed(FnT fn) const
    {
        if (changed())
            fn(prev_, now_);
    }

    void update() { prev_ = now_; }

private:
    T prev_;
    T now_;
};

template <typename T>
struct LastN
{
    LastN(size_t max_size, T precision)
        : max_size_(max_size)
        , sum_(0)
        , precision_(precision)
    {
    }

    void clear()
    {
        values_.clear();
        sum_ = 0;
    }

    void push(T v)
    {
        values_.push_back(v);
        if (values_.size() > max_size_) {
            sum_ -= values_.front();
            values_.pop_front();
        }
        sum_ += v;
    }

    T average() const
    {
        T sz = values_.size();
        return sz ? (((sum_ * precision_) / sz) / precision_) : sum_;
    }
private:

    size_t max_size_;
    T sum_;
    T precision_;
    std::list<T> values_;
};

class BatteryInfo
{
public:
    BatteryInfo(BatteryContextPropertyProvider *provider);
    ~BatteryInfo() {}

    void setup();

    void set_denergy_now(long de);

    long denergy_max() const
    {
        return denergy_max_;
    }

    long energy_full() const
    {
        return energy_full_;
    }

    void calculate();

    BatteryContextPropertyProvider *m_provider = nullptr;
    ChangingValue<time_t> last_energy_change_time;
    ChangingValue<long> energy_now;
    ChangingValue<long> time_to_full;
    ChangingValue<long> time_to_low;
    ChangingValue<long> power;

private:
    void set_energy(long);

    void calculate_energy_power_now();
    void calculate_energy_current();

    long get_energy_now_directly_();
    long get_energy_now_from_charge_();
    long get_energy_now_from_capacity_();

    bool has_filesystem_attr(const QString &attrName);
    long attr_long_direct(const QString &attrName);
    QByteArray attr_bytes_direct(const QString &attrName);

    long nominal_voltage_;
    long energy_full_;
    long denergy_max_;
    LastN<long> denergy_;

    long mw_per_percent_;
    long sec_per_percent_max_;
    void (BatteryInfo::*calculate_energy_)(void);
    long (BatteryInfo::*get_energy_now_)(void);
    int current_sign_;
};

#endif
