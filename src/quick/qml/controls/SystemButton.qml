import QtQml
import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl

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

    implicitWidth: 47
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
        IconLabel {
            id: image
            anchors.centerIn: parent
            icon.color: !button.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
                        button.hovered ? Theme.foregroundPrimaryColor :
                        !Window.active ? Theme.foregroundSecondaryColor :
                        Theme.foregroundPrimaryColor
            icon.source: {
                switch (button.type) {
                    case SystemButton.Close:
                        return "qrc:/qt/qml/SVSCraft/UIComponents/assets/Dismiss16Regular.svg"
                    case SystemButton.MaximizeRestore:
                        return Window.visibility === Window.Windowed ? "qrc:/qt/qml/SVSCraft/UIComponents/assets/Maximize16Regular.svg" : "qrc:/qt/qml/SVSCraft/UIComponents/assets/SquareMultiple16Regular.svg"
                    case SystemButton.Maximize:
                        return "qrc:/qt/qml/SVSCraft/UIComponents/assets/Maximize16Regular.svg"
                    case SystemButton.Restore:
                        return "qrc:/qt/qml/SVSCraft/UIComponents/assets/SquareMultiple16Regular.svg"
                    case SystemButton.Minimize:
                        return "qrc:/qt/qml/SVSCraft/UIComponents/assets/Subtract16Regular.svg"
                    case SystemButton.Help:
                        return "qrc:/qt/qml/SVSCraft/UIComponents/assets/Question16Regular.svg"
                }
            }
            icon.width: 16
            icon.height: 16
        }
    }

    background: Rectangle {
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