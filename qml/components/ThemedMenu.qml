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

        implicitWidth: Math.max(background ? background.implicitWidth : 0,
                                contentItem.implicitWidth + leftPadding + rightPadding)
        implicitHeight: Math.max(background ? background.implicitHeight : 0,
                                 Math.max(contentItem.implicitHeight,
                                          indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)
        baselineOffset: contentItem.y + contentItem.baselineOffset

        contentItem: IconLabel {
            text: control.text
            font: control.font
            icon: control.icon
            color: themeManager.properties.get("MenuItem", "color", control.palette.windowText, !control.enabled ? "disabled" : ((control.subMenu !== null && (control.highlighted || control.hovered)) ? "active" : control.hovered ? "hover" : ""))
        }

        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 40
            x: 1
            y: 1
            width: control.width - 2
            height: control.height - 2
            color: themeManager.properties.get("MenuItem", "background-color", control.palette.midlight, !control.enabled ? "disabled" : ((control.subMenu !== null && (control.highlighted || control.hovered)) ? "active" : control.hovered ? "hover" : ""))
        }

        arrow: ColorImage {
            x: control.mirrored ? control.leftPadding : control.width - width - control.rightPadding
            y: control.topPadding + (control.availableHeight - height) / 2

            visible: control.subMenu
            mirror: control.mirrored
            source: control.subMenu ? "qrc:/qt-project.org/imports/QtQuick/Controls.2/images/arrow-indicator.png" : ""
            color: control.palette.windowText
            defaultColor: "#353637"
        }
    }
}
