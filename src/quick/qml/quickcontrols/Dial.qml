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
import QtQuick.Shapes
import QtQuick.Templates as T

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

T.Dial {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    hoverEnabled: true
    inputMode: T.Dial.Horizontal

    background: Item {
        implicitWidth: 24
        implicitHeight: 24

        property double _animatedAngle: control.angle
        property bool _doubleClicked: false
        Connections {
            target: control
            function onAngleChanged() {
                control.background._doubleClicked = false
            }
        }
        Behavior on _animatedAngle {
            enabled: control.background._doubleClicked
            NumberAnimation {
                duration: control.Theme.visualEffectAnimationDuration
                easing.type: Easing.OutCubic
                onRunningChanged: () => {
                    if (!running) {
                        control.background._doubleClicked = false
                    }
                }
            }
        }

        Rectangle {
            anchors.centerIn: parent
            width: Math.min(parent.width, parent.height)
            height: Math.min(parent.width, parent.height)
            color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.buttonColor) :
                   control.pressed ? Theme.foregroundPressedColorChange.apply(Theme.buttonColor) :
                   control.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.buttonColor) :
                   Theme.buttonColor
            border.color: control.visualFocus ? Theme.navigationColor : Theme.borderColor
            border.width: control.visualFocus ? 2 : 1
            radius: width / 2
            Behavior on color {
                ColorAnimation {
                    duration: Theme.colorAnimationDuration
                    easing.type: Easing.OutCubic
                }
            }
        }

        Shape {
            id: shape
            anchors.fill: parent
            ShapePath {
                strokeWidth: 2
                strokeColor: control.Theme.accentColor
                fillColor: "transparent"
                capStyle: ShapePath.RoundCap
                PathAngleArc {
                    centerX: shape.width / 2
                    centerY: shape.height / 2
                    radiusX: shape.width / 2 - 1
                    radiusY: shape.height / 2 - 1
                    startAngle: control.startAngle + (0 - control.from) / (control.to - control.from) * (control.endAngle - control.startAngle) - 90
                    sweepAngle: -(startAngle + 90 - control.background._animatedAngle)
                }
            }
        }
    }

    handle: Rectangle {
        id: handleItem
        x: control.background.x + control.background.width / 2 - width / 2
        y: control.background.y + control.background.height / 2 - height / 2
        width: 1
        height: 4
        color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
               control.pressed ? Theme.foregroundPressedColorChange.apply(Theme.foregroundPrimaryColor) :
               control.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundPrimaryColor) :
               Theme.foregroundPrimaryColor
        transform: [
            Translate {
                y: -control.background.width * 0.4 + handleItem.height
            },
            Rotation {
                angle: control.background._animatedAngle
                origin.x: handleItem.width / 2
                origin.y: handleItem.height / 2
            }
        ]
    }

    HoverHandler {
        cursorShape: control.inputMode === T.Dial.Horizontal ? Qt.SizeHorCursor : control.inputMode === T.Dial.Vertical ? Qt.SizeVerCursor : undefined
    }

    TapHandler {
        onDoubleTapped: () => {
            if (!Theme.doubleClickResetEnabled)
                return
            if (from > 0 && to > 0 || from < 0 && to < 0)
                return
            GlobalHelper.ungrabMouse(control)
            control.background._doubleClicked = true
            GlobalHelper.setProperty(control, "value", 0)
        }
    }
}