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
import QtQuick.Templates as T
import QtQuick.Controls.impl

T.SpinBox {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    leftPadding: padding + control.mirrored ? Math.max((down.indicator ? down.indicator.width : 0), (up.indicator ? up.indicator.width : 0)) : 0
    rightPadding: padding + control.mirrored ? 0 : Math.max((down.indicator ? down.indicator.width : 0), (up.indicator ? up.indicator.width : 0))

    font: Theme.font

    editable: true

    validator: IntValidator {
        locale: control.locale.name
        bottom: Math.min(control.from, control.to)
        top: Math.max(control.from, control.to)
    }

    contentItem: TextInput {
        text: control.displayText
        padding: 6
        leftPadding: 8
        rightPadding: 8
        font: control.font

        color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
               Theme.foregroundPrimaryColor
        selectionColor: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.accentColor) :
                        Theme.accentColor
        selectedTextColor: color
        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
        Behavior on selectionColor {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }

        verticalAlignment: Text.AlignVCenter
        readOnly: !control.editable
        validator: control.validator
        inputMethodHints: control.inputMethodHints
    }

    up.indicator: Rectangle {
        x: control.mirrored ? 3 : control.width - width - 3
        y: 3
        height: control.height / 2 - 3
        implicitWidth: 24
        implicitHeight: 12
        radius: 1
        property color _statusColor: control.up.pressed ? Theme.controlPressedColorChange.apply(Theme.buttonColor) :
                                     control.up.hovered ? Theme.controlHoveredColorChange.apply(Theme.buttonColor) :
                                     "transparent"
        color: !enabled ? "transparent" : _statusColor
        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
        ColorImage {
            anchors.centerIn: parent
            color: !enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
                   Theme.foregroundPrimaryColor
            source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/ChevronUp12Filled.svg"
            Behavior on color {
                ColorAnimation {
                    duration: Theme.colorAnimationDuration
                    easing.type: Easing.OutCubic
                }
            }
        }
    }

    down.indicator: Rectangle {
        x: control.mirrored ? 3 : control.width - width - 3
        y: control.height / 2
        height: control.height / 2 - 3
        implicitWidth: 24
        implicitHeight: 12
        radius: 1
        property color _statusColor: control.down.pressed ? Theme.controlPressedColorChange.apply(Theme.buttonColor) :
                                     control.down.hovered ? Theme.controlHoveredColorChange.apply(Theme.buttonColor) :
                                     "transparent"
        color: !enabled ? "transparent" : _statusColor
        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
        ColorImage {
            anchors.centerIn: parent
            color: !enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
                   Theme.foregroundPrimaryColor
            source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/ChevronDown12Filled.svg"
            Behavior on color {
                ColorAnimation {
                    duration: Theme.colorAnimationDuration
                    easing.type: Easing.OutCubic
                }
            }
        }
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 28
        border.width: control.activeFocus ? 2 : 1
        color: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.textFieldColor) :
               Theme.textFieldColor
        border.color: control.activeFocus ? Theme.accentColor : Theme.borderColor
        radius: 4
        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
        Behavior on border.color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
    }
}
