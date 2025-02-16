import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SVSCraft.UIComponents as SVS

Window {
    visible: true
    width: 1024
    height: 800

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
                    title: "组合框 & 框架"
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
                            ThemedItem.trackStart: 0.25
                            orientation: Qt.Horizontal
                        }
                        Slider {
                            ThemedItem.trackStart: 0.25
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