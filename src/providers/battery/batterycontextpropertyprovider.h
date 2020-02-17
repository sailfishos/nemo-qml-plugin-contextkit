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
class BatteryInfo;

class BatteryContextPropertyProvider : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)

    Q_PROPERTY(long capacity READ capacity NOTIFY capacityChanged)
    Q_PROPERTY(long charge READ charge NOTIFY chargeChanged)
    Q_PROPERTY(long current READ current NOTIFY currentChanged)
    Q_PROPERTY(long energy READ energy NOTIFY energyChanged)
    Q_PROPERTY(long energyFull READ energyFull NOTIFY energyFullChanged)
    Q_PROPERTY(long power READ power NOTIFY powerChanged)
    Q_PROPERTY(long temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(long timeUntilFull READ timeUntilFull NOTIFY timeUntilFullChanged)
    Q_PROPERTY(long timeUntilLow READ timeUntilLow NOTIFY timeUntilLowChanged)
    Q_PROPERTY(long voltage READ voltage NOTIFY voltageChanged)

public:
    BatteryContextPropertyProvider(QObject *parent = nullptr);
    ~BatteryContextPropertyProvider();

    int interval() const;
    void setInterval(int interval);

    bool active() const;
    void setActive(bool active);

    long capacity() const;
    long charge() const;
    long current() const;
    long energy() const;
    long energyFull() const;
    long power() const;
    long temperature() const;
    long timeUntilFull() const;
    long timeUntilLow() const;
    long voltage() const;

Q_SIGNALS:
    void intervalChanged();
    void activeChanged();

    void capacityChanged();
    void chargeChanged();
    void currentChanged();
    void energyChanged();
    void energyFullChanged();
    void powerChanged();
    void temperatureChanged();
    void timeUntilFullChanged();
    void timeUntilLowChanged();
    void voltageChanged();

private:
    enum BatteryDataType {
        Capacity,
        Charge,
        Current,
        Energy,
        EnergyFull,
        Temperature,
        Voltage,
    };

    struct BatteryData
    {
        QString filePath;
        long value = 0;
        void (BatteryContextPropertyProvider::*notifySignal)();
    };

    void timeout();
    void updateData(bool isInitialUpdate = false);
    void addDataType(BatteryDataType type,
                     const QString &filePath,
                     void (BatteryContextPropertyProvider::*notifySignal)());

    QString filePathForBatteryData(const QString &name) const;
    long readLong(const QString &fileName, bool *ok = nullptr);
    QByteArray readLine(const QString &fileName, bool *ok = nullptr);

    static QString batteryDirPath();

    friend class BatteryInfo;

    BatteryInfo *m_batteryInfo = nullptr;
    QString m_batteryDir;
    QTimer *m_poll = nullptr;
    QMap<BatteryDataType, BatteryData> m_data;
};

#endif
