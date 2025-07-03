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

    contentItem: Item {
        ColorImage {
            id: image
            anchors.centerIn: parent
            color: !button.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
                   Theme.foregroundPrimaryColor
            source: {
                switch (button.type) {
                    case SystemButton.Close:
                        return "qrc:/qt/qml/SVSCraft/UIComponents/assets/Dismiss16Regular.svg"
                    case SystemButton.MaximizeRestore:
                        return Window.visibility === Window.Windowed ? "qrc:/qt/qml/SVSCraft/UIComponents/assets/Square16Regular.svg" : "qrc:/qt/qml/SVSCraft/UIComponents/assets/SquareMultiple16Regular.svg"
                    case SystemButton.Maximize:
                        return "qrc:/qt/qml/SVSCraft/UIComponents/assets/Square16Regular.svg"
                    case SystemButton.Restore:
                        return "qrc:/qt/qml/SVSCraft/UIComponents/assets/SquareMultiple16Regular.svg"
                    case SystemButton.Minimize:
                        return "qrc:/qt/qml/SVSCraft/UIComponents/assets/Subtract16Regular.svg"
                    case SystemButton.Help:
                        return "qrc:/qt/qml/SVSCraft/UIComponents/assets/Question16Regular.svg"
                }
            }
        }
    }

    background: Rectangle {
        readonly property color hoveredColor: Qt.rgba(Theme.foregroundPrimaryColor.r, Theme.foregroundPrimaryColor.g, Theme.foregroundPrimaryColor.b, Theme.foregroundPrimaryColor.a * 0.25)
        readonly property color pressedColor: Qt.rgba(Theme.foregroundPrimaryColor.r, Theme.foregroundPrimaryColor.g, Theme.foregroundPrimaryColor.b, Theme.foregroundPrimaryColor.a * 0.15)
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