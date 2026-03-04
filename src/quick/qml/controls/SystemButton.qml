import QtQml
import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl
import QtCore

import SVSCraft
import SVSCraft.UIComponents

T.Button {
    id: button
    enum Type {
        Close,
        MaximizeRestore,
        Maximize,
        Restore,
        Minimize,
        Help
    }
    required property int type
    property bool useWindowsStyle: false

    implicitWidth: button.useWindowsStyle && Qt.platform.os === "windows" ? 48 : 32
    implicitHeight: 28

    Accessible.name: {
        switch (button.type) {
            case SystemButton.Close:
                return qsTr("Close")
            case SystemButton.MaximizeRestore:
                return Window.visibility === Window.Windowed ? qsTr("Maximize") : qsTr("Restore")
            case SystemButton.Maximize:
                return qsTr("Maximize")
            case SystemButton.Restore:
                return qsTr("Restore")
            case SystemButton.Minimize:
                return qsTr("Minimize")
            case SystemButton.Help:
                return qsTr("Help")
        }
    }

    contentItem: Item {
        Text {
            id: windowsPlatformIcon
            anchors.centerIn: parent
            visible: button.useWindowsStyle && Qt.platform.os === "windows"
            color: !button.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
                   button.hovered ? Theme.foregroundPrimaryColor :
                   !Window.active ? Theme.foregroundSecondaryColor :
                   Theme.foregroundPrimaryColor
            text: {
                switch (button.type) {
                    case SystemButton.Close:
                        return "\ue8bb"
                    case SystemButton.MaximizeRestore:
                        return Window.visibility === Window.Windowed ? "\ue922" : "\ue923"
                    case SystemButton.Maximize:
                        return "\ue922"
                    case SystemButton.Restore:
                        return "\ue923"
                    case SystemButton.Minimize:
                        return "\ue921"
                    case SystemButton.Help:
                        return "\ue897"
                }
            }
            font.family: SystemInformation.productVersion === "11" ? "Segoe Fluent Icons" : "Segoe MDL2 Assets"
            font.pointSize: 7.2
        }
        IconImage {
            id: genericIcon
            anchors.centerIn: parent
            visible: !button.useWindowsStyle || Qt.platform.os !== "windows"
            color: !button.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
                   button.hovered ? Theme.foregroundPrimaryColor :
                   !Window.active ? Theme.foregroundSecondaryColor :
                   Theme.foregroundPrimaryColor
            sourceSize.width: 12
            sourceSize.height: 12
            source: {
                switch (button.type) {
                    case SystemButton.Close:
                        return "image://fluent-system-icons/dismiss?size=16&style=regular"
                    case SystemButton.MaximizeRestore:
                        return Window.visibility === Window.Windowed ? "image://fluent-system-icons/maximize?size=16&style=regular" : "image://fluent-system-icons/square_multiple?size=16&style=regular"
                    case SystemButton.Maximize:
                        return "image://fluent-system-icons/maximize?size=16&style=regular"
                    case SystemButton.Restore:
                        return "image://fluent-system-icons/square_multiple?size=16&style=regular"
                    case SystemButton.Minimize:
                        return "image://fluent-system-icons/subtract?size=16&style=regular"
                    case SystemButton.Help:
                        return "image://fluent-system-icons/question?size=16&style=regular"
                }
            }
        }
    }

    background: Item {
        Rectangle {
            anchors.centerIn: parent
            width: button.useWindowsStyle && Qt.platform.os === "windows" ? parent.width : 20
            height: button.useWindowsStyle && Qt.platform.os === "windows" ? parent.height : 20
            radius: button.useWindowsStyle && Qt.platform.os === "windows" ? 0 : 10
            readonly property color hoveredColor: Qt.rgba(Theme.foregroundPrimaryColor.r, Theme.foregroundPrimaryColor.g, Theme.foregroundPrimaryColor.b, Theme.foregroundPrimaryColor.a * 0.2)
            readonly property color pressedColor: Qt.rgba(Theme.foregroundPrimaryColor.r, Theme.foregroundPrimaryColor.g, Theme.foregroundPrimaryColor.b, Theme.foregroundPrimaryColor.a * 0.1)
            readonly property color closeHoveredColor: "#E81123"
            readonly property color closePressedColor: "#9E1E2A"
            color: {
                if (button.pressed) {
                    return button.type === SystemButton.Close ? closePressedColor : pressedColor
                }
                if (button.hovered) {
                    return button.type === SystemButton.Close ? closeHoveredColor : hoveredColor
                }
                return "transparent"
            }
        }
    }

    onClicked: () => {
        if (type === SystemButton.Close) {
            Window.window.close()
        } else if (type === SystemButton.MaximizeRestore) {
            if (window.visibility === Window.Windowed) {
                window.showMaximized()
            } else {
                window.showNormal()
            }
        } else if (type === SystemButton.Maximize) {
            window.showMaximized()
        } else if (type === SystemButton.Restore) {
            window.showNormal()
        } else if (type === SystemButton.Minimize) {
            window.showMinimized()
        }
    }

}