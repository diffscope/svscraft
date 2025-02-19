import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SVSCraft.UIComponents as SVS

ApplicationWindow {
    visible: true
    width: 1024
    height: 800

    menuBar: MenuBar {
        ThemedItem.dividerStroke: SVS.Theme.DS_Border
        Menu {
            title: "&File"
            Action {
                text: "&Normal"
            }
            Action {
                text: "&Icon"
                icon.source: "icon.svg"
            }
            Action {
                text: "&Checkable"
                checkable: true
                shortcut: "Ctrl+A"
            }
            MenuSeparator {
            }
            Menu {
                title: "&Submenu"
                Action {
                    text: "Test"
                    shortcut: "Ctrl+B"
                }
            }
            Action {
                text: "&Disabled"
                enabled: false
                icon.source: "icon.svg"
                shortcut: "Ctrl+C"
            }
            Action {
                text: "&Disabled"
                enabled: false
                checkable: true
                checked: true
            }
        }
        Menu {
            title: "&Edit"
            Action {
                text: "&Test"
            }
        }
    }

    header: ToolBar {
        ThemedItem.dividerStroke: SVS.Theme.DS_Border
        Row {
            anchors.fill: parent
            spacing: 4
            ToolButton {
                text: "Normal"
            }
            ToolButton {
                text: "Checkable"
                icon.source: "icon.svg"
                checkable: true
            }
            ToolButton {
                icon.source: "icon.svg"
            }
        }
        enabled: !disabledCheckBox.checked
    }

    Pane {
        anchors.fill: parent
        ScrollView {
            anchors.fill: parent
            ColumnLayout {
                id: layout
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 8
                spacing: 8

                CheckBox {
                    id: disabledCheckBox
                    text: "Disabled"
                }

                GroupBox {
                    title: "按钮"
                    RowLayout {
                        id: buttonRowLayout
                        spacing: 8
                        enabled: !disabledCheckBox.checked
                        Button {
                            ThemedItem.controlType: SVS.Theme.CT_Normal
                            text: "Normal"
                            icon.source: "icon.svg"
                        }
                        Button {
                            ThemedItem.controlType: SVS.Theme.CT_Accent
                            text: "Accent"
                            icon.source: "icon.svg"
                        }
                        Button {
                            ThemedItem.controlType: SVS.Theme.CT_Error
                            text: "Error"
                            icon.source: "icon.svg"
                        }
                        Button {
                            ThemedItem.controlType: SVS.Theme.CT_Normal
                            text: "Normal Flat"
                            flat: true
                            icon.source: "icon.svg"
                        }
                        Button {
                            icon.source: "icon.svg"
                        }
                        Button {
                            ThemedItem.controlType: SVS.Theme.CT_Accent
                            text: "Accent Flat"
                            flat: true
                            icon.source: "icon.svg"
                        }
                        Button {
                            ThemedItem.controlType: SVS.Theme.CT_Error
                            text: "Error Flat"
                            flat: true
                            icon.source: "icon.svg"
                        }
                        Button {
                            text: "Checkable"
                            checkable: true
                            icon.source: "icon.svg"
                        }
                        Button {
                            text: "Checkable Flat"
                            checkable: true
                            flat: true
                            icon.source: "icon.svg"
                        }
                        Button {
                            text: "Uncolorized Icon"
                            icon.source: "icon_alt.svg"
                            icon.color: "transparent"
                        }
                    }

                }

                GroupBox {
                    title: "选择框"
                    RowLayout {
                        anchors.fill: parent
                        spacing: 8
                        CheckBox {
                            text: "Check Box"
                            tristate: true
                        }
                        enabled: !disabledCheckBox.checked
                    }
                }

                GroupBox {
                    title: "单选框"
                    RowLayout {
                        anchors.fill: parent
                        spacing: 8
                        RadioButton {
                            text: "Radio Button"
                        }
                        RadioButton {
                            text: "Radio Button"
                        }
                        enabled: !disabledCheckBox.checked
                    }
                }

                GroupBox {
                    title: "开关"
                    RowLayout {
                        anchors.fill: parent
                        spacing: 8
                        Switch {
                            text: "Switch"
                        }
                        enabled: !disabledCheckBox.checked
                    }
                }

                GroupBox {
                    id: groupBox
                    title: "分组框 & 框架"
                    Layout.fillWidth: true
                    ThemedItem.foldable: foldableCheckBox.checked
                    enabled: !disabledCheckBox.checked
                    Item {
                        width: parent.width
                        implicitHeight: groupBox.ThemedItem.folded ? 0 : 100
                        clip: true
                        visible: implicitHeight !== 0
                        Behavior on implicitHeight {
                            NumberAnimation {
                                duration: SVS.Theme.visualEffectAnimationDuration
                                easing.type: Easing.OutCubic
                            }
                        }
                        Frame {
                            anchors.fill: parent
                            ThemedItem.backgroundLevel: SVS.Theme.BL_Secondary
                            CheckBox {
                                id: foldableCheckBox
                                text: "Foldable"
                                checked: true
                            }
                        }
                    }
                }

                GroupBox {
                    title: "滑块"
                    RowLayout {
                        anchors.fill: parent
                        spacing: 8
                        Slider {
                            id: slider1
                            ThemedItem.trackStart: 0.25
                            orientation: Qt.Horizontal
                        }
                        Slider {
                            ThemedItem.trackStart: slider1.position
                            orientation: Qt.Vertical
                        }
                        enabled: !disabledCheckBox.checked
                    }
                }

                GroupBox {
                    title: "文本框"
                    RowLayout {
                        anchors.fill: parent
                        spacing: 8
                        TextField {
                            placeholderText: "placeholder"
                        }
                        enabled: !disabledCheckBox.checked
                    }
                }

                GroupBox {
                    title: "组合框"
                    RowLayout {
                        anchors.fill: parent
                        spacing: 8
                        ComboBox {
                            model: ["Test 1", "Test 2", "Test 3"]
                        }
                        ComboBox {
                            model: ["Editable"]
                            editable: true
                        }
                        enabled: !disabledCheckBox.checked
                    }
                }

                GroupBox {
                    title: "标签栏"
                    RowLayout {
                        anchors.fill: parent
                        spacing: 8
                        TabBar {
                            TabButton {
                                text: "Test 1"
                                icon.source: "icon.svg"
                            }
                            TabButton {
                                text: "Test 2"
                                icon.source: "icon.svg"
                            }
                            TabButton {
                                text: "Test 3"
                                icon.source: "icon.svg"
                            }
                        }
                        TabBar {
                            ThemedItem.flat: true
                            TabButton {
                                text: "Flat Top"
                                icon.source: "icon.svg"
                                width: implicitWidth
                                ThemedItem.tabIndicator: SVS.Theme.TI_Top
                            }
                            TabButton {
                                text: "Flat Bottom"
                                icon.source: "icon.svg"
                                width: implicitWidth
                                ThemedItem.tabIndicator: SVS.Theme.TI_Bottom
                            }
                            TabButton {
                                text: "Flat Left"
                                icon.source: "icon.svg"
                                width: implicitWidth
                                ThemedItem.tabIndicator: SVS.Theme.TI_Left
                            }
                            TabButton {
                                text: "Flat Right"
                                icon.source: "icon.svg"
                                width: implicitWidth
                                ThemedItem.tabIndicator: SVS.Theme.TI_Right
                            }
                            TabButton {
                                text: "Flat Fill"
                                icon.source: "icon.svg"
                                width: implicitWidth
                            }
                        }
                        enabled: !disabledCheckBox.checked
                    }
                }

                GroupBox {
                    title: "弹窗"
                    Popup {
                        id: popup
                        anchors.centerIn: Overlay.overlay
                        width: 320
                        height: 200
                    }
                    RowLayout {
                        anchors.fill: parent
                        spacing: 8
                        Button {
                            text: "Show Modal"
                            onClicked: () => {
                                popup.modal = true
                                popup.open()
                            }
                        }
                        Button {
                            text: "Show Modeless"
                            onClicked: () => {
                                popup.modal = false
                                popup.open()
                            }
                        }
                    }
                }

                GroupBox {
                    title: "菜单 & 菜单栏"
                    RowLayout {
                        anchors.fill: parent
                        spacing: 8
                    }
                }

                GroupBox {
                    title: "工具按钮 & 工具栏"
                    RowLayout {
                        anchors.fill: parent
                        spacing: 8
                    }
                }

                GroupBox {
                    title: "标签"
                    RowLayout {
                        anchors.fill: parent
                        spacing: 8
                        Label {
                            text: "Normal Text <a href='#a'>Link Text</a>"
                        }
                    }
                }

                GroupBox {
                    title: "滚动条 & 滚动视图"
                    RowLayout {
                        anchors.fill: parent
                        spacing: 8
                    }
                }

            }
        }
    }

}