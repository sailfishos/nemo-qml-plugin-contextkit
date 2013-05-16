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

#include <QtQml/QQmlExtensionPlugin>
#include <qqml.h>

namespace ContextKitQml
{

#define MY_PLUGIN_NS org.freedesktop.contextkit

class Plugin : public QQmlExtensionPlugin
{
    Q_PLUGIN_METADATA(IID #MY_PLUGIN_NS)
public:
    void registerTypes(char const *uri)
    {
        qmlRegisterType<Property>(uri, 1, 0, "ContextProperty");
    }
};

}
