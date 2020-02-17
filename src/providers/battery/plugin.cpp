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

#include <QtQml/qqml.h>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlExtensionPlugin>

class ContextKitPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.freedesktop.contextkit.providers.battery")

public:
    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(engine)
        Q_UNUSED(uri)
    }

    void registerTypes(char const* uri) {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("org.freedesktop.contextkit.providers.battery"));

        qmlRegisterType<BatteryContextPropertyProvider>(uri, 1, 0, "BatteryContextPropertyProvider");
    }
};

#include "plugin.moc"
