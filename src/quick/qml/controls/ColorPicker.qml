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
import QtQuick.Dialogs

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Pane {
    id: colorPicker
    property color color: Theme.accentColor
    property color currentColor: "white"
    property int axis: SVS.CA_Hue
    readonly property ColorDialog colorDialog: ColorDialog {
        selectedColor: colorPicker.color
        onAccepted: GlobalHelper.setProperty(colorPicker, "color", selectedColor)
    }
    component ColorPickerColorPalette: ColorPalette {
        hue: colorPicker.color.hsvHue
        saturation: colorPicker.color.hsvSaturation
        value: colorPicker.color.hsvValue
        alpha: colorPicker.color.a
        onModified: GlobalHelper.setProperty(colorPicker, "color", Qt.hsva(hue, saturation, value, alpha))
    }
    RowLayout {
        spacing: 8
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
            }
        }
        ColumnLayout {
            Layout.preferredWidth: 160
            RowLayout {
                Layout.fillWidth: true
                spacing: 4
                ToolButton {
                    text: colorPicker.axis === SVS.CA_Hue ? "H" : colorPicker.axis === SVS.CA_Saturation ? "S" : colorPicker.axis === SVS.CA_Value ? "V" : undefined
                    DescriptiveText.activated: hovered
                    DescriptiveText.toolTip: qsTr("Switch palette")
                    onClicked: () => {
                        GlobalHelper.setProperty(colorPicker, "axis", colorPicker.axis === SVS.CA_Hue ? SVS.CA_Saturation : colorPicker.axis === SVS.CA_Saturation ? SVS.CA_Value : colorPicker.axis === SVS.CA_Value ? SVS.CA_Hue : colorPicker.axis)
                    }
                }
                ToolButton {
                    icon.source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/Color16Filled.svg"
                    DescriptiveText.activated: hovered
                    DescriptiveText.toolTip: qsTr("Show native color dialog")
                    onClicked: colorPicker.colorDialog.open()
                }
                ToolButton {
                    icon.source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/Eyedropper20Filled.svg"
                    DescriptiveText.activated: hovered
                    DescriptiveText.toolTip: qsTr("Pick a color on the screen")
                }
                Item {
                    id: colorPreviewArea
                    Layout.fillWidth: true
                    height: 24
                    component ColorPreview: Item {
                        property color color: "white"
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
                    }
                    ColorPreview {
                        width: parent.width / 2
                        height: parent.height
                        color: colorPicker.color
                    }
                    ColorPreview {
                        x: parent.width / 2
                        width: parent.width / 2
                        height: parent.height
                        color: colorPicker.currentColor
                    }
                }
            }
            TabBar {
                id: colorSpecTabBar
                TabButton {
                    text: "RGB"
                }
                TabButton {
                    text: "HSV"
                }
                TabButton {
                    text: "HSL"
                }
                TabButton {
                    text: "CMYK"
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
                StackLayout {
                    anchors.fill: parent
                    currentIndex: colorSpecTabBar.currentIndex
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
                            from: 0
                            to: 255
                            value: colorPicker.color.r * 255
                            onValueModified: frame.updateRgb()
                        }
                        Label {
                            text: "G"
                        }
                        SpinBox {
                            id: gSpinBox
                            Layout.fillWidth: true
                            from: 0
                            to: 255
                            value: colorPicker.color.g * 255
                            onValueModified: frame.updateRgb()
                        }
                        Label {
                            text: "B"
                        }
                        SpinBox {
                            id: bSpinBox
                            Layout.fillWidth: true
                            from: 0
                            to: 255
                            value: colorPicker.color.b * 255
                            onValueModified: frame.updateRgb()
                        }
                        Label {
                            text: "A"
                        }
                        SpinBox {
                            id: aSpinBoxRgb
                            Layout.fillWidth: true
                            from: 0
                            to: 100
                            value: colorPicker.color.a * 100
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
                            from: 0
                            to: 360
                            value: colorPicker.color.hsvHue * 360
                            onValueModified: frame.updateHsv()
                        }
                        Label {
                            text: "S"
                        }
                        SpinBox {
                            id: sSpinBoxHsv
                            Layout.fillWidth: true
                            from: 0
                            to: 100
                            value: colorPicker.color.hsvSaturation * 100
                            onValueModified: frame.updateHsv()
                        }
                        Label {
                            text: "V"
                        }
                        SpinBox {
                            id: vSpinBox
                            Layout.fillWidth: true
                            from: 0
                            to: 100
                            value: colorPicker.color.hsvValue * 100
                            onValueModified: frame.updateHsv()
                        }
                        Label {
                            text: "A"
                        }
                        SpinBox {
                            id: aSpinBoxHsv
                            Layout.fillWidth: true
                            from: 0
                            to: 100
                            value: colorPicker.color.a * 100
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
                            from: 0
                            to: 360
                            value: colorPicker.color.hslHue * 360
                            onValueModified: frame.updateHsl()
                        }
                        Label {
                            text: "S"
                        }
                        SpinBox {
                            id: sSpinBoxHsl
                            Layout.fillWidth: true
                            from: 0
                            to: 100
                            value: colorPicker.color.hslSaturation * 100
                            onValueModified: frame.updateHsl()
                        }
                        Label {
                            text: "L"
                        }
                        SpinBox {
                            id: lSpinBox
                            Layout.fillWidth: true
                            from: 0
                            to: 100
                            value: colorPicker.color.hslLightness * 100
                            onValueModified: frame.updateHsl()
                        }
                        Label {
                            text: "A"
                        }
                        SpinBox {
                            id: aSpinBoxHsl
                            Layout.fillWidth: true
                            from: 0
                            to: 100
                            value: colorPicker.color.a * 100
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
                            Layout.columnSpan: 3
                            from: 0
                            to: 100
                            value: GlobalHelper.getCmyk(colorPicker.color, 0) * 100
                            onValueModified: frame.updateCmyk()
                        }
                        Label {
                            text: "M"
                        }
                        SpinBox {
                            id: mSpinBox
                            Layout.fillWidth: true
                            Layout.columnSpan: 3
                            from: 0
                            to: 100
                            value: GlobalHelper.getCmyk(colorPicker.color, 1) * 100
                            onValueModified: frame.updateCmyk()
                        }
                        Label {
                            text: "Y"
                        }
                        SpinBox {
                            id: ySpinBox
                            Layout.fillWidth: true
                            Layout.columnSpan: 3
                            from: 0
                            to: 100
                            value: GlobalHelper.getCmyk(colorPicker.color, 2) * 100
                            onValueModified: frame.updateCmyk()
                        }
                        Label {
                            text: "K"
                        }
                        SpinBox {
                            id: kSpinBox
                            Layout.fillWidth: true
                            Layout.horizontalStretchFactor: 1
                            from: 0
                            to: 100
                            value: GlobalHelper.getCmyk(colorPicker.color, 3) * 100
                            onValueModified: frame.updateCmyk()
                        }
                        Label {
                            text: "A"
                        }
                        SpinBox {
                            id: aSpinBoxCmyk
                            Layout.fillWidth: true
                            Layout.horizontalStretchFactor: 1
                            from: 0
                            to: 100
                            value: colorPicker.color.a * 100
                            onValueModified: frame.updateCmyk()
                        }
                    }
                }
            }
            RowLayout {
                spacing: 4
                Label {
                    text: "Hex"
                }
                TextField {
                    Layout.fillWidth: true
                    Text {
                        id: hashtag
                        x: 8
                        anchors.verticalCenter: parent.verticalCenter
                        text: "#"
                        color: parent.color
                    }
                    leftPadding: hashtag.width + 8
                }
            }

        }
    }
}