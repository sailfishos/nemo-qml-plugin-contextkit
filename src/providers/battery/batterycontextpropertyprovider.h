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

#ifndef CONTEXTKIT_PROVIDER_BATTERY_H
#define CONTEXTKIT_PROVIDER_BATTERY_H

#include <QObject>
#include <QMap>

class QTimer;

class BatteryContextPropertyProvider : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)

    Q_PROPERTY(long capacity READ capacity NOTIFY capacityChanged)
    Q_PROPERTY(long current READ current NOTIFY currentChanged)
    Q_PROPERTY(long energy READ energy NOTIFY energyChanged)
    Q_PROPERTY(long energyFull READ energy NOTIFY energyFullChanged)
    Q_PROPERTY(long temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(long voltage READ voltage NOTIFY voltageChanged)

public:
    BatteryContextPropertyProvider(QObject *parent = nullptr);

    int interval() const;
    void setInterval(int interval);

    bool active() const;
    void setActive(bool active);

    long capacity() const;
    long current() const;
    long energy() const;
    long energyFull() const;
    long temperature() const;
    long voltage() const;

Q_SIGNALS:
    void intervalChanged();
    void activeChanged();

    void capacityChanged();
    void currentChanged();
    void energyChanged();
    void energyFullChanged();
    void temperatureChanged();
    void voltageChanged();

private:
    enum BatteryDataType {
        Capacity,
        Current,
        Energy,
        EnergyFull,
        Temperature,
        Voltage,
    };

    struct BatteryData
    {
        QString filePath;
        long value = 0.0;
        void (BatteryContextPropertyProvider::*notifySignal)();
    };

    long readLong(const QString &fileName, bool *ok);
    void timeout();
    void addDataType(BatteryDataType type,
                     const QString &filePath,
                     void (BatteryContextPropertyProvider::*notifySignal)());
    static QString batteryDirPath();

    QTimer *m_poll = nullptr;
    QMap<BatteryDataType, BatteryData> m_data;
};

#endif
