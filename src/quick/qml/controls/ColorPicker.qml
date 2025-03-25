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
import QtQuick.Layouts
import QtQuick.Controls

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Pane {
    id: colorPicker
    property color color: Theme.accentColor
    property color currentColor: "white"
    property int axis: SVS.CA_Hue
    property int flags: 16383
    component ColorPickerColorPalette: ColorPalette {
        hue: Math.max(colorPicker.color.hsvHue, colorPicker.color.hslHue, 0)
        saturation: colorPicker.color.hsvSaturation
        value: colorPicker.color.hsvValue
        alpha: colorPicker.color.a
        onModified: GlobalHelper.setProperty(colorPicker, "color", Qt.hsva(hue, saturation, value, alpha))
    }
    GridLayout {
        rowSpacing: 8
        columnSpacing: 8
        columns: colorSpecTabBar.model.length > 0 ? 2 : 1
        ColumnLayout {
            spacing: 4
            RowLayout {
                spacing: 4
                ColorPickerColorPalette {
                    dimensions: Qt.Horizontal | Qt.Vertical
                    axis: colorPicker.axis
                }
                ColorPickerColorPalette {
                    dimensions: Qt.Vertical
                    axis: colorPicker.axis
                }
            }
            ColorPickerColorPalette {
                dimensions: Qt.Horizontal
                axis: SVS.CA_Alpha
                visible: colorPicker.flags & SVS.CM_Alpha
            }
        }
        ColumnLayout {
            RowLayout {
                Layout.fillWidth: true
                spacing: 4
                ToolButton {
                    text: colorPicker.axis === SVS.CA_Hue ? "H" : colorPicker.axis === SVS.CA_Saturation ? "S" : colorPicker.axis === SVS.CA_Value ? "V" : undefined
                    visible: colorPicker.flags & SVS.CM_AxisChangeable
                    DescriptiveText.activated: hovered
                    DescriptiveText.toolTip: qsTr("Switch palette")
                    onClicked: () => {
                        GlobalHelper.setProperty(colorPicker, "axis", colorPicker.axis === SVS.CA_Hue ? SVS.CA_Saturation : colorPicker.axis === SVS.CA_Saturation ? SVS.CA_Value : colorPicker.axis === SVS.CA_Value ? SVS.CA_Hue : colorPicker.axis)
                    }
                }
                ToolButton {
                    icon.source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/Color16Filled.svg"
                    visible: (colorPicker.flags & SVS.CM_NativeColorDialog) && GlobalHelper.hasNativeColorChooser()
                    DescriptiveText.activated: hovered
                    DescriptiveText.toolTip: qsTr("Show native color dialog")
                    onClicked: () => {
                        let c = GlobalHelper.nativeChooseColor(colorPicker.color, Window.window, colorPicker.flags & SVS.CM_Alpha)
                        if (!c.valid)
                            return
                        GlobalHelper.setProperty(colorPicker, "color", c)
                    }
                }
                ToolButton {
                    icon.source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/Eyedropper20Filled.svg"
                    visible: colorPicker.flags & SVS.CM_Eyedropper
                    DescriptiveText.activated: hovered
                    DescriptiveText.toolTip: qsTr("Pick a color on the screen")
                    onClicked: () => {
                        let c = GlobalHelper.pickColor(Window.window)
                        if (!c.valid)
                            return
                        GlobalHelper.setProperty(colorPicker, "color", c)
                    }
                }
                Item {
                    id: colorPreviewArea
                    Layout.fillWidth: true
                    height: 24
                    component ColorPreview: Item {
                        property color color: "white"
                        readonly property bool hovered: hoverHandler.hovered
                        Image {
                            anchors.fill: parent
                            fillMode: Image.Tile
                            horizontalAlignment: Image.AlignLeft
                            verticalAlignment: Image.AlignTop
                            source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/transparent_tile.svg"
                        }
                        Rectangle {
                            anchors.fill: parent
                            color: parent.color
                        }
                        HoverHandler {
                            id: hoverHandler
                        }
                    }
                    ColorPreview {
                        width: parent.width / ((colorPicker.flags & SVS.CM_ShowCurrentColor) ? 2 : 1)
                        height: parent.height
                        color: colorPicker.color
                        DescriptiveText.activated: hovered
                        DescriptiveText.toolTip: qsTr("New color")
                    }
                    Button {
                        visible: colorPicker.flags & SVS.CM_ShowCurrentColor
                        x: parent.width / 2
                        width: parent.width / 2
                        height: parent.height
                        contentItem: null
                        background: ColorPreview {
                            color: colorPicker.currentColor
                        }
                        DescriptiveText.activated: hovered
                        DescriptiveText.toolTip: qsTr("Current color (click to set)")
                        onClicked: GlobalHelper.setProperty(colorPicker, "color", colorPicker.currentColor)
                    }

                }
            }
            TabBar {
                id: colorSpecTabBar
                Layout.fillWidth: true
                readonly property list<int> model: ((flags) => {
                    let a = []
                    if (flags & SVS.CM_ColorSpecRgb)
                        a.push(SVS.CM_ColorSpecRgb)
                    if (flags & SVS.CM_ColorSpecHsv)
                        a.push(SVS.CM_ColorSpecHsv)
                    if (flags & SVS.CM_ColorSpecHsl)
                        a.push(SVS.CM_ColorSpecHsl)
                    if (flags & SVS.CM_ColorSpecCmyk)
                        a.push(SVS.CM_ColorSpecCmyk)
                    return a
                })(colorPicker.flags)
                readonly property int currentFlag: model[currentIndex] ?? 0
                visible: model.length > 1
                Repeater {
                    model: colorSpecTabBar.model
                    TabButton {
                        required property int modelData
                        text: modelData === SVS.CM_ColorSpecRgb ? "RGB" : modelData === SVS.CM_ColorSpecHsv ? "HSV" : modelData === SVS.CM_ColorSpecHsl ? "HSL" : "CMYK"
                    }
                }
            }
            Frame {
                id: frame
                function updateRgb() {
                    GlobalHelper.setProperty(colorPicker, "color", Qt.rgba(rSpinBox.value / 255, gSpinBox.value / 255, bSpinBox.value / 255, aSpinBoxRgb.value / 100))
                }
                function updateHsv() {
                    GlobalHelper.setProperty(colorPicker, "color", Qt.hsva(hSpinBoxHsv.value / 360, sSpinBoxHsv.value / 100, vSpinBox.value / 100, aSpinBoxHsv.value / 100))
                }
                function updateHsl() {
                    GlobalHelper.setProperty(colorPicker, "color", Qt.hsla(hSpinBoxHsl.value / 360, sSpinBoxHsl.value / 100, lSpinBox.value / 100, aSpinBoxHsl.value / 100))
                }
                function updateCmyk() {
                    GlobalHelper.setProperty(colorPicker, "color", GlobalHelper.setCmyk(cSpinBox.value / 100, mSpinBox.value / 100, ySpinBox.value / 100, kSpinBox.value / 100, aSpinBoxCmyk.value / 100))
                }
                Layout.fillWidth: true
                Layout.fillHeight: true
                padding: 8
                visible: colorSpecTabBar.model.length > 0
                StackLayout {
                    currentIndex: colorSpecTabBar.currentFlag === SVS.CM_ColorSpecRgb ? 0 : colorSpecTabBar.currentFlag === SVS.CM_ColorSpecHsv ? 1 : colorSpecTabBar.currentFlag === SVS.CM_ColorSpecHsl ? 2 : 3
                    GridLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        columns: 2
                        Label {
                            text: "R"
                        }
                        SpinBox {
                            id: rSpinBox
                            Layout.fillWidth: true
                            Accessible.name: "R"
                            from: 0
                            to: 255
                            value: Math.round(colorPicker.color.r * 255)
                            onValueModified: frame.updateRgb()
                        }
                        Label {
                            text: "G"
                        }
                        SpinBox {
                            id: gSpinBox
                            Layout.fillWidth: true
                            Accessible.name: "G"
                            from: 0
                            to: 255
                            value: Math.round(colorPicker.color.g * 255)
                            onValueModified: frame.updateRgb()
                        }
                        Label {
                            text: "B"
                        }
                        SpinBox {
                            id: bSpinBox
                            Layout.fillWidth: true
                            Accessible.name: "B"
                            from: 0
                            to: 255
                            value: Math.round(colorPicker.color.b * 255)
                            onValueModified: frame.updateRgb()
                        }
                        Label {
                            text: "A"
                            visible: colorPicker.flags & SVS.CM_Alpha
                        }
                        SpinBox {
                            id: aSpinBoxRgb
                            visible: colorPicker.flags & SVS.CM_Alpha
                            Layout.fillWidth: true
                            Accessible.name: "A"
                            from: 0
                            to: 100
                            value: Math.round(colorPicker.color.a * 100)
                            onValueModified: frame.updateRgb()
                        }
                    }
                    GridLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        columns: 2
                        Label {
                            text: "H"
                        }
                        SpinBox {
                            id: hSpinBoxHsv
                            Layout.fillWidth: true
                            Accessible.name: "H"
                            from: 0
                            to: 360
                            value: Math.round(Math.max(colorPicker.color.hsvHue, colorPicker.color.hslHue, 0) * 360)
                            onValueModified: frame.updateHsv()
                        }
                        Label {
                            text: "S"
                        }
                        SpinBox {
                            id: sSpinBoxHsv
                            Layout.fillWidth: true
                            Accessible.name: "S"
                            from: 0
                            to: 100
                            value: Math.round(colorPicker.color.hsvSaturation * 100)
                            onValueModified: frame.updateHsv()
                        }
                        Label {
                            text: "V"
                        }
                        SpinBox {
                            id: vSpinBox
                            Layout.fillWidth: true
                            Accessible.name: "V"
                            from: 0
                            to: 100
                            value: Math.round(colorPicker.color.hsvValue * 100)
                            onValueModified: frame.updateHsv()
                        }
                        Label {
                            text: "A"
                            visible: colorPicker.flags & SVS.CM_Alpha
                        }
                        SpinBox {
                            id: aSpinBoxHsv
                            visible: colorPicker.flags & SVS.CM_Alpha
                            Layout.fillWidth: true
                            Accessible.name: "A"
                            from: 0
                            to: 100
                            value: Math.round(colorPicker.color.a * 100)
                            onValueModified: frame.updateHsv()
                        }
                    }
                    GridLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        columns: 2
                        Label {
                            text: "H"
                        }
                        SpinBox {
                            id: hSpinBoxHsl
                            Layout.fillWidth: true
                            Accessible.name: "H"
                            from: 0
                            to: 360
                            value: Math.round(Math.max(colorPicker.color.hsvHue, colorPicker.color.hslHue, 0) * 360)
                            onValueModified: frame.updateHsl()
                        }
                        Label {
                            text: "S"
                        }
                        SpinBox {
                            id: sSpinBoxHsl
                            Layout.fillWidth: true
                            Accessible.name: "S"
                            from: 0
                            to: 100
                            value: Math.round(colorPicker.color.hslSaturation * 100)
                            onValueModified: frame.updateHsl()
                        }
                        Label {
                            text: "L"
                        }
                        SpinBox {
                            id: lSpinBox
                            Layout.fillWidth: true
                            Accessible.name: "L"
                            from: 0
                            to: 100
                            value: Math.round(colorPicker.color.hslLightness * 100)
                            onValueModified: frame.updateHsl()
                        }
                        Label {
                            text: "A"
                            visible: colorPicker.flags & SVS.CM_Alpha
                        }
                        SpinBox {
                            id: aSpinBoxHsl
                            visible: colorPicker.flags & SVS.CM_Alpha
                            Layout.fillWidth: true
                            Accessible.name: "A"
                            from: 0
                            to: 100
                            value: Math.round(colorPicker.color.a * 100)
                            onValueModified: frame.updateHsl()
                        }
                    }
                    GridLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        columns: 4
                        Label {
                            text: "C"
                        }
                        SpinBox {
                            id: cSpinBox
                            Layout.fillWidth: true
                            Accessible.name: "C"
                            Layout.columnSpan: 3
                            from: 0
                            to: 100
                            value: Math.round(GlobalHelper.getCmyk(colorPicker.color, 0) * 100)
                            onValueModified: frame.updateCmyk()
                        }
                        Label {
                            text: "M"
                        }
                        SpinBox {
                            id: mSpinBox
                            Layout.fillWidth: true
                            Accessible.name: "M"
                            Layout.columnSpan: 3
                            from: 0
                            to: 100
                            value: Math.round(GlobalHelper.getCmyk(colorPicker.color, 1) * 100)
                            onValueModified: frame.updateCmyk()
                        }
                        Label {
                            text: "Y"
                        }
                        SpinBox {
                            id: ySpinBox
                            Layout.fillWidth: true
                            Accessible.name: "Y"
                            Layout.columnSpan: 3
                            from: 0
                            to: 100
                            value: Math.round(GlobalHelper.getCmyk(colorPicker.color, 2) * 100)
                            onValueModified: frame.updateCmyk()
                        }
                        Label {
                            text: "K"
                        }
                        SpinBox {
                            id: kSpinBox
                            Layout.fillWidth: true
                            Accessible.name: "K"
                            Layout.horizontalStretchFactor: 1
                            Layout.columnSpan: (colorPicker.flags & SVS.CM_Alpha) ? 1 : 3
                            from: 0
                            to: 100
                            value: Math.round(GlobalHelper.getCmyk(colorPicker.color, 3) * 100)
                            onValueModified: frame.updateCmyk()
                        }
                        Label {
                            text: "A"
                            visible: colorPicker.flags & SVS.CM_Alpha
                        }
                        SpinBox {
                            id: aSpinBoxCmyk
                            visible: colorPicker.flags & SVS.CM_Alpha
                            Accessible.name: "A"
                            Layout.fillWidth: true
                            Layout.horizontalStretchFactor: 1
                            from: 0
                            to: 100
                            value: Math.round(colorPicker.color.a * 100)
                            onValueModified: frame.updateCmyk()
                        }
                    }
                }
            }
            RowLayout {
                visible: colorPicker.flags & SVS.CM_Hex
                spacing: 4
                Label {
                    text: qsTr("Hex")
                }
                TextField {
                    id: hexEditor
                    Layout.fillWidth: true
                    Accessible.name: qsTr("Hex")
                    Text {
                        id: hashtag
                        x: 8
                        anchors.verticalCenter: parent.verticalCenter
                        text: "#"
                        color: parent.color
                    }
                    leftPadding: hashtag.width + 8
                    Connections {
                        target: colorPicker
                        enabled: !hexEditor.activeFocus
                        function onColorChanged() {
                            hexEditor.text = colorPicker.color.toString().slice(1)
                        }
                    }
                    text: { text = colorPicker.color.toString().slice(1) }
                    font.capitalization: Font.AllUppercase
                    validator: RegularExpressionValidator {
                        regularExpression: (colorPicker.flags & SVS.CM_Alpha) ? /^[0-9A-Za-z]{0,8}$/ : /^[0-9A-Za-z]{0,6}$/
                    }
                    onTextEdited: () => {
                        if (/^[0-9A-Za-z]{3}|[0-9A-Za-z]{6}$/.test(text) || (colorPicker.flags & SVS.CM_Alpha) && /^[0-9A-Za-z]{8}$/.test(text) ) {
                            GlobalHelper.setProperty(colorPicker, "color", `#${text}`)
                        }
                    }
                    onEditingFinished: () => {
                        hexEditor.text = colorPicker.color.toString().slice(1)
                    }
                }
            }

        }
    }
}