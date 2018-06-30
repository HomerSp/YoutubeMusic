import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.4

MenuBar {
    id: menuBar
    delegate: MenuBarItem {
        id: control
        hoverEnabled: true

        implicitWidth: Math.max(background ? background.implicitWidth : 0,
                                    contentItem.implicitWidth + leftPadding + rightPadding)
        implicitHeight: Math.max(background ? background.implicitHeight : 0,
                                 Math.max(contentItem.implicitHeight,
                                          indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)
        baselineOffset: contentItem.y + contentItem.baselineOffset

        contentItem: IconLabel {
            text: parent.text
            font: parent.font
            opacity: enabled ? 1.0 : 0.3
            color: themeManager.properties.get("MenuBarItem", "color", control.palette.buttonText, ((control.menu !== null && control.menu.visible) ? "active" : control.hovered ? "hover" : ""))
        }

        background: Rectangle {
            implicitWidth: 40
            implicitHeight: 40
            opacity: enabled ? 1 : 0.3
            color: themeManager.properties.get("MenuBarItem", "background-color", "transparent", ((control.menu !== null && control.menu.visible) ? "active" : control.hovered ? "hover" : ""))
        }
    }

    background: Rectangle {
       implicitHeight: 40
       color: themeManager.properties.get("MenuBar", "background-color", menuBar.palette.button)

       Rectangle {
           color: themeManager.properties.get("MenuBar", "border-bottom-color", menuBar.palette.button)
           width: parent.width
           height: 1
           anchors.bottom: parent.bottom
       }
   }
}
