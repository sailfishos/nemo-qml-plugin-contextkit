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

#include "ck_property.hpp"
#include <contextproperty.h>

namespace ContextKitQml
{

Property::Property(QObject *parent) : QObject(parent)
{
}

Property::~Property()
{
}

QString Property::key() const
{
        return prop ? prop->key() : QString("");
}

void Property::setKey(QString const &key)
{
        prop.reset(new ContextProperty(key));
        connect(prop.data(), SIGNAL(valueChanged()),
                SIGNAL(valueChanged()));
}

void Property::setDefaultValue(QVariant const &value)
{
        default_value = value;
}

void Property::subscribe() const
{
        if (prop)
                prop->subscribe();
}

void Property::unsubscribe() const
{
        if (prop)
                prop->unsubscribe();
}

QVariant Property::value() const
{
        return prop ? prop->value(default_value) : default_value;
}

}
