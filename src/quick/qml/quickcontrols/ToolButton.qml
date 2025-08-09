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
import QtQuick.Controls.impl // NOTE: Qt Quick private API

import SVSCraft
import SVSCraft.UIComponents

T.ToolButton {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    padding: 4
    spacing: 4

    font: Theme.font

    flat: true

    icon.width: 16
    icon.height: 16
    icon.color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundColor(control.ThemedItem.foregroundLevel)) :
                control.down ? Theme.foregroundPressedColorChange.apply(Theme.foregroundColor(control.ThemedItem.foregroundLevel)) :
                control.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundColor(control.ThemedItem.foregroundLevel)) :
                Theme.foregroundColor(control.ThemedItem.foregroundLevel)
    Behavior on icon.color {
        ColorAnimation {
            duration: Theme.colorAnimationDuration
            easing.type: Easing.OutCubic
        }
    }

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font

        color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundColor(control.ThemedItem.foregroundLevel)) :
               control.down ? Theme.foregroundPressedColorChange.apply(Theme.foregroundColor(control.ThemedItem.foregroundLevel)) :
               control.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundColor(control.ThemedItem.foregroundLevel)) :
               Theme.foregroundColor(control.ThemedItem.foregroundLevel)

        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
    }

    background: Rectangle {
        implicitWidth: 24
        implicitHeight: 24
        function transparentIf(condition, color) {
            return condition ? Qt.rgba(color.r, color.g, color.b, 0) : color
        }
        property color _checkedColor: control.flat ? control.ThemedItem.controlType === SVS.CT_Normal ? Theme.controlCheckedColorChange.apply(Theme.buttonColor) : Theme.controlColor(control.ThemedItem.controlType) : control.ThemedItem.controlType === SVS.CT_Normal ? Theme.accentColor : Theme.controlColor(control.ThemedItem.controlType)
        property color _baseColor: control.checkable || control.highlighted ? control.checked || control.highlighted ? _checkedColor : Theme.buttonColor : Theme.controlColor(control.ThemedItem.controlType)
        property color _unactiveColor: control.checkable || control.highlighted ? control.checked || control.highlighted ? _checkedColor : transparentIf(control.flat, Theme.buttonColor) : transparentIf(control.flat, Theme.controlColor(control.ThemedItem.controlType))
        property color _statusColor: control.down ? Theme.controlPressedColorChange.apply(_baseColor) :
                                     control.hovered ? Theme.controlHoveredColorChange.apply(_baseColor) :
                                     _unactiveColor
        color: !control.enabled ? transparentIf(control.flat, Theme.controlDisabledColorChange.apply(_unactiveColor)) : _statusColor
        border.color: control.visualFocus ? Theme.navigationColor : Theme.borderColor
        border.width: control.visualFocus ? 2 : control.flat || !control.checkable && control.ThemedItem.controlType !== SVS.CT_Normal || control.checkable && control.checked ? 0 : 1
        radius: 4

        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
    }
}