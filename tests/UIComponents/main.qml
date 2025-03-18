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
import QtQml.Models
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SVSCraft
import SVSCraft.UIComponents

ApplicationWindow {
    visible: true
    width: 1024
    height: 800
    LayoutMirroring.enabled: mirroringCheckBox.checked
    LayoutMirroring.childrenInherit: true

    menuBar: MenuBar {
        ThemedItem.dividerStroke: SVS.DS_Border
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
        ThemedItem.dividerStroke: SVS.DS_Border
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

    footer: ToolBar {
        Row {
            anchors.fill: parent
            spacing: 4
            Label {
                text: DescriptiveText.statusText
            }
        }
    }

    CommandPalette {
        id: commandPalette
        placeholderText: "placeholder"
        emptyText: "empty"
        recentlyUsedText: "recently used"
        model: ListModel {
            ListElement {
                title: "Test 1"
                subtitle: "test 1"
                description: "Description of Test 1"
                keySequence: "Ctrl+A"
                recentlyUsed: true
            }
            ListElement {
                title: "Test 2"
                subtitle: "test 2"
                description: "Description of Test 2"
                keySequence: "Ctrl+B"
            }
            ListElement { title: "Test 3"; subtitle: "test 3" }
            ListElement { title: "Test 4"; subtitle: "test 4" }
            ListElement { title: "Test 5"; subtitle: "test 5" }
            ListElement { title: "Test 6"; subtitle: "test 6" }
            ListElement { title: "Test 7"; subtitle: "test 7" }
            ListElement { title: "Test 8"; subtitle: "test 8" }
        }
        onCurrentIndexChanged: console.log("command palette: currentIndex =", currentIndex)
        onAccepted: console.log("command palette: committed", currentIndex)
    }

    Pane {
        id: pane
        anchors.fill: parent
        ScrollView {
            anchors.fill: parent
            ColumnLayout {
                id: layout
                width: pane.width - 8
                anchors.top: parent.top
                anchors.margins: 8
                spacing: 8

                RowLayout {
                    spacing: 8
                    CheckBox {
                        id: disabledCheckBox
                        text: "Disabled"
                    }
                    CheckBox {
                        id: mirroringCheckBox
                        text: "Mirroring"
                    }
                }

                GroupBox {
                    title: "按钮"
                    ColumnLayout {
                        id: buttonRowLayout
                        spacing: 8
                        enabled: !disabledCheckBox.checked
                        RowLayout {
                            spacing: 8
                            Button {
                                ThemedItem.controlType: SVS.CT_Normal
                                text: "Normal"
                                icon.source: "icon.svg"
                            }
                            Button {
                                ThemedItem.controlType: SVS.CT_Accent
                                text: "Accent"
                                icon.source: "icon.svg"
                            }
                            Button {
                                ThemedItem.controlType: SVS.CT_Error
                                text: "Error"
                                icon.source: "icon.svg"
                            }
                            Button {
                                ThemedItem.controlType: SVS.CT_Normal
                                text: "Normal Flat"
                                flat: true
                                icon.source: "icon.svg"
                            }
                            Button {
                                ThemedItem.controlType: SVS.CT_Accent
                                text: "Accent Flat"
                                flat: true
                                icon.source: "icon.svg"
                            }
                            Button {
                                ThemedItem.controlType: SVS.CT_Error
                                text: "Error Flat"
                                flat: true
                                icon.source: "icon.svg"
                            }
                        }
                        RowLayout {
                            spacing: 8
                            Button {
                                text: "Checkable Normal/Accent"
                                checkable: true
                                icon.source: "icon.svg"
                            }
                            Button {
                                text: "Checkable Error"
                                checkable: true
                                ThemedItem.controlType: SVS.CT_Error
                                icon.source: "icon.svg"
                            }
                            Button {
                                text: "Checkable Flat Normal"
                                checkable: true
                                flat: true
                                icon.source: "icon.svg"
                            }
                            Button {
                                text: "Checkable Flat Accent"
                                checkable: true
                                ThemedItem.controlType: SVS.CT_Accent
                                flat: true
                                icon.source: "icon.svg"
                            }
                            Button {
                                text: "Checkable Flat Error"
                                checkable: true
                                ThemedItem.controlType: SVS.CT_Error
                                flat: true
                                icon.source: "icon.svg"
                            }
                        }
                        RowLayout {
                            spacing: 8
                            Button {
                                icon.source: "icon.svg"
                                DescriptiveText.activated: hovered
                                DescriptiveText.toolTip: "Icon only"
                            }
                            Button {
                                text: "Uncolorized Icon"
                                icon.source: "icon_alt.svg"
                                icon.color: "transparent"
                            }
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
                                duration: Theme.visualEffectAnimationDuration
                                easing.type: Easing.OutCubic
                            }
                        }
                        Frame {
                            anchors.fill: parent
                            ThemedItem.backgroundLevel: SVS.BL_Secondary
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
                            from: -1
                            to: 2
                            id: slider1
                            orientation: Qt.Horizontal
                        }
                        Slider {
                            from: -1
                            to: 2
                            orientation: Qt.Vertical
                        }
                        Dial {
                            from: -1
                            to: 2
                        }
                        RangeSlider {
                            orientation: Qt.Horizontal
                        }
                        RangeSlider {
                            orientation: Qt.Vertical
                        }
                        enabled: !disabledCheckBox.checked
                    }
                }

                GroupBox {
                    title: "文本框"
                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8
                        TextField {
                            placeholderText: "placeholder"
                        }
                        TextField {
                            placeholderText: "מציין מיקום"
                        }
                        TextArea {
                            placeholderText: "placeholder"
                        }
                        enabled: !disabledCheckBox.checked
                    }
                }

                GroupBox {
                    title: "旋钮"
                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8
                        SpinBox {
                            to: 2147483647
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
                                ThemedItem.tabIndicator: SVS.TI_Top
                            }
                            TabButton {
                                text: "Flat Bottom"
                                icon.source: "icon.svg"
                                width: implicitWidth
                                ThemedItem.tabIndicator: SVS.TI_Bottom
                            }
                            TabButton {
                                text: "Flat Left"
                                icon.source: "icon.svg"
                                width: implicitWidth
                                ThemedItem.tabIndicator: SVS.TI_Left
                            }
                            TabButton {
                                text: "Flat Right"
                                icon.source: "icon.svg"
                                width: implicitWidth
                                ThemedItem.tabIndicator: SVS.TI_Right
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
                }

                GroupBox {
                    title: "工具按钮 & 工具栏"
                }

                GroupBox {
                    title: "标签"
                    Label {
                        text: "Normal Text <a href='#a'>Link Text</a>"
                    }
                }

                GroupBox {
                    title: "工具提示 & 状态提示"
                    Button {
                        text: "Hover me"
                        DescriptiveText.activated: hovered
                        DescriptiveText.toolTip: "Tool tip"
                        DescriptiveText.statusTip: "Status tip"
                    }
                }

                GroupBox {
                    title: "分割视图"
                    SplitView {
                        implicitWidth: 400
                        implicitHeight: 200
                        Rectangle {
                            color: Theme.backgroundSecondaryColor
                        }
                        Rectangle {
                            color: Theme.backgroundTertiaryColor
                        }
                        enabled: !disabledCheckBox.checked
                    }
                }

                GroupBox {
                    title: "滚动条 & 滚动视图"
                }

                GroupBox {
                    title: "命令面板"
                    Button {
                        text: "显示"
                        onClicked: commandPalette.open()
                    }
                }

                GroupBox {
                    title: "表单"
                    Layout.fillWidth: true
                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8
                        FormGroup {
                            label: "Form Group"
                            rowItem: Switch {}
                            columnItem: TextField {}
                            Layout.fillWidth: true
                        }
                        FormGroup {
                            label: "Form Group w/o Row Item"
                            columnItem: TextField {}
                            Layout.fillWidth: true
                        }
                        FormGroup {
                            label: "Form Group w/o Column Item"
                            rowItem: Switch {}
                            Layout.fillWidth: true
                        }
                        FormGroup {
                            columnItem: TextField {
                                placeholderText: "Form Group w/o Label"
                            }
                            Layout.fillWidth: true
                        }
                        FormGroup {
                            label: Label {
                                color: "red"
                                text: "Form Group with Custom Label Item"
                            }
                            rowItem: Switch {}
                            columnItem: TextField {}
                            Layout.fillWidth: true
                        }
                    }
                }

                GroupBox {
                    title: "卡片"
                    RowLayout {
                        anchors.fill: parent
                        spacing: 8
                        Card {
                            image: "qrc:/qt/qml/SVSCraft/Test/UIComponents/crs.png"
                            title: "CRS"
                            subtitle: "CrSjimo"
                            toolBar: Button {
                                flat: true
                                icon.source: "icon.svg"
                            }
                        }

                        ColumnLayout {
                            width: 300
                            spacing: 0
                            Card {
                                atTop: true
                                atBottom: false
                                image: Rectangle {
                                    color: "red"
                                }
                                title: "Custom Image Item"
                                subtitle: "Custom Image Item"
                            }
                            Card {
                                atTop: false
                                atBottom: false
                                image: "qrc:/qt/qml/SVSCraft/Test/UIComponents/crs.png"
                                title: Text {
                                    text: "Custom Title Item"
                                    color: "red"
                                }
                                subtitle: TextField {
                                    text: "Custom Subtitle Item"
                                }
                                toolBar: Button {
                                    flat: true
                                    icon.source: "icon.svg"
                                }
                            }
                            Card {
                                atTop: false
                                atBottom: true
                                image: "qrc:/qt/qml/SVSCraft/Test/UIComponents/crs.png"
                                title: "Bottom"
                                subtitle: "Bottom"
                                toolBar: Button {
                                    flat: true
                                    icon.source: "icon.svg"
                                }
                            }
                        }
                    }
                }

                GroupBox {
                    title: "注释"
                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8
                        RowLayout {
                            spacing: 8
                            Annotation {
                                ThemedItem.controlType: SVS.CT_Normal
                                label: "Normal"
                            }
                            Annotation {
                                ThemedItem.controlType: SVS.CT_Accent
                                label: "Accent"
                            }
                            Annotation {
                                ThemedItem.controlType: SVS.CT_Warning
                                label: "Warning"
                            }
                            Annotation {
                                ThemedItem.controlType: SVS.CT_Error
                                label: "Error"
                            }
                        }
                        RowLayout {
                            Annotation {
                                id: closableAnnotation
                                ThemedItem.controlType: SVS.CT_Normal
                                label: "Closable"
                                closable: true
                            }
                            Annotation {
                                ThemedItem.controlType: SVS.CT_Normal
                                label: "Action"
                                action: Action {
                                    text: "Action"
                                    icon.source: "qrc:/qt/qml/SVSCraft/Test/UIComponents/icon.svg"
                                    onTriggered: console.log("Action")
                                }
                            }
                            Annotation {
                                id: closableWithActionAnnotation
                                ThemedItem.controlType: SVS.CT_Normal
                                label: "Closable with Action"
                                closable: true
                                action: Action {
                                    text: "Action"
                                    icon.source: "qrc:/qt/qml/SVSCraft/Test/UIComponents/icon.svg"
                                    onTriggered: console.log("Action")
                                }
                            }
                            Annotation {
                                ThemedItem.controlType: SVS.CT_Normal
                                label: Label {
                                    text: "Custom Label <a href='#b'>Link</a>"
                                }
                            }
                            Button {
                                text: "Reset Visibility"
                                onClicked: closableAnnotation.visible = closableWithActionAnnotation.visible = true
                            }
                        }
                    }
                }

                GroupBox {
                    title: "注释弹窗"
                    RowLayout {
                        anchors.fill: parent
                        spacing: 8
                        Button {
                            text: "Normal"
                            property AnnotationPopup annotationPopup: AnnotationPopup {
                                anchors.centerIn: Overlay.overlay
                                ThemedItem.controlType: SVS.CT_Normal
                                title: "Title"
                                content: "Content"
                                closable: true
                            }
                            onClicked: annotationPopup.open()
                        }
                        Button {
                            text: "Accent"
                            property AnnotationPopup annotationPopup: AnnotationPopup {
                                anchors.centerIn: Overlay.overlay
                                ThemedItem.controlType: SVS.CT_Accent
                                title: "Title"
                                content: "Content"
                                closable: true
                            }
                            onClicked: annotationPopup.open()
                        }
                        Button {
                            text: "Warning"
                            property AnnotationPopup annotationPopup: AnnotationPopup {
                                anchors.centerIn: Overlay.overlay
                                ThemedItem.controlType: SVS.CT_Warning
                                title: "Title"
                                content: "Content"
                                closable: true
                            }
                            onClicked: annotationPopup.open()
                        }
                        Button {
                            text: "Error"
                            property AnnotationPopup annotationPopup: AnnotationPopup {
                                anchors.centerIn: Overlay.overlay
                                ThemedItem.controlType: SVS.CT_Error
                                title: "Title"
                                content: "Content"
                                closable: true
                            }
                            onClicked: annotationPopup.open()
                        }
                        Button {
                            text: "Auto Close"
                            property AnnotationPopup annotationPopup: AnnotationPopup {
                                anchors.centerIn: Overlay.overlay
                                title: "Title"
                                content: "Content"
                                closable: true
                                timeout: 5000
                            }
                            onClicked: annotationPopup.open()
                        }
                    }
                }

                Item { height: 64 }

            }
        }
    }

}