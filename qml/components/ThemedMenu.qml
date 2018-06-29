import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.4

Menu {
    id: menu
    background: Rectangle {
       implicitWidth: 200
       implicitHeight: 40
       color: themeManager.properties.get("MenuItem", "background-color", menu.palette.window)
       border.color: themeManager.properties.get("MenuItem", "background-color", menu.palette.dark)
    }
    delegate: MenuItem {
        id: control
        hoverEnabled: true

        contentItem: IconLabel {
            text: control.text
            font: control.font
            icon: control.icon
            color: themeManager.properties.get("MenuItem", "color", control.palette.windowText, ((control.subMenu !== null && (control.highlighted || control.hovered)) ? "active" : control.hovered ? "hover" : ""))
        }

        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 40
            x: 1
            y: 1
            width: control.width - 2
            height: control.height - 2
            color: themeManager.properties.get("MenuItem", "background-color", control.palette.midlight, ((control.subMenu !== null && (control.highlighted || control.hovered)) ? "active" : control.hovered ? "hover" : ""))
        }
    }
}
