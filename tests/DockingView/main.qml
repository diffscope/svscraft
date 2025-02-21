import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SVSCraft.UIComponents

ApplicationWindow {
    visible: true
    width: 1024
    height: 800

    background: Rectangle {
        color: Theme.backgroundQuaternaryColor
    }

    RowLayout {
        anchors.fill: parent
        spacing: 4
        DockingView {
            Layout.fillHeight: true
            DockingPane {
                title: "Test 1"
                iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon.svg"
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
                iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon.svg"
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
                iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon.svg"
                dock: false
            }
            Action {
                text: "Action"
                icon.source: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon.svg"
                onTriggered: console.log("Action")
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
                    iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon.svg"
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
                    iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon.svg"
                }
                DockingStretch {
                }
                DockingPane {
                    title: "Test 7"
                    iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon.svg"
                }
            }
        }
        DockingView {
            Layout.fillHeight: true
            edge: Qt.RightEdge
            DockingPane {
                title: "Test 8"
                iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon.svg"
            }
            DockingStretch {
            }
        }
    }

}