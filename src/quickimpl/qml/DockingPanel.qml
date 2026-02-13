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
import QtQuick.Templates as T

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
    property var selectorModel: null
    signal hideTriggered()
    signal removed()
    signal activated()
    signal selectorActivated(selectedPane: DockingPane)

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
                T.ComboBox {
                    id: comboBox
                    padding: 4
                    rightPadding: container.selectorModel ? 20 : 4
                    implicitWidth: implicitContentWidth + leftPadding + rightPadding
                    implicitHeight: implicitContentHeight + topPadding + bottomPadding
                    Layout.alignment: Qt.AlignVCenter
                    contentItem: IconLabel {
                        spacing: 4
                        icon: container.pane?.icon ?? GlobalHelper.defaultIcon()
                        font: container.font
                        text: container.pane?.title ?? ""
                        color: Theme.foregroundPrimaryColor
                    }
                    indicator: Item {
                        x: comboBox.mirrored ? comboBox.padding : comboBox.width - width - comboBox.padding + 4
                        y: comboBox.topPadding + (comboBox.availableHeight - height) / 2
                        width: image.width + 8
                        visible: Boolean(container.selectorModel)
                        ColorImage {
                            id: image
                            anchors.centerIn: parent
                            color: Theme.foregroundPrimaryColor
                            sourceSize.width: 12
                            sourceSize.height: 12
                            source: "image://fluent-system-icons/chevron_down?size=12"
                        }
                    }
                    popup: T.Popup {
                        y: comboBox.height
                        width: contentItem.implicitWidth
                        height: Math.min(contentItem.implicitHeight, comboBox.Window.height - topMargin - bottomMargin)
                        topMargin: 6
                        bottomMargin: 6

                        contentItem: ListView {
                            clip: true
                            implicitWidth: {
                                let w = 100
                                for (let i = 0; i < count; i++) {
                                    let item = itemAtIndex(i)
                                    w = Math.max(w, item?.implicitWidth ?? 0)
                                }
                                return w
                            }
                            implicitHeight: contentHeight
                            model: comboBox.delegateModel
                            currentIndex: comboBox.currentIndex
                            highlightMoveDuration: 0

                            Rectangle {
                                z: 10
                                width: parent.width
                                height: parent.height
                                color: "transparent"
                                border.color: Theme.borderColor
                                radius: 4
                            }

                            T.ScrollIndicator.vertical: ScrollIndicator { }
                        }

                        background: Rectangle {
                            id: backgroundArea
                            color: Theme.buttonColor
                            radius: 4
                        }
                    }
                    textRole: "text"
                    valueRole: "data"
                    currentIndex: {
                        if (!container.selectorModel)
                            return 0
                        for (let i = 0; i < container.selectorModel.length; i++) {
                            if (container.selectorModel[i].data === container.pane)
                                return i
                        }
                        return 0
                    }
                    model: container.selectorModel
                    onActivated: (index) => container.selectorActivated(valueAt(index))
                    delegate: ItemDelegate {
                        required property var model
                        required property int index

                        width: ListView.view.width
                        height: 32
                        icon: model[comboBox.valueRole].icon
                        text: model[comboBox.textRole]
                        highlighted: comboBox.currentIndex === index
                        hoverEnabled: comboBox.hoverEnabled
                    }
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
                    icon.source: "image://fluent-system-icons/more_vertical"
                    display: AbstractButton.IconOnly
                    text: qsTr("Menu")
                    Layout.alignment: Qt.AlignVCenter
                    visible: container.pane?.menu ?? false
                    onClicked: container.pane.menu.popup()
                }
                ToolButton {
                    icon.source: container.dockingWindow ?
                        "image://fluent-system-icons/drag" :
                        container.pane?.dock ?
                        "image://fluent-system-icons/panel_separate_window" :
                        "image://fluent-system-icons/panel_left"
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
                    icon.source: "image://fluent-system-icons/subtract"
                    display: AbstractButton.IconOnly
                    text: qsTr("Hide")
                    Layout.alignment: Qt.AlignVCenter
                    onClicked: container.hideTriggered()
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
            color: Theme.paneSeparatorColor
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