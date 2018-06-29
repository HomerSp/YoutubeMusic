import QtQuick 2.0
import QtQuick.Controls 2.2

MenuSeparator {
    id: control
    contentItem: Rectangle {
        implicitWidth: 188
        implicitHeight: 1
        color: themeManager.properties.get("MenuSeparator", "color", control.palette.mid)
    }
    background: Rectangle {
        implicitWidth: 188
        implicitHeight: 5
        color: themeManager.properties.get("MenuSeparator", "background-color", "transparent")
    }
}
