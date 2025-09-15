import QtQml
import QtQuick

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Item {
    id: thumbnail
    implicitWidth: 160
    implicitHeight: 120
    LayoutMirroring.enabled: false
    LayoutMirroring.childrenInherit: true
    clip: true

    property color accentColor: Theme.accentColor
    property color buttonColor: Theme.buttonColor
    property color textFieldColor: Theme.textFieldColor
    property color borderColor: Theme.borderColor
    property color backgroundPrimaryColor: Theme.backgroundPrimaryColor
    property color backgroundSecondaryColor: Theme.backgroundSecondaryColor
    property color backgroundTertiaryColor: Theme.backgroundTertiaryColor
    property color backgroundQuaternaryColor: Theme.backgroundQuaternaryColor
    property color foregroundPrimaryColor: Theme.foregroundPrimaryColor
    property color foregroundSecondaryColor: Theme.foregroundSecondaryColor

    Rectangle {
        id: backgroundGraph
        anchors.fill: parent
        color: thumbnail.backgroundPrimaryColor
    }

    Rectangle {
        id: windowGraph
        width: thumbnail.width
        height: thumbnail.height
        x: 20
        y: 20
        radius: 4
        color: thumbnail.backgroundSecondaryColor
        border.color: thumbnail.borderColor
    }

    Rectangle {
        id: cardGraph
        width: thumbnail.width - 36
        height: 30
        x: 28
        y: 28
        radius: 2
        color: thumbnail.backgroundTertiaryColor
        border.color: thumbnail.borderColor
    }

    Rectangle {
        id: cardImageGraph
        width: 22
        height: 22
        x: 32
        y: 32
        color: thumbnail.backgroundQuaternaryColor
        border.color: thumbnail.borderColor
    }

    Rectangle {
        id: cardPrimaryTextGraph
        width: thumbnail.width - 70
        height: 6
        x: 58
        y: 34
        radius: 3
        color: thumbnail.foregroundPrimaryColor
    }

    Rectangle {
        id: cardSecondaryTextGraph
        width: (thumbnail.width - 70) / 2
        height: 6
        x: 58
        y: 46
        radius: 3
        color: thumbnail.foregroundSecondaryColor
    }

    Rectangle {
        id: textFieldGraph
        width: thumbnail.width - 36
        height: 16
        x: 28
        y: 66
        radius: 2
        color: thumbnail.textFieldColor
        border.color: thumbnail.accentColor
    }

    Rectangle {
        id: textFieldTextGraph
        width: thumbnail.width - 70
        height: 6
        x: 32
        y: 71
        radius: 3
        color: thumbnail.foregroundPrimaryColor
    }

    Rectangle {
        id: buttonGraph
        width: 40
        height: 16
        x: thumbnail.width - 48
        y: thumbnail.height - 24
        radius: 2
        color: thumbnail.buttonColor
        border.color: thumbnail.borderColor
    }

    Rectangle {
        id: primaryButtonGraph
        width: 40
        height: 16
        x: thumbnail.width - 92
        y: thumbnail.height - 24
        radius: 2
        color: thumbnail.accentColor
        border.color: thumbnail.borderColor
    }

    Rectangle {
        id: border
        anchors.fill: parent
        color: "transparent"
        border.color: Theme.borderColor
    }

}