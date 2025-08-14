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
import SVSCraft.UIComponents.impl

T.Button {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    padding: 6
    horizontalPadding: control.text && control.display !== T.AbstractButton.IconOnly ? 8 : 6
    spacing: 4

    font: Theme.font

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

    background: ButtonRectangle {
        control: control
        implicitWidth: control.text && control.display !== T.AbstractButton.IconOnly ? 64 : 28
        implicitHeight: 28
        radius: 4
    }

    ActionToolTipHelper {
        text: control.text
        shortcut: control.action?.shortcut ?? ""
        delay: Theme.toolTipDelay
        timeout: Theme.toolTipTimeout
        visible: control.text !== "" && control.display === T.AbstractButton.IconOnly && control.hovered
    }
}