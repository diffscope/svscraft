/******************************************************************************
 * Copyright (c) 2025 OpenVPI                                                 *
 *                                                                            *
 * This file is part of SVSCraft                                              *
 *                                                                            *
 * SVSCraft is free software: you can redistribute it and/or modify it under  *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License, or (at your     *
 * option) any later version.                                                 *
 *                                                                            *
 * SVSCraft is distributed in the hope that it will be useful, but WITHOUT    *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public        *
 * License for more details.                                                  *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with SVSCraft. If not, see <https://www.gnu.org/licenses/>.          *
 ******************************************************************************/

import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.impl

import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Pane {
    id: container
    Accessible.name: container.pane ? qsTr("Docking View Panel of %1").arg(container.pane.title) : ""
    implicitWidth: 400
    implicitHeight: 400
    function set() {
        paneHeaderArea.set()
        paneArea.set()
    }
    property DockingPane pane: null
    property bool undockToolButtonVisible: true
    property bool dockingWindow: false
    property bool active: false
    signal removed()
    signal activated()
    onPaneChanged: set()
    FocusScope {
        anchors.fill: parent
        onActiveFocusChanged: () => {
            if (activeFocus && container.pane)
                container.activated()
        }
        Rectangle {
            id: header
            Accessible.role: Accessible.ToolBar
            Accessible.name: container.pane?.title ?? ""
            Accessible.description: qsTr("Docking panel title bar")
            anchors.top: parent.top
            width: parent.width
            height: 32
            color: container.active ? Theme.dockingPanelHeaderActiveColorChange.apply(Theme.backgroundPrimaryColor) : Theme.backgroundPrimaryColor
            Behavior on color {
                ColorAnimation {
                    duration: Theme.colorAnimationDuration
                    easing.type: Easing.OutCubic
                }
            }
            RowLayout {
                spacing: 4
                anchors.fill: parent
                anchors.margins: 4
                IconLabel {
                    leftPadding: 4
                    icon.source: container.pane?.icon.source ?? ""
                    icon.color: Theme.foregroundPrimaryColor
                    icon.width: 16
                    icon.height: 16
                    font: container.font
                    Layout.alignment: Qt.AlignVCenter
                }
                Text {
                    color: Theme.foregroundPrimaryColor
                    rightPadding: 4
                    font: container.font
                    text: container.pane?.title ?? ""
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
                    icon.source: container.dockingWindow ?
                        "qrc:/qt/qml/SVSCraft/UIComponents/assets/Drag24Filled.svg" :
                        container.pane?.dock ?
                        "qrc:/qt/qml/SVSCraft/UIComponents/assets/PanelSeparateWindow20Filled.svg" :
                        "qrc:/qt/qml/SVSCraft/UIComponents/assets/PanelLeft16Filled.svg"
                    visible: container.undockToolButtonVisible && !container.pane?.locked
                    display: AbstractButton.IconOnly
                    text: container.dockingWindow ? qsTr("Drag to Dock") : container.pane?.dock ? qsTr("Undock") : qsTr("Dock")
                    DescriptiveText.statusTip: container.dockingWindow ? qsTr("Drag this button to a side bar to dock current panel") : container.pane?.dock ? qsTr("Show current panel in a popup window") : qsTr("Dock current panel back to the side bar")
                    Layout.alignment: Qt.AlignVCenter
                    onClicked: () => {
                        if (!container.pane || container.dockingWindow)
                            return
                        container.pane.dock = !container.pane.dock
                    }
                    down: mouseArea.pressed
                    Item {
                        id: dragTarget
                        readonly property QtObject modelData: container.pane
                        readonly property QtObject container: null
                        readonly property bool current: true
                        anchors.fill: parent
                        Drag.keys: ["SVSCraft.UIComponent.DockingView"]
                        Drag.hotSpot.x: width / 2
                        Drag.hotSpot.y: height / 2
                        Drag.mimeData: {
                            "SVSCraft.UIComponent.DockingView": ""
                        }
                        Drag.dragType: Drag.Automatic
                        Drag.active: container.dockingWindow && mouseArea.drag.active
                        function remove() {
                            container.removed()
                        }
                    }
                    MouseArea {
                        id: mouseArea
                        drag.target: dragTarget
                        anchors.fill: parent
                        onClicked: parent.click()
                        onReleased: () => {
                            if (!drag.active)
                                return
                            if (dragTarget.Drag.target) {
                                dragTarget.Drag.drop()
                            }
                        }
                    }
                }
                ToolButton {
                    icon.source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/MoreHorizontal16Filled.svg"
                    display: AbstractButton.IconOnly
                    text: qsTr("Menu")
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
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.AllButtons
        cursorShape: undefined
        onPressed: (mouse) => {
            container.activated()
            mouse.accepted = false
        }
    }
}