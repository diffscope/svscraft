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
                orientation: Qt.Vertical
                SplitView.fillWidth: true
                SplitView.minimumWidth: main.minimumPanelSize
                ThemedItem.splitHandleVisible: topDock.panelOpened || bottomDock.panelOpened
                Item {
                    SplitView.minimumHeight: topDock.barSize + (topDock.panelOpened ? main.minimumPanelSize : 0)
                    SplitView.preferredHeight: topDock.barSize + topDock.preferredPanelSize
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
                    SplitView.preferredHeight: bottomDock.barSize + bottomDock.preferredPanelSize
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