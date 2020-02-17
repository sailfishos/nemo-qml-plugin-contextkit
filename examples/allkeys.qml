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

import QtQuick 2.6
import Sailfish.Silica 1.0
import org.freedesktop.contextkit 1.0

ApplicationWindow {
    id: root

    initialPage: demoComponent
    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All

    readonly property var keys: {
        "Alarm": [
            "Present",
            "Triggers",
        ],

        "Battery": [
            "Capacity",
            "ChargePercentage",
            "ChargerType",
            "ChargingState",
            "Current",
            "Energy",
            "EnergyFull",
            "IsCharging",
            "Level",
            "LowBattery",
            "OnBattery",
            "Power",
            "State",
            "Temperature",
            "TimeUntilFull",
            "TimeUntilLow",
            "Voltage",
        ],

        "Cellular": [
            "CachedCardIdentifier",
            "CachedSubscriberIdentity",
            "CallCount",
            "CapabilityData",
            "CapabilityVoice",
            "CellName",
            "CurrentMCC",
            "CurrentMNC",
            "DataRoamingAllowed",
            "DataTechnology",
            "ExtendedNetworkName",
            "GPRSAttached",
            "HomeMCC",
            "HomeMNC",
            "MMSContext",
            "ModemPath",
            "NetworkName",
            "RegistrationStatus",
            "ServiceProviderName",
            "SignalBars",
            "SignalStrength",
            "Sim",
            "Status",
            "StkIdleModeText",
            "SubscriberIdentity",
            "Technology",
        ],

        "Internet": [
            "NetworkName",
            "NetworkState",
            "NetworkType",
            "SignalStrength",
            "Tethering",
        ],

        "Profile": [
            "Name",
        ],

        "Screen": [
            "Blanked",
        ],

        "Sensors": [
            "Orientation",
        ],

        "System": [
            "PowerSaveMode",
            "OfflineMode",
            "WlanEnabled",
            "InternetEnabled",
            "KeyboardPresent",
            "KeyboardOpen",
        ]
    }

    ListModel {
        id: listModel

        Component.onCompleted: {
            for (var k in root.keys) {
                append({"namespace": k})
                if (k === "Cellular") {
                    append({"namespace": "Cellular_1"})
                }
            }
        }
    }

    Component {
        id: demoComponent

        Page {
            ListView {
                anchors.fill: parent
                header: PageHeader { title: "Context properties" }
                model: listModel

                delegate: ListItem {
                    Label {
                        anchors.centerIn: parent
                        text: model.namespace
                    }

                    onClicked: {
                        var keys = model.namespace === "Cellular_1"
                                ? root.keys["Cellular"]
                                : root.keys[model.namespace]
                        pageStack.animatorPush(keysComponent, {
                                                   "namespace": model.namespace,
                                                   "keyList": keys
                                               })
                    }
                }
            }
        }
    }

    Component {
        id: keysComponent

        Page {
            property string namespace
            property var keyList

            ListView {
                anchors.fill: parent
                header: PageHeader { title: namespace }
                model: keyList

                delegate: DetailItem {
                    label: modelData
                    value: contextProperty.value === undefined
                           ? "undefined"
                           : contextProperty.value.toString()

                    ContextProperty {
                        id: contextProperty
                        key: namespace + "." + modelData
                        value: ""
                    }
                }
            }
        }
    }
}

