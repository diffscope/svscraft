import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.impl

import SVSCraft.UIComponents

Pane {
    id: container
    implicitWidth: 400
    implicitHeight: 400
    function set() {
        paneHeaderArea.set()
        paneArea.set()
    }
    property DockingPane pane: null
    onPaneChanged: set()
    Item {
        id: header
        anchors.top: parent.top
        width: parent.width
        height: 40
        RowLayout {
            spacing: 4
            anchors.fill: parent
            anchors.margins: 4
            IconLabel {
                spacing: 4
                leftPadding: 4
                rightPadding: 4
                icon.source: container.pane?.iconSource ?? ""
                icon.color: Theme.foregroundPrimaryColor
                icon.width: 16
                icon.height: 16
                text: container.pane?.title ?? ""
                color: Theme.foregroundPrimaryColor
                Layout.alignment: Qt.AlignVCenter
            }
            Rectangle {
                width: 1
                Layout.fillHeight: true
                Layout.topMargin: 4
                Layout.bottomMargin: 4
                color: Theme.borderColor
            }
            Item {
                id: paneHeaderArea
                Layout.fillWidth: true
                Layout.fillHeight: true
                property Item paneHeader: null
                function set() {
                    if (paneHeader)
                        paneHeader.parent = null
                    paneHeader = container.pane?.header ?? null
                    if (paneHeader) {
                        paneHeader.parent = paneHeaderArea
                        paneHeader.width = width
                        paneHeader.height = height
                    }
                }
                Connections {
                    target: container.pane
                    function onHeaderChanged() {
                        paneHeaderArea.set()
                    }
                }
                onWidthChanged: () => {
                    if (paneHeader)
                        paneHeader.width = width
                }
                onHeightChanged: () => {
                    if (paneHeader)
                        paneHeader.height = height
                }
            }
            Rectangle {
                width: 1
                Layout.fillHeight: true
                Layout.topMargin: 4
                Layout.bottomMargin: 4
                color: Theme.borderColor
                visible: !container.pane?.locked || (container.pane?.menu ?? false)
            }
            ToolButton {
                icon.source: container.pane?.dock ?
                    "qrc:/qt/qml/SVSCraft/UIComponents/assets/PanelSeparateWindow20Filled.svg" :
                    `qrc:/qt/qml/SVSCraft/UIComponents/assets/PanelLeft16Filled.svg`
                visible: !container.pane?.locked
                DescriptiveText.activated: hovered
                DescriptiveText.toolTip: container.pane?.dock ? qsTr("Undock") : qsTr("Dock")
                Layout.alignment: Qt.AlignVCenter
                onClicked: () => {
                    if (!container.pane)
                        return
                    container.pane.dock = !container.pane.dock
                }
            }
            ToolButton {
                icon.source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/MoreHorizontal16Filled.svg"
                Layout.alignment: Qt.AlignVCenter
                visible: container.pane?.menu ?? false
                onClicked: container.pane.menu.popup()
            }
        }
    }
    Item {
        id: paneArea
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        property Item pane: null
        function set() {
            if (pane)
                pane.parent = null
            pane = container.pane
            if (pane) {
                pane.parent = paneArea
                pane.width = width
                pane.height = height
            }
        }
        onWidthChanged: () => {
            if (pane)
                pane.width = width
        }
        onHeightChanged: () => {
            if (pane)
                pane.height = height
        }
    }
    Rectangle {
        anchors.top: header.bottom
        width: header.width
        height: 1
        color: Theme.splitterColor
    }
}