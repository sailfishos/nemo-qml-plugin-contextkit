/*
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

#include "batterycontextpropertyprovider.h"
#include "provider_power_udev.h"

#include <QFile>
#include <QDir>
#include <QTimer>
#include <QDebug>
#include <QQmlInfo>

BatteryContextPropertyProvider::BatteryContextPropertyProvider(QObject *parent)
    : QObject(parent)
    , m_batteryInfo(new BatteryInfo(this))
    , m_poll(new QTimer(this))
{
    m_poll->setInterval(1000);

    const QString batteryDir = batteryDirPath();
    if (batteryDir.isEmpty()) {
        qmlInfo(this) << "Battery system info cannot be supplied, cannot find data directory";
        return;
    }

    m_batteryDir = batteryDir;
    addDataType(Capacity, batteryDir + "/capacity", &BatteryContextPropertyProvider::capacityChanged);
    addDataType(Charge, batteryDir + "/charge_now", &BatteryContextPropertyProvider::chargeChanged);
    addDataType(Current, batteryDir + "/current_now", &BatteryContextPropertyProvider::currentChanged);
    addDataType(Energy, batteryDir + "/energy_now", &BatteryContextPropertyProvider::energyChanged);
    addDataType(EnergyFull, batteryDir + "/energy_full", &BatteryContextPropertyProvider::energyFullChanged);
    addDataType(Temperature, batteryDir + "/temp", &BatteryContextPropertyProvider::temperatureChanged);
    addDataType(Voltage, batteryDir + "/voltage_now", &BatteryContextPropertyProvider::voltageChanged);

    if (!m_data.isEmpty()) {
        // setup initial values
        updateData(true);
        m_batteryInfo->setup();

        connect(m_poll, &QTimer::timeout, this, &BatteryContextPropertyProvider::timeout);
        m_poll->start();
    }
}

BatteryContextPropertyProvider::~BatteryContextPropertyProvider()
{
    delete m_batteryInfo;
}

void BatteryContextPropertyProvider::addDataType(BatteryDataType type,
                                                 const QString &filePath,
                                                 void (BatteryContextPropertyProvider::*notifySignal)())
{
    if (!QFile::exists(filePath)) {
        return;
    }

    BatteryData data;
    data.filePath = filePath;
    data.value = 0.0;
    data.notifySignal = notifySignal;
    m_data.insert(type, data);
}

bool BatteryContextPropertyProvider::active() const
{
    return m_poll->isActive();
}

void BatteryContextPropertyProvider::setActive(bool active)
{
    if (active != m_poll->isActive()) {
        if (active) {
            m_poll->start();
        } else {
            m_poll->stop();
        }
        emit activeChanged();
    }
}

int BatteryContextPropertyProvider::interval() const
{
    return m_poll->interval();
}

void BatteryContextPropertyProvider::setInterval(int interval)
{
    if (m_poll->interval() != interval) {
        m_poll->setInterval(interval);
        emit intervalChanged();
    }
}

long BatteryContextPropertyProvider::capacity() const
{
    return m_data.value(Capacity).value;
}

long BatteryContextPropertyProvider::charge() const
{
    return m_data.value(Charge).value;
}

long BatteryContextPropertyProvider::current() const
{
    return m_data.value(Current).value;
}

long BatteryContextPropertyProvider::energy() const
{
    const auto it = m_data.find(Energy);
    return (it != m_data.end()) ? it->value : m_batteryInfo->energy_now.last();
}

long BatteryContextPropertyProvider::energyFull() const
{
    const auto it = m_data.find(EnergyFull);
    return (it != m_data.end()) ? it->value : m_batteryInfo->energy_full();
}

long BatteryContextPropertyProvider::power() const
{
    return m_batteryInfo->power.last();
}

long BatteryContextPropertyProvider::temperature() const
{
    return m_data.value(Temperature).value;
}

long BatteryContextPropertyProvider::timeUntilFull() const
{
    return m_batteryInfo->time_to_full.last();
}

long BatteryContextPropertyProvider::timeUntilLow() const
{
    return m_batteryInfo->time_to_low.last();
}

long BatteryContextPropertyProvider::voltage() const
{
    return m_data.value(Voltage).value;
}

long BatteryContextPropertyProvider::readLong(const QString &fileName, bool *ok)
{
    return readLine(fileName, ok).trimmed().toLong(ok);
}

QByteArray BatteryContextPropertyProvider::readLine(const QString &fileName, bool *ok)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qmlInfo(this) << "Cannot open" << fileName << file.errorString();
        if (ok) {
            *ok = false;
        }
        return QByteArray();
    }
    return file.readLine();
}

QString BatteryContextPropertyProvider::filePathForBatteryData(const QString &name) const
{
    const QString path = m_batteryDir + '/' + name;
    return QFile::exists(path) ? path : QString();
}

void BatteryContextPropertyProvider::timeout()
{
    updateData(false);
}

void BatteryContextPropertyProvider::updateData(bool isInitialUpdate)
{
    for (auto it = m_data.begin(); it != m_data.end(); ++it) {
        BatteryData &batteryData = it.value();
        bool ok = false;
        long value = readLong(batteryData.filePath, &ok);
        if (ok) {
            if (batteryData.value != value) {
                batteryData.value = value;
                emit (this->*batteryData.notifySignal)();
            }
        } else {
            qmlInfo(this) << "Unable to read data from:" << batteryData.filePath;
        }
    }

    if (!isInitialUpdate) {
        m_batteryInfo->calculate();

        const auto it = m_data.find(Energy);
        if (it == m_data.end()) {
            if (m_batteryInfo->energy_now.last() != m_batteryInfo->energy_now.previous()) {
                emit energyChanged();
            }
        }

        if (m_batteryInfo->power.last() != m_batteryInfo->power.previous()) {
            emit powerChanged();
        }
        if (m_batteryInfo->time_to_full.last() != m_batteryInfo->time_to_full.previous()) {
            emit timeUntilFullChanged();
        }
        if (m_batteryInfo->time_to_low.last() != m_batteryInfo->time_to_low.previous()) {
            emit timeUntilLowChanged();
        }
    }
}

QString BatteryContextPropertyProvider::batteryDirPath()
{
    QDir dir("/sys/class/power_supply");
    const QStringList dirs = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for (const QString &dirName : dirs) {
        const QString subDirPath = dir.absoluteFilePath(dirName);
        QFile file(subDirPath + "/type");
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            if (file.readLine().trimmed() == "Battery") {
                return subDirPath;
            }
        }
    }
    return QString();
}
