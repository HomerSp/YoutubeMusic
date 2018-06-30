import QtQuick 2.0
import QtQuick.Controls 2.2

MenuSeparator {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0, contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0, contentItem.implicitHeight + topPadding + bottomPadding)

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
