import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import SVSCraft.UIComponents

ApplicationWindow {
    visible: true
    width: 1024
    height: 800

    background: Rectangle {
        color: Theme.backgroundQuaternaryColor
    }

    MessageDialog {
        id: actionDialog
        title: "Action"
        text: "Action triggered"
    }

    RowLayout {
        anchors.fill: parent
        spacing: 4
        DockingView {
            Layout.fillHeight: true
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
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            DockingView {
                Layout.fillWidth: true
                edge: Qt.TopEdge
                DockingPane {
                    title: "Test 5"
                    iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon5.svg"
                }
                DockingStretch {
                }
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            DockingView {
                Layout.fillWidth: true
                edge: Qt.BottomEdge
                DockingPane {
                    title: "Test 6"
                    iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon6.svg"
                }
                DockingStretch {
                }
                DockingPane {
                    title: "Test 7"
                    iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon7.svg"
                }
            }
        }
        DockingView {
            Layout.fillHeight: true
            edge: Qt.RightEdge
            DockingPane {
                title: "Test 8"
                iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon8.svg"
            }
            DockingStretch {
            }
        }
    }

}