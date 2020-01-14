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

#include <QFile>
#include <QDir>
#include <QTimer>
#include <QDebug>
#include <QQmlInfo>

BatteryContextPropertyProvider::BatteryContextPropertyProvider(QObject *parent)
    : QObject(parent)
    , m_poll(new QTimer(this))
{
    m_poll->setInterval(1000);

    const QString batteryDir = batteryDirPath();
    if (batteryDir.isEmpty()) {
        qmlInfo(this) << "Battery system info cannot be supplied, cannot find data directory";
        return;
    }

    addDataType(Capacity, batteryDir + "/capacity", &BatteryContextPropertyProvider::capacityChanged);
    addDataType(Current, batteryDir + "/current_now", &BatteryContextPropertyProvider::currentChanged);
    addDataType(Energy, batteryDir + "/energy_now", &BatteryContextPropertyProvider::energyChanged);
    addDataType(EnergyFull, batteryDir + "/energy_full", &BatteryContextPropertyProvider::energyFullChanged);
    addDataType(Temperature, batteryDir + "/temp", &BatteryContextPropertyProvider::temperatureChanged);
    addDataType(Voltage, batteryDir + "/voltage_now", &BatteryContextPropertyProvider::voltageChanged);

    if (!m_data.isEmpty()) {
        timeout();  // get initial value
        connect(m_poll, &QTimer::timeout, this, &BatteryContextPropertyProvider::timeout);
        m_poll->start();
    }
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
    if (active) {
        m_poll->start();
    } else {
        m_poll->stop();
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
    }
}

long BatteryContextPropertyProvider::capacity() const
{
    return m_data.value(Capacity).value;
}

long BatteryContextPropertyProvider::current() const
{
    return m_data.value(Current).value;
}

long BatteryContextPropertyProvider::energy() const
{
    return m_data.value(Energy).value;
}

long BatteryContextPropertyProvider::energyFull() const
{
    return m_data.value(EnergyFull).value;
}

long BatteryContextPropertyProvider::temperature() const
{
    return m_data.value(Temperature).value;
}

long BatteryContextPropertyProvider::voltage() const
{
    return m_data.value(Voltage).value;
}

long BatteryContextPropertyProvider::readLong(const QString &fileName, bool *ok)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qmlInfo(this) << "Cannot open" << fileName << file.errorString();
        if (ok) {
            *ok = false;
        }
        return 0.0;
    }
    return file.readLine().trimmed().toLong(ok);
}

void BatteryContextPropertyProvider::timeout()
{
    for (auto it = m_data.begin(); it != m_data.end(); ++it) {
        BatteryData &batteryData = it.value();
        bool ok = false;
        long value = readLong(batteryData.filePath, &ok);
        if (ok) {
            batteryData.value = value;
            emit (this->*batteryData.notifySignal)();
        } else {
            qmlInfo(this) << "Unable to read data from:" << batteryData.filePath;
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
