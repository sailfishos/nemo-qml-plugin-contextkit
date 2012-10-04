import Qt 4.7
import QtQuick 1.1
import org.freedesktop.contextkit 1.0

Rectangle {
    color: "navy"
    width: 320
    height: 200

    ContextProperty {
        key: "Battery.ChargePercentage"
        id: percentage
        value: 0 // default
    }

    Text {
        anchors { centerIn: parent }
        text: "Battery level: " + percentage.value + "%"
    }
}
