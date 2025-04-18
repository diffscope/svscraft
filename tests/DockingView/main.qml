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
import QtQuick.Dialogs

import SVSCraft.UIComponents

ApplicationWindow {
    id: main
    visible: true
    width: 1024
    height: 800

    readonly property double minimumPanelSize: 100

    background: Rectangle {
        color: Theme.backgroundQuaternaryColor
    }

    MessageDialog {
        id: actionDialog
        title: "Action"
        text: "Action triggered"
    }

    menuBar: MenuBar {
        Menu {
            title: "Actions"
            Action {
                text: "Show Docking Window"
                onTriggered: () => {
                    if (dockingWindowPane.Docking.dockingView) {
                        dockingWindowPane.Docking.dockingView.showPane(dockingWindowPane)
                    } else {
                        dockingWindow.show()
                    }
                }
            }
        }
    }

    DockingWindow {
        id: dockingWindow
        visible: true
        width: 400
        height: 400
        DockingPane {
            id: dockingWindowPane
            title: "Docking Window"
            iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/Window16Filled.svg"
            menu: Menu {
                Action {
                    text: "Eject from Docking View"
                    enabled: dockingWindowPane.Docking.dockingView
                    onTriggered: () => {
                        dockingWindowPane.Docking.dockingView.removeContent(dockingWindowPane)
                        dockingWindow.show()
                    }
                }
            }
        }
    }

    SplitView {
        anchors.fill: parent
        ThemedItem.splitHandleEnabled: rightDock.panelOpened
        SplitView {
            SplitView.minimumWidth: leftDock.SplitView.minimumWidth + main.minimumPanelSize
            SplitView.fillWidth: true
            ThemedItem.splitHandleEnabled: leftDock.panelOpened
            DockingView {
                id: leftDock
                property double preferredPanelSize: 400
                SplitView.minimumWidth: barSize + (panelOpened ? main.minimumPanelSize : 0)
                SplitView.preferredWidth: barSize + (panelOpened ? preferredPanelSize : 0)
                SplitView.maximumWidth: !panelOpened ? barSize : Infinity
                clip: true
                panelSize: width - barSize
                onPanelSizeChanged: () => {
                    if (panelSize > 0)
                        preferredPanelSize = panelSize
                }
                DockingPane {
                    title: "Test 1"
                    iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon1.svg"
                    header: Button { text: "header 1" }
                    padding: 8
                    implicitWidth: 400
                    implicitHeight: 300
                    menu: Menu {
                        Action {
                            text: "Menu Action"
                        }
                    }
                    ColumnLayout {
                        Label {
                            text: "Pane 1"
                        }
                        TextField {

                        }
                    }

                }
                DockingPane {
                    title: "Test 2"
                    iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon2.svg"
                    header: Button { text: "header 2" }
                    padding: 8
                    Label {
                        text: "Pane 2"
                    }
                }
                DockingStretch {
                }
                DockingPane {
                    title: "Test 3"
                    iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon3.svg"
                    dock: false
                }
                Action {
                    text: "Action"
                    icon.source: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon4.svg"
                    onTriggered: actionDialog.open()
                }
            }
            SplitView {
                id: middleSplitView
                orientation: Qt.Vertical
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                SplitView.minimumWidth: main.minimumPanelSize
                ThemedItem.splitHandleVisible: topDock.panelOpened || bottomDock.panelOpened
                Item {
                    SplitView.minimumHeight: !bottomDock.panelOpened ? middleSplitView.height - bottomDock.barSize - 1 : topDock.barSize + (topDock.panelOpened ? main.minimumPanelSize : 0)
                    SplitView.preferredHeight: (middleSplitView.height - 1) / 2
                    SplitView.maximumHeight: Math.max(SplitView.minimumHeight, !topDock.panelOpened ? topDock.barSize : Infinity)
                    DockingView {
                        id: topDock
                        width: parent.width
                        anchors.top: parent.top
                        edge: Qt.TopEdge
                        property double preferredPanelSize: 400
                        panelSize: parent.height - barSize
                        onPanelSizeChanged: () => {
                            if (panelSize > 0)
                                preferredPanelSize = panelSize
                        }
                        DockingPane {
                            title: "编曲面板"
                            iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon5.svg"
                            Component.onCompleted: Docking.dockingView.showPane(this)
                        }
                        DockingStretch {
                        }
                    }
                    Rectangle {
                        width: parent.width
                        height: 1
                        anchors.top: topDock.bottom
                        color: Theme.splitterColor
                        visible: !topDock.panelOpened
                    }
                }
                Item {
                    SplitView.minimumHeight: bottomDock.barSize + (bottomDock.panelOpened ? main.minimumPanelSize : 0)
                    SplitView.preferredHeight: (middleSplitView.height - 1) / 2
                    SplitView.maximumHeight: Math.max(SplitView.minimumHeight, !bottomDock.panelOpened ? bottomDock.barSize : Infinity)
                    DockingView {
                        id: bottomDock
                        width: parent.width
                        anchors.bottom: parent.bottom
                        edge: Qt.BottomEdge
                        property double preferredPanelSize: 400
                        panelSize: parent.height - barSize
                        onPanelSizeChanged: () => {
                            if (panelSize > 0)
                                preferredPanelSize = panelSize
                        }
                        DockingPane {
                            title: "钢琴卷帘"
                            iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon6.svg"
                            Component.onCompleted: Docking.dockingView.showPane(this)
                        }
                        DockingStretch {
                        }
                        DockingPane {
                            title: "Test 7"
                            iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon7.svg"
                        }
                    }
                    Rectangle {
                        width: parent.width
                        height: 1
                        anchors.bottom: bottomDock.top
                        color: Theme.splitterColor
                        visible: !bottomDock.panelOpened
                    }
                }
            }
        }

        DockingView {
            id: rightDock
            SplitView.minimumWidth: barSize + (panelOpened ? main.minimumPanelSize : 0)
            SplitView.preferredWidth: barSize + (panelOpened ? preferredPanelSize : 0)
            SplitView.maximumWidth: !panelOpened ? barSize : Infinity
            clip: true
            edge: Qt.RightEdge
            property double preferredPanelSize: 400
            panelSize: width - barSize
            onPanelSizeChanged: () => {
                if (panelSize > 0)
                    preferredPanelSize = panelSize
            }
            DockingPane {
                title: "Test 8"
                iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon8.svg"
            }
            DockingStretch {
            }
        }
    }

}