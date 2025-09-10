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

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

T.MenuItem {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)

    padding: 4
    spacing: 4

    font: Theme.font

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

    ThemedItem.controlType: SVS.CT_Accent

    DescriptiveText.statusTip: subMenu?.DescriptiveAction.statusTip ?? action?.DescriptiveAction.statusTip ?? ""
    DescriptiveText.contextHelpTip: subMenu?.DescriptiveAction.contextHelpTip ?? action?.DescriptiveAction.contextHelpTip ?? ""
    DescriptiveText.activated: hovered
    DescriptiveText.bindAccessibleDescription: true

    contentItem: IconLabel {
        readonly property real arrowPadding: control.arrow ? control.arrow.width + control.spacing : 0
        readonly property real indicatorPadding: control.icon.source.toString() && !control.checkable ? 0 : 20
        leftPadding: !control.mirrored ? indicatorPadding : arrowPadding
        rightPadding: control.mirrored ? indicatorPadding : arrowPadding

        spacing: control.spacing
        mirrored: control.mirrored
        display: control.checkable ? IconLabel.TextOnly : control.display
        alignment: Qt.AlignLeft

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

    indicator: ColorImage {
        x: control.mirrored ? control.width - width - control.rightPadding - 2 : control.leftPadding + 2
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 12
        height: 12
        visible: control.checked
        source: control.checkable ? "qrc:/qt/qml/SVSCraft/UIComponents/assets/Checkmark12Filled.svg" : ""
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

    arrow: Item {
        x: control.mirrored ? control.leftPadding : control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: control.subMenu ? arrowItem.width : keySequenceItem.width
        height: control.subMenu ? arrowItem.height : keySequenceItem.height

        Text {
            id: keySequenceItem
            Shortcut {
                id: keySequenceShortcutHelper
                enabled: false
                sequence: control.action?.shortcut ?? ""
            }
            text: keySequenceShortcutHelper.nativeText
            visible: !control.subMenu
            font: control.font
            color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundSecondaryColor) :
                   control.down ? Theme.foregroundPressedColorChange.apply(Theme.foregroundSecondaryColor) :
                   control.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundSecondaryColor) :
                   Theme.foregroundSecondaryColor
            Behavior on color {
                ColorAnimation {
                    duration: Theme.colorAnimationDuration
                    easing.type: Easing.OutCubic
                }
            }
        }

        ColorImage {
            id: arrowItem
            visible: control.subMenu
            mirror: control.mirrored
            width: 12
            height: 12
            source: control.subMenu ? "qrc:/qt/qml/SVSCraft/UIComponents/assets/ChevronRight12Filled.svg" : ""
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

    }

    background: ButtonRectangle {
        control: control
        flat: true
        checked: false

        implicitWidth: 200
        implicitHeight: 24
        width: control.width
        height: control.height
        color: !control.enabled ? Theme.buttonColor :
               control.down ? Theme.controlPressedColorChange.apply(Theme.accentColor) :
               control.hovered || control.subMenu?.visible ? Theme.controlHoveredColorChange.apply(Theme.accentColor) :
               Theme.buttonColor
        radius: 2
    }
}
