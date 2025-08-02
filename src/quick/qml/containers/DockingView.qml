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

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Item {
    id: view

    Accessible.role: Accessible.Pane
    Accessible.name: qsTr("Docking View")

    readonly property int firstIndex: tabBar.firstIndex
    readonly property int lastIndex: tabBar.lastIndex
    readonly property DockingPane firstItem: firstIndex < 0 ? null : contentData[firstIndex]
    readonly property DockingPane lastItem: lastIndex < 0 ? null : contentData[lastIndex]
    readonly property int stretchIndex: tabBar.stretchIndex
    readonly property bool panelOpened: panel.visible
    readonly property list<int> undockedIndices: {
        let a = []
        for (let i = 0; i < contentData.length; i++) {
            let o = contentData[i]
            if (o instanceof DockingPane && !o.dock) {
                a.push(i)
            }
        }
        return a
    }
    readonly property list<DockingPane> undockedItems: undockedIndices.map(i => contentData[i])

    default property list<QtObject> contentData: []
    property int edge: Qt.LeftEdge
    property double barSize: 32
    property double panelSize: 400
    property int barBackgroundLevel: SVS.BL_Primary
    property bool firstActive: false
    property bool lastActive: false
    property DockingPane activeUndockedPane: null

    signal firstActivated()
    signal lastActivated()
    signal undockedActivated(QtObject dockingPane)
    signal undockedDeactivated(QtObject dockingPane)

    implicitWidth: view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge ? tabBar.width + (panel.visible ? panel.width : 0) : 0
    implicitHeight: view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge ? tabBar.height + (panel.visible ? panel.height : 0) : 0

    function removeContent(index, keepWindow) {
        if (typeof(index) !== "number") {
            for (let i = 0; i < view.contentData.length; i++) {
                if (view.contentData[i] === index) {
                    index = i
                    break
                }
            }
        }
        if (typeof(index) !== "number" || index < 0 || index >= view.contentData.length)
            return
        let previousFirstIndex = firstIndex
        tabBar.firstIndex = -1
        let previousLastIndex = lastIndex
        tabBar.lastIndex = -1
        let itemToRemove = view.contentData[index]
        itemToRemove.DockingImpl.dockingView = null
        if (!keepWindow)
            itemToRemove.DockingImpl.window?.destroy()
        view.contentData = [...view.contentData.slice(0, index), ...view.contentData.slice(index + 1)]
        if (index === previousFirstIndex)
            previousFirstIndex = -1
        else if (index < previousFirstIndex)
            previousFirstIndex--
        if (index === previousLastIndex)
            previousLastIndex = -1
        else if (index < previousLastIndex)
            previousLastIndex--
        tabBar.firstIndex = previousFirstIndex
        tabBar.lastIndex = previousLastIndex
    }
    function insertContent(index, content, current) {
        if (index < 0 || index > view.contentData.length)
            return
        let previousFirstIndex = firstIndex
        tabBar.firstIndex = -1
        let previousLastIndex = lastIndex
        tabBar.lastIndex = -1
        view.contentData = [...view.contentData.slice(0, index), content, ...view.contentData.slice(index)]
        if (index <= previousFirstIndex)
            previousFirstIndex++
        if (index <= previousLastIndex)
            previousLastIndex++
        tabBar.firstIndex = previousFirstIndex
        tabBar.lastIndex = previousLastIndex
        if (current)
            showPane(index)
    }
    function moveContent(sourceIndex, targetIndex, current) {
        if (sourceIndex < 0 || sourceIndex >= view.contentData.length)
            return
        if (targetIndex < 0 || targetIndex > view.contentData.length)
            return
        let content = view.contentData[sourceIndex]

        let previousFirstIndex = firstIndex
        tabBar.firstIndex = -1
        let previousLastIndex = lastIndex
        tabBar.lastIndex = -1

        let contentData = [...view.contentData.slice(0, sourceIndex), ...view.contentData.slice(sourceIndex + 1)]
        if (sourceIndex === previousFirstIndex)
            previousFirstIndex = -1
        else if (sourceIndex < previousLastIndex)
            previousLastIndex--
        if (sourceIndex === previousLastIndex)
            previousLastIndex = -1
        else if (sourceIndex < previousLastIndex)
            previousLastIndex--

        if (sourceIndex < targetIndex)
            targetIndex--

        view.contentData = [...contentData.slice(0, targetIndex), content, ...contentData.slice(targetIndex)]
        if (targetIndex <= previousFirstIndex)
            previousFirstIndex++
        if (targetIndex <= previousLastIndex)
            previousLastIndex++
        tabBar.firstIndex = previousFirstIndex
        tabBar.lastIndex = previousLastIndex

        if (current)
            showPane(targetIndex)
    }
    function showPane(content) {
        let index = -1
        let pane = null
        if (typeof(content) === "number") {
            index = content
            pane = view.contentData[index]
        } else {
            for (let i = 0; i < view.contentData.length; i++) {
                if (view.contentData[i] === content) {
                    index = i
                    pane = content
                    break
                }
            }
        }
        if (index === -1)
            return
        if (!(pane instanceof DockingPane))
            return
        if (pane.dock) {
            if (index < stretchIndex)
                tabBar.firstIndex = index
            else
                tabBar.lastIndex = index
        } else {
            createWindow(pane)
        }
    }
    function hidePane(content) {
        let index = -1
        let pane = null
        if (typeof(content) === "number") {
            index = content
            pane = view.contentData[index]
        } else {
            for (let i = 0; i < view.contentData.length; i++) {
                if (view.contentData[i] === content) {
                    index = i
                    pane = content
                    break
                }
            }
        }
        if (index === -1)
            return
        if (!(pane instanceof DockingPane))
            return
        if (pane.dock) {
            if (firstIndex === index)
                tabBar.firstIndex = -1
            else if (lastIndex === index)
                tabBar.lastIndex = -1
        } else {
            pane.Docking.window?.close()
        }
    }
    function createWindow(content) {
        if (!content.Docking.window) {
            content.DockingImpl.window = floatingWindow.createObject(content, {
                width: content.implicitWidth >= 200 ? content.implicitWidth : (view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge ? view.width: 400),
                height: content.implicitHeight >= 200 ? content.implicitHeight : (view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge ? view.height: 400),
                currentItem: content,
            })
        }
        content.Docking.window.show()
    }

    Component {
        id: floatingWindow
        Window {
            id: window
            required property DockingPane currentItem
            title: currentItem.title + " - " + view.Window.window.title
            transientParent: view.Window.window
            LayoutMirroring.enabled: view.LayoutMirroring.enabled
            LayoutMirroring.childrenInherit: true
            DockingPanel {
                id: panel
                anchors.fill: parent
                pane: window.currentItem
                active: view.activeUndockedPane === window.currentItem
            }
            onVisibleChanged: () => {
                if (visible)
                    panel.set()
            }
            onActiveChanged: () => {
                GlobalHelper.setProperty(null, "", "")
                if (active && GlobalHelper.focusWindow() === window) {
                    view.undockedActivated(currentItem)
                } else if (GlobalHelper.focusWindow() !== window) {
                    view.undockedDeactivated(currentItem)
                }

            }
        }
    }
    Pane {
        id: tabBar
        Accessible.name: qsTr("Docking View")
        Accessible.role: Accessible.PageTabList
        implicitWidth: view.barSize
        implicitHeight: view.barSize
        width: view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge ? parent.width : undefined
        height: view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge ? parent.height: undefined
        anchors.top: view.edge === Qt.TopEdge ? parent.top : undefined
        anchors.left: view.edge === Qt.LeftEdge ? parent.left : undefined
        anchors.right: view.edge === Qt.RightEdge ? parent.right : undefined
        anchors.bottom: view.edge === Qt.BottomEdge ? parent.bottom : undefined
        LayoutMirroring.enabled: false
        ThemedItem.backgroundLevel: view.barBackgroundLevel
        property int firstIndex: -1
        property int lastIndex: -1
        function indexOfStretch(model) {
            let index = -1
            for (let i = 0; i < model.length; i++) {
                if (model[i] instanceof DockingStretch) {
                    if (index === -1) {
                        index = i
                    } else {
                        throw new Error("More than one DockingStretches in DockingView")
                    }
                }
            }
            if (index === -1) {
                throw new Error("No DockingStretch in DockingView")
            }
            return index
        }
        property int stretchIndex: indexOfStretch(view.contentData)
        GridLayout {
            rowSpacing: 4
            columnSpacing: 4
            anchors.fill: parent
            anchors.margins: 4
            columns: view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge ? 1 : -1
            rows: view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge ? 1 : -1
            LayoutMirroring.enabled: false
            Repeater {
                model: view.contentData
                Item {
                    id: tabItem
                    required property QtObject modelData
                    required property int index
                    property bool _isStretch: !(modelData instanceof DockingPane || modelData instanceof Action)
                    Layout.fillWidth: (view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge) && _isStretch
                    Layout.fillHeight: (view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge) && _isStretch
                    Component.onCompleted: modelData.DockingImpl.dockingView = view
                    width: 24
                    height: 24
                    function undockPanelOnDrag() {
                        if (!(modelData instanceof DockingPane))
                            return
                        if (modelData.locked)
                            return
                        modelData.dock = false
                        view.createWindow(modelData)
                        let p = GlobalHelper.cursorPos()
                        modelData.Docking.window.x = p.x
                        modelData.Docking.window.y = p.y
                    }
                    Connections {
                        target: tabItem.modelData instanceof DockingPane ? tabItem.modelData : null
                        enabled: tabItem.modelData instanceof DockingPane
                        function onDockChanged() {
                            if (tabItem.modelData.dock && modelData.Docking.window?.visible) {
                                tabItem.modelData.Docking.window.close()
                                showPane(tabItem.modelData)
                            } else if (!tabItem.modelData.dock) {
                                if (tabBar.firstIndex === tabItem.index) {
                                    tabBar.firstIndex = -1
                                    view.createWindow(tabItem.modelData)
                                } else if (tabBar.lastIndex === tabItem.index) {
                                    tabBar.lastIndex = -1
                                    view.createWindow(tabItem.modelData)
                                }
                            }
                        }
                    }
                    ToolButton {
                        id: tabButton
                        anchors.fill: parent
                        enabled: !tabItem._isStretch
                        visible: !tabItem._isStretch
                        background.opacity: _isStretch ? 0 : 1
                        down: mouseArea.pressed
                        icon: !_isStretch ? modelData.icon : GlobalHelper.defaultIcon()
                        display: AbstractButton.IconOnly
                        highlighted: modelData instanceof DockingPane && (modelData.dock && (view.firstIndex === index || view.lastIndex === index) || !modelData.dock && modelData.Docking.window && modelData.Docking.window.visible)
                        action: modelData instanceof Action ? modelData : null
                        DescriptiveText.activated: hovered && !_isStretch
                        text: (modelData instanceof Action ? modelData.text : modelData.title) ?? ""
                        DescriptiveText.toolTip: mouseArea.drag.active ? "" : (modelData instanceof Action ? modelData.text : modelData.title) ?? ""
                        DescriptiveText.statusTip: modelData.description ?? ""
                        DescriptiveText.bindAccessibleDescription: false
                        Accessible.role: modelData instanceof Action ? checkable ? Accessible.CheckBox : Accessible.Button : Accessible.PageTab
                        Accessible.checkable: modelData instanceof Action ? modelData.checkable : true
                        Accessible.checked: modelData instanceof Action ? modelData.checked: highlighted
                        Accessible.description: modelData instanceof Action ? "" : (modelData.description ?? "") + "\n" + qsTr("Show or hide the docking pane")
                        onClicked: () => {
                            if (modelData instanceof Action) {
                                return
                            }
                            if (highlighted) {
                                hidePane(tabItem.modelData)
                            } else {
                                showPane(tabItem.modelData)
                            }
                        }

                    }
                    Item {
                        id: dragTarget
                        readonly property QtObject modelData: tabItem.modelData
                        readonly property QtObject container: tabBar
                        readonly property bool current: tabButton.highlighted
                        width: parent.width
                        height: parent.height
                        Popup {
                            background: Item {}
                            padding: 0
                            width: parent.width
                            height: parent.height
                            visible: mouseArea.drag.active
                            IconLabel {
                                anchors.fill: parent
                                icon: tabButton.icon
                                display: AbstractButton.IconOnly
                            }
                        }
                        visible: !tabItem._isStretch
                        anchors.centerIn: mouseArea.drag.active ? null : parent
                        Drag.keys: ["SVSCraft.UIComponent.DockingView"]
                        Drag.hotSpot.x: width / 2
                        Drag.hotSpot.y: height / 2
                        Drag.active: !tabItem._isStretch && mouseArea.drag.active
                        function remove() {
                            view.removeContent(tabItem.index, true)
                        }
                        function move(index) {
                            view.moveContent(tabItem.index, index, current)
                        }
                    }
                    DropArea {
                        id: dropArea
                        anchors.fill: parent
                        anchors.margins: -3
                        keys: ["SVSCraft.UIComponent.DockingView"]
                        readonly property bool _dropBefore: tabItem.index < tabBar.stretchIndex || (tabItem.index === tabBar.stretchIndex && ((view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge) && drag.y <= height / 2 || (view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge) && drag.x <= width / 2))
                        onDropped: (drop) => {
                            let sourceModelData = drop.source.modelData
                            let current = drop.source.current
                            if (sourceModelData === tabItem.modelData)
                                return
                            if (drop.source.container === tabBar) {
                                drop.source.move(tabItem.index + (_dropBefore ? 0 : 1))
                            } else {
                                drop.source.remove()
                                view.insertContent(tabItem.index + (_dropBefore ? 0 : 1), sourceModelData, current)
                            }
                        }
                    }
                    MouseArea {
                        id: mouseArea
                        drag.target: dragTarget
                        anchors.fill: parent
                        onClicked: tabButton.click()
                        onReleased: () => {
                            if (!drag.active)
                                return
                            if (dragTarget.Drag.target) {
                                dragTarget.Drag.drop()
                            } else {
                                tabItem.undockPanelOnDrag()
                            }
                        }
                    }
                    Rectangle {
                        id: topDropIndicator
                        width: view.barSize - 2
                        height: 2
                        anchors.horizontalCenter: parent.horizontalCenter
                        y: -4
                        color: Theme.accentColor
                        visible: (view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge) && dropArea.containsDrag && dropArea._dropBefore
                    }
                    Rectangle {
                        id: bottomDropIndicator
                        width: view.barSize - 2
                        height: 2
                        anchors.horizontalCenter: parent.horizontalCenter
                        y: parent.height + 2
                        color: Theme.accentColor
                        visible: (view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge) && dropArea.containsDrag && !dropArea._dropBefore
                    }
                    Rectangle {
                        id: leftDropIndicator
                        width: 2
                        height: view.barSize - 2
                        anchors.verticalCenter: parent.verticalCenter
                        x: -4
                        color: Theme.accentColor
                        visible: (view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge) && dropArea.containsDrag && dropArea._dropBefore
                    }
                    Rectangle {
                        id: rightDropIndicator
                        width: 2
                        height: view.barSize - 2
                        anchors.verticalCenter: parent.verticalCenter
                        x: parent.width + 2
                        color: Theme.accentColor
                        visible: (view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge) && dropArea.containsDrag && !dropArea._dropBefore
                    }
                }
            }
        }
    }
    Item {
        id: panel
        visible: view.firstItem !== null || view.lastItem !== null
        implicitWidth: view.panelSize
        implicitHeight: view.panelSize
        width: view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge ? parent.width * (visible ? 1 : 0) : undefined
        height: view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge ? parent.height * (visible ? 1 : 0) : undefined
        anchors.left: view.edge === Qt.LeftEdge ? tabBar.right : undefined
        anchors.right: view.edge === Qt.RightEdge ? tabBar.left : undefined
        anchors.top: view.edge === Qt.TopEdge ? tabBar.bottom : undefined
        anchors.bottom: view.edge === Qt.BottomEdge ? tabBar.top : undefined
        LayoutMirroring.enabled: false
        Rectangle {
            anchors.left: view.edge !== Qt.RightEdge ? parent.left : undefined
            anchors.right: view.edge !== Qt.LeftEdge ? parent.right : undefined
            anchors.top: view.edge !== Qt.BottomEdge ? parent.top : undefined
            anchors.bottom: view.edge !== Qt.TopEdge ? parent.bottom : undefined
            LayoutMirroring.enabled: false
            implicitHeight: 1
            implicitWidth: 1
            color: Theme.splitterColor
            z: 10
        }
        SplitView {
            id: splitView
            Accessible.role: Accessible.Grouping
            anchors.fill: parent
            orientation: view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge ? Qt.Horizontal : Qt.Vertical
            DockingPanel {
                pane: view.firstItem
                visible: view.firstItem !== null
                SplitView.preferredWidth: splitView.orientation === Qt.Horizontal ? (splitView.width - 1) / 2 : undefined
                SplitView.preferredHeight: splitView.orientation === Qt.Vertical ? (splitView.height - 1) / 2 : undefined
                onActivated: view.firstActivated()
                active: view.firstActive
            }
            DockingPanel {
                pane: view.lastItem
                visible: view.lastItem !== null
                SplitView.preferredWidth: splitView.orientation === Qt.Horizontal ? (splitView.width - 1) / 2 : undefined
                SplitView.preferredHeight: splitView.orientation === Qt.Vertical ? (splitView.height - 1) / 2 : undefined
                onActivated: view.lastActivated()
                active: view.lastActive
            }
        }
    }
}