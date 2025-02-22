import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.impl

import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Item {
    id: view
    default property list<QtObject> contentData: []
    property int edge: Qt.LeftEdge
    property int currentIndex: -1
    readonly property QtObject currentItem: currentIndex < 0 ? null : contentData[currentIndex]
    implicitWidth: view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge ? tabBar.width + (panel.visible ? panel.width : 0) : 0
    implicitHeight: view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge ? tabBar.height + (panel.visible ? panel.height : 0) : 0

    function removeContent(index) {
        let previousCurrentIndex = currentIndex
        tabBar.currentIndex = -1
        view.contentData = [...view.contentData.slice(0, index), ...view.contentData.slice(index + 1)]
        if (index === previousCurrentIndex)
            previousCurrentIndex = -1
        else if (index < previousCurrentIndex)
            previousCurrentIndex--
        tabBar.currentIndex = previousCurrentIndex
    }

    function insertContent(index, content, current) {
        let previousCurrentIndex = currentIndex
        tabBar.currentIndex = -1
        view.contentData = [...view.contentData.slice(0, index), content, ...view.contentData.slice(index)]
        if (index <= previousCurrentIndex)
            previousCurrentIndex++
        if (current)
            previousCurrentIndex = index
        tabBar.currentIndex = previousCurrentIndex
    }

    function moveContent(sourceIndex, targetIndex, current) {
        let content = view.contentData[sourceIndex]
        removeContent(sourceIndex)
        if (sourceIndex < targetIndex)
            targetIndex--
        insertContent(targetIndex, content, current)
    }

    Component {
        id: floatingWindow
        Window {
            id: window
            required property DockingPane currentItem
            transientParent: view.Window.window
            DockingPanel {
                id: panel
                anchors.fill: parent
                pane: window.currentItem
            }
            onVisibleChanged: () => {
                if (visible)
                    panel.set()
            }
        }
    }
    Pane {
        id: tabBar
        implicitWidth: 40
        implicitHeight: 40
        width: view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge ? parent.width : undefined
        height: view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge ? parent.height: undefined
        anchors.right: view.edge === Qt.RightEdge ? parent.right : undefined
        anchors.bottom: view.edge === Qt.BottomEdge ? parent.bottom : undefined
        property QtObject _currentItem: null
        property int currentIndex: -1
        function indexOfStretch(model) {
            for (let i = 0; i < model.length; i++) {
                if (model[i] instanceof DockingStretch)
                    return i
            }
            return 0
        }
        property int stretchIndex: indexOfStretch(view.contentData)
        Binding {
            view.currentIndex: tabBar.currentIndex
        }
        GridLayout {
            rowSpacing: 6
            columnSpacing: 6
            anchors.fill: parent
            anchors.margins: 6
            columns: view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge ? 1 : -1
            rows: view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge ? 1 : -1
            Repeater {
                model: view.contentData
                Item {
                    id: tabItem
                    required property QtObject modelData
                    required property int index
                    property bool _isStretch: !(modelData instanceof DockingPane || modelData instanceof Action)
                    Layout.fillWidth: (view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge) && _isStretch
                    Layout.fillHeight: (view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge) && _isStretch
                    width: 28
                    height: 28
                    function createWindow() {
                        if (!modelData.window) {
                            modelData.window = floatingWindow.createObject(modelData, {
                                width: modelData.implicitWidth >= 200 ? modelData.implicitWidth : (view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge ? view.width: 400),
                                height: modelData.implicitHeight >= 200 ? modelData.implicitHeight : (view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge ? view.height: 400),
                                currentItem: modelData,
                            })
                        }
                        modelData.window.show()
                    }
                    function undockPanelOnDrag() {
                        if (!(modelData instanceof DockingPane))
                            return
                        modelData.dock = false
                        createWindow()
                        let p = GlobalHelper.cursorPos()
                        modelData.window.x = p.x
                        modelData.window.y = p.y
                    }
                    Connections {
                        target: tabItem.modelData instanceof DockingPane ? tabItem.modelData : null
                        enabled: tabItem.modelData instanceof DockingPane
                        function onDockChanged() {
                            if (tabItem.modelData.dock && modelData.window?.visible) {
                                tabItem.modelData.window.close()
                                tabBar.currentIndex = tabItem.index
                            } else if (!tabItem.modelData.dock && tabBar.currentIndex === tabItem.index) {
                                tabBar.currentIndex = -1
                                tabItem.createWindow()
                            }
                        }
                    }
                    ToolButton {
                        id: tabButton
                        anchors.fill: parent
                        enabled: !tabItem._isStretch
                        background.opacity: _isStretch ? 0 : 1
                        down: mouseArea.pressed
                        icon.source: !_isStretch ? modelData.iconSource : ""
                        display: AbstractButton.IconOnly
                        highlighted: modelData instanceof DockingPane && (modelData.dock && view.currentIndex === index || !modelData.dock && modelData.window && modelData.window.visible)
                        action: modelData instanceof Action ? modelData : null
                        DescriptiveText.activated: hovered && !_isStretch
                        DescriptiveText.toolTip: mouseArea.drag.active ? "" : (modelData instanceof Action ? modelData.text : modelData.title) ?? ""
                        DescriptiveText.statusTip: modelData.description ?? ""
                        onClicked: () => {
                            if (modelData instanceof Action) {
                                return
                            }
                            if (modelData.dock) {
                                if (view.currentIndex === index) {
                                    tabBar.currentIndex = -1
                                } else {
                                    tabBar.currentIndex = index
                                }
                            } else {
                                if (modelData.window?.visible) {
                                    modelData.window.close()
                                } else {
                                    createWindow()
                                }
                            }
                        }

                    }
                    Item {
                        id: dragTarget
                        readonly property QtObject modelData: tabItem.modelData
                        readonly property QtObject container: tabBar
                        readonly property bool current: view.currentIndex === tabItem.index
                        width: 28
                        height: 28
                        Popup {
                            background: Item {}
                            padding: 0
                            width: 28
                            height: 28
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
                            view.removeContent(tabItem.index)
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
                        width: 36
                        height: 2
                        anchors.horizontalCenter: parent.horizontalCenter
                        y: -4
                        color: Theme.accentColor
                        visible: (view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge) && dropArea.containsDrag && dropArea._dropBefore
                    }
                    Rectangle {
                        id: bottomDropIndicator
                        width: 36
                        height: 2
                        anchors.horizontalCenter: parent.horizontalCenter
                        y: parent.height + 2
                        color: Theme.accentColor
                        visible: (view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge) && dropArea.containsDrag && !dropArea._dropBefore
                    }
                    Rectangle {
                        id: leftDropIndicator
                        width: 2
                        height: 36
                        anchors.verticalCenter: parent.verticalCenter
                        x: -4
                        color: Theme.accentColor
                        visible: (view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge) && dropArea.containsDrag && dropArea._dropBefore
                    }
                    Rectangle {
                        id: rightDropIndicator
                        width: 2
                        height: 36
                        anchors.verticalCenter: parent.verticalCenter
                        x: parent.width + 2
                        color: Theme.accentColor
                        visible: (view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge) && dropArea.containsDrag && !dropArea._dropBefore
                    }
                }
            }
        }
    }
    DockingPanel {
        id: panel
        pane: view.currentItem
        visible: view.currentItem !== null
        width: view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge ? parent.width * (visible ? 1 : 0) : undefined
        height: view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge ? parent.height * (visible ? 1 : 0) : undefined
        anchors.left: view.edge === Qt.LeftEdge ? tabBar.right : undefined
        anchors.right: view.edge === Qt.RightEdge ? tabBar.left : undefined
        anchors.top: view.edge === Qt.TopEdge ? tabBar.bottom : undefined
        anchors.bottom: view.edge === Qt.BottomEdge ? tabBar.top : undefined
        Rectangle {
            anchors.left: view.edge !== Qt.RightEdge ? parent.left : undefined
            anchors.right: view.edge !== Qt.LeftEdge ? parent.right : undefined
            anchors.top: view.edge !== Qt.BottomEdge ? parent.top : undefined
            anchors.bottom: view.edge !== Qt.TopEdge ? parent.bottom : undefined
            implicitHeight: 1
            implicitWidth: 1
            color: Theme.splitterColor
            z: 10
        }
    }
    }