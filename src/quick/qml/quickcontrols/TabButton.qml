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

T.TabButton {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    font: Theme.font

    padding: 6
    horizontalPadding: 8
    spacing: 4
    icon.width: 16
    icon.height: 16
    icon.color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
                control.down ? Theme.foregroundPressedColorChange.apply(Theme.foregroundPrimaryColor) :
                control.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundPrimaryColor) :
                Theme.foregroundPrimaryColor
    Behavior on icon.color {
        ColorAnimation {
            duration: Theme.colorAnimationDuration
            easing.type: Easing.OutCubic
        }
    }

    DescriptiveText.statusTip: action?.DescriptiveAction.statusTip ?? ""
    DescriptiveText.contextHelpTip: action?.DescriptiveAction.contextHelpTip ?? ""
    DescriptiveText.activated: hovered

    ThemedItem.controlType: SVS.CT_Accent

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font

        color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
               control.down ? Theme.foregroundPressedColorChange.apply(Theme.foregroundPrimaryColor) :
               control.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundPrimaryColor) :
               Theme.foregroundPrimaryColor

        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
    }

    background: ButtonRectangle {
        control: control
        flat: true
        implicitWidth: 64
        implicitHeight: 20
        property color _baseColor: control.checked && control.ThemedItem.tabIndicator === SVS.TI_Fill ? Theme.accentColor : Qt.rgba(Theme.buttonColor.r, Theme.buttonColor.g, Theme.buttonColor.b, 0)
        color: !control.enabled ? _baseColor.a ? Theme.controlDisabledColorChange.apply(_baseColor) : _baseColor :
               control.down ? _baseColor.a ? Theme.controlPressedColorChange.apply(_baseColor) : Theme.controlPressedColorChange.apply(Theme.buttonColor) :
               control.hovered? _baseColor.a ? Theme.controlHoveredColorChange.apply(_baseColor) : Theme.controlHoveredColorChange.apply(Theme.buttonColor) :
               _baseColor
        radius: 4

        Rectangle {
            visible: control.ThemedItem.tabIndicator !== SVS.TI_Fill
            anchors.top: control.ThemedItem.tabIndicator === SVS.TI_Top ? parent.top : undefined
            anchors.bottom: control.ThemedItem.tabIndicator === SVS.TI_Bottom ? parent.bottom : undefined
            anchors.left: control.ThemedItem.tabIndicator === SVS.TI_Left ? parent.left : undefined
            anchors.right: control.ThemedItem.tabIndicator === SVS.TI_Right ? parent.right : undefined
            anchors.verticalCenter: control.ThemedItem.tabIndicator === SVS.TI_Left || control.ThemedItem.tabIndicator === SVS.TI_Right ? parent.verticalCenter : undefined
            anchors.horizontalCenter: control.ThemedItem.tabIndicator === SVS.TI_Top || control.ThemedItem.tabIndicator === SVS.TI_Bottom ? parent.horizontalCenter : undefined
            width: control.ThemedItem.tabIndicator === SVS.TI_Top || control.ThemedItem.tabIndicator === SVS.TI_Bottom ? parent.width * 0.75 : 2
            height: control.ThemedItem.tabIndicator === SVS.TI_Left || control.ThemedItem.tabIndicator === SVS.TI_Right ? parent.height * 0.75 : 2
            radius: 1
            property color _baseColor: control.checked ? Theme.accentColor : "transparent"
            color: !control.enabled && _baseColor.a ? Theme.controlDisabledColorChange.apply(_baseColor) : _baseColor
            Behavior on color {
                ColorAnimation {
                    duration: Theme.colorAnimationDuration
                    easing.type: Easing.OutCubic
                }
            }
        }

    }
}