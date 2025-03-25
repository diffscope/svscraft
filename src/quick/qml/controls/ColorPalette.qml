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

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Control {
    id: colorPalette
    clip: true
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    padding: 4
    focus: true
    focusPolicy: Qt.TabFocus
    activeFocusOnTab: true
    Accessible.name: qsTr("Color Palette")

    property double hue: 1
    property double saturation: 1
    property double value: 1
    property double alpha: 1
    property int axis: SVS.CA_Hue
    property int dimensions: Qt.Horizontal | Qt.Vertical

    signal modified()

    background: Rectangle {
        color: "transparent"
        border.width: colorPalette.visualFocus ? 2 : 0
        border.color: Theme.navigationColor
    }

    contentItem: Item {

        implicitWidth: (dimensions & Qt.Horizontal) ? 200 : 8
        implicitHeight: (dimensions & Qt.Vertical) ? 200 : 8

        Item {
            anchors.fill: parent
            Image {
                id: background
                anchors.fill: parent
                fillMode: Image.Tile
                horizontalAlignment: Image.AlignLeft
                verticalAlignment: Image.AlignTop
                source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/transparent_tile.svg"
            }
            Rectangle {
                id: alphaLayer
                anchors.fill: parent
                visible: colorPalette.axis === SVS.CA_Alpha && (colorPalette.dimensions === Qt.Horizontal || colorPalette.dimensions === Qt.Vertical)
                transform: Scale { origin.y: alphaLayer.height / 2; yScale: -1 }
                gradient: Gradient {
                    orientation: colorPalette.dimensions === Qt.Horizontal ? Qt.Horizontal : Qt.Vertical
                    GradientStop {
                        position: colorPalette.dimensions === Qt.Horizontal ? 0 : 1
                        color: Qt.hsva(colorPalette.hue, colorPalette.saturation, colorPalette.value, 0)
                    }
                    GradientStop {
                        position: colorPalette.dimensions === Qt.Horizontal ? 1 : 0
                        color: Qt.hsva(colorPalette.hue, colorPalette.saturation, colorPalette.value, 1)
                    }
                }
            }
            Rectangle {
                id: hueBackdrop
                anchors.fill: parent
                visible: colorPalette.axis === SVS.CA_Hue && (colorPalette.dimensions === Qt.Horizontal || colorPalette.dimensions === Qt.Vertical) || colorPalette.axis !== SVS.CA_Hue && ((colorPalette.dimensions & Qt.Horizontal) && (colorPalette.dimensions & Qt.Vertical))
                transform: Scale { origin.y: hueBackdrop.height / 2 ; yScale: -1 }
                gradient: Gradient {
                    orientation: colorPalette.dimensions === Qt.Vertical ? Qt.Vertical : Qt.Horizontal
                    GradientStop {
                        position: 0
                        color: Qt.hsva(0, 1, 1, 1)
                    }
                    GradientStop {
                        position: 1/6
                        color: Qt.hsva(1/6, 1, 1, 1)
                    }
                    GradientStop {
                        position: 2/6
                        color: Qt.hsva(2/6, 1, 1, 1)
                    }
                    GradientStop {
                        position: 3/6
                        color: Qt.hsva(3/6, 1, 1, 1)
                    }
                    GradientStop {
                        position: 4/6
                        color: Qt.hsva(4/6, 1, 1, 1)
                    }
                    GradientStop {
                        position: 5/6
                        color: Qt.hsva(5/6, 1, 1, 1)
                    }
                    GradientStop {
                        position: 1
                        color: Qt.hsva(1, 1, 1, 1)
                    }
                }
            }
            Rectangle {
                id: saturationBackdrop
                anchors.fill: parent
                visible: colorPalette.axis === SVS.CA_Saturation && (colorPalette.dimensions === Qt.Horizontal || colorPalette.dimensions === Qt.Vertical) || colorPalette.axis === SVS.CA_Hue && ((colorPalette.dimensions & Qt.Horizontal) && (colorPalette.dimensions & Qt.Vertical))
                transform: Scale { origin.y: hueBackdrop.height / 2 ; yScale: -1 }
                gradient: Gradient {
                    orientation: colorPalette.dimensions === Qt.Vertical ? Qt.Vertical : Qt.Horizontal
                    GradientStop {
                        position: 0
                        color: Qt.hsva(colorPalette.hue, 0, ((colorPalette.dimensions & Qt.Horizontal) && (colorPalette.dimensions & Qt.Vertical)) ? 1 : colorPalette.value, 1)
                    }
                    GradientStop {
                        position: 1
                        color: Qt.hsva(colorPalette.hue, 1, ((colorPalette.dimensions & Qt.Horizontal) && (colorPalette.dimensions & Qt.Vertical)) ? 1 : colorPalette.value, 1)
                    }
                }
            }
            Rectangle {
                id: valueBackdrop
                anchors.fill: parent
                visible: colorPalette.axis === SVS.CA_Value && (colorPalette.dimensions === Qt.Horizontal || colorPalette.dimensions === Qt.Vertical)
                transform: Scale { origin.y: hueBackdrop.height / 2 ; yScale: -1 }
                gradient: Gradient {
                    orientation: colorPalette.dimensions === Qt.Vertical ? Qt.Vertical : Qt.Horizontal
                    GradientStop {
                        position: 0
                        color: Qt.hsva(colorPalette.hue, ((colorPalette.dimensions & Qt.Horizontal) && (colorPalette.dimensions & Qt.Vertical)) ? 1 : colorPalette.saturation, 0, 1)
                    }
                    GradientStop {
                        position: 1
                        color: Qt.hsva(colorPalette.hue, ((colorPalette.dimensions & Qt.Horizontal) && (colorPalette.dimensions & Qt.Vertical)) ? 1 : colorPalette.saturation, 1, 1)
                    }
                }
            }
            Rectangle {
                id: saturationLayer
                anchors.fill: parent
                visible: colorPalette.axis === SVS.CA_Value && ((colorPalette.dimensions & Qt.Horizontal) && (colorPalette.dimensions & Qt.Vertical))
                transform: Scale { origin.y: saturationLayer.height / 2; yScale: -1 }
                gradient: Gradient {
                    orientation: Qt.Vertical
                    GradientStop {
                        position: 0
                        color: Qt.hsva(0, 0, 1, 1)
                    }
                    GradientStop {
                        position: 1
                        color: Qt.hsva(0, 0, 1, 0)
                    }
                }
            }
            Rectangle {
                id: valueLayer
                anchors.fill: parent
                visible: colorPalette.axis !== SVS.CA_Value && ((colorPalette.dimensions & Qt.Horizontal) && (colorPalette.dimensions & Qt.Vertical))
                transform: Scale { origin.y: valueLayer.height / 2; yScale: -1 }
                gradient: Gradient {
                    orientation: Qt.Vertical
                    GradientStop {
                        position: 0
                        color: Qt.hsva(0, 0, 0, 1)
                    }
                    GradientStop {
                        position: 1
                        color: Qt.hsva(0, 0, 0, 0)
                    }
                }
            }
        }

        Item {
            id: indicator
            readonly property bool is2d: (colorPalette.dimensions & Qt.Horizontal) && (colorPalette.dimensions & Qt.Vertical)
            property double factorX2d: colorPalette.axis !== SVS.CA_SaturationValue ? colorPalette.hue : colorPalette.saturation
            property double factorY2d: colorPalette.axis === SVS.CA_HueSaturation ? colorPalette.saturation : colorPalette.value
            property double factor: colorPalette.axis === SVS.CA_Hue ? colorPalette.hue : colorPalette.axis === SVS.CA_Saturation ? colorPalette.saturation : colorPalette.axis === SVS.CA_Value ? colorPalette.value : colorPalette.alpha
            x: (is2d ? factorX2d : colorPalette.dimensions === Qt.Horizontal ? factor : 0.5) * parent.width
            y: (1 - (is2d ? factorY2d : colorPalette.dimensions === Qt.Vertical ? factor : 0.5)) * parent.height
            Rectangle {
                anchors.centerIn: parent
                readonly property bool is2d: indicator.is2d
                width: 8
                height: 8
                radius: 4
                color: Qt.hsva(colorPalette.hue, colorPalette.axis === SVS.CA_Hue && !is2d || colorPalette.axis === SVS.CA_HueValue && is2d ? 1 : colorPalette.saturation, colorPalette.axis === SVS.CA_Hue && !is2d || colorPalette.axis === SVS.CA_HueSaturation && is2d ? 1 : colorPalette.value, colorPalette.axis === SVS.CA_Alpha ? 0 : 1)
                readonly property color _c: Qt.hsva(color.hsvHue, color.hsvSaturation * (colorPalette.axis === SVS.CA_Alpha ? colorPalette.alpha : 1), color.hsvValue, 1)
                border.color: Qt.rgba(1 - _c.r, 1 - _c.g, 1 - _c.b, 1)
            }
        }

        MouseArea {
            anchors.fill: parent
            anchors.margins: -4
            preventStealing: true
            property bool is2d: (colorPalette.dimensions & Qt.Horizontal) && (colorPalette.dimensions & Qt.Vertical)
            onMouseXChanged: () => {
                let factor = Math.min(Math.max(0, (mouseX - 4) / parent.width), 1)
                if (is2d) {
                    GlobalHelper.setProperty(colorPalette, colorPalette.axis !== SVS.CA_SaturationValue ? "hue" : "saturation", factor)
                    colorPalette.modified()
                } else if (colorPalette.dimensions === Qt.Horizontal) {
                    GlobalHelper.setProperty(colorPalette, colorPalette.axis === SVS.CA_Hue ? "hue" : colorPalette.axis === SVS.CA_Saturation ? "saturation" : colorPalette.axis === SVS.CA_Value ? "value" : "alpha", factor)
                    colorPalette.modified()
                }
            }
            onMouseYChanged: () => {
                let factor = Math.min(Math.max(0, 1 - (mouseY - 4) / parent.height), 1)
                if (is2d) {
                    GlobalHelper.setProperty(colorPalette, colorPalette.axis === SVS.CA_HueSaturation ? "saturation" : "value", factor)
                    colorPalette.modified()
                } else if (colorPalette.dimensions === Qt.Vertical) {
                    GlobalHelper.setProperty(colorPalette, colorPalette.axis === SVS.CA_Hue ? "hue" : colorPalette.axis === SVS.CA_Saturation ? "saturation" : colorPalette.axis === SVS.CA_Value ? "value" : "alpha", factor)
                    colorPalette.modified()
                }
            }
        }

    }
}