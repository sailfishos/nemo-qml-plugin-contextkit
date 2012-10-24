#ifndef _CK_QML_CK_PROPERTY_HPP_
#define _CK_QML_CK_PROPERTY_HPP_
/*
 * ContextKit property QML binding
 *
 * Copyright (C) 2012 Jolla Ltd.
 * Contact: Denis Zalevskiy <denis.zalevskiy@jollamobile.com>
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

#include <QObject>
#include <QVariant>
#include <QString>
#include <QScopedPointer>

class ContextProperty;

namespace ContextKitQml
{

class Property : public QObject
{
        Q_OBJECT;
        Q_PROPERTY(QString key READ key WRITE setKey);
        Q_PROPERTY(QVariant value READ value WRITE setDefaultValue
                   NOTIFY valueChanged);
        Q_PROPERTY(bool subscribed READ isSubscribed WRITE setSubscribed NOTIFY subscribedChanged)
        Q_CLASSINFO("DefaultProperty", "value");
public:
        Property(QObject *parent = 0);
        ~Property();

        QString key() const;
        void setKey(QString const &);

        QVariant value() const;
        void setDefaultValue(QVariant const &);

        bool isSubscribed() const;
        void setSubscribed(bool subscribed);
        Q_INVOKABLE void subscribe();
        Q_INVOKABLE void unsubscribe();

signals:
        void valueChanged();
        void subscribedChanged();

private:
        QScopedPointer<ContextProperty> prop;
        QVariant default_value;
        bool subscribed;
};

}

#endif // _CK_QML_CK_PROPERTY_HPP_
