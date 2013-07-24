import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Silica.theme 1.0
import org.freedesktop.contextkit 1.0

Rectangle {
    color: "navy"
    width: 480
    height: 480

    Column {
        width: parent.width
    Repeater {
        model : ["Battery.ChargePercentage"
                 , "Battery.IsCharging"
                 , "Battery.OnBattery"
                 , "Battery.TimeUntilFull"
                 , "Battery.TimeUntilLow"
                 , "Bluetooth.Enabled"
                 , "Bluetooth.Connected"
                 , "Bluetooth.Visible"
                 , "System.OfflineMode"
                ]
            Row {
                spacing: 10
                ContextProperty {
                    key: modelData
                    id: src
                    value: ""
                    onValueChanged: console.log(key + "->" + value + "/" + typeof(value))
                }
                Label {
                    color: Theme.highlightColor
                    text: src.key
                }

                Label {
                    text: src.value
                }
            }
    }

    }

}
