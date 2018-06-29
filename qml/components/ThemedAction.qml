import QtQuick.Controls 2.4

Action {
    property var name
    property var displayName

    text: displayName
    onTriggered: {
        themeManager.theme = name
    }
}
