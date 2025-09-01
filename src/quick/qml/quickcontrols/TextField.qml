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
import QtQuick.Templates as T
import QtQuick.Controls.impl

import SVSCraft
import SVSCraft.UIComponents

T.TextField {
    id: control

    implicitWidth: implicitBackgroundWidth + leftInset + rightInset || Math.max(contentWidth, placeholder.implicitWidth) + leftPadding + rightPadding
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding, placeholder.implicitHeight + topPadding + bottomPadding)

    padding: 6
    leftPadding: ThemedItem.icon.source.toString().length ? 32 : 8
    rightPadding: 8

    ThemedItem.icon.width: 16
    ThemedItem.icon.height: 16
    ThemedItem.icon.color: color

    font: Theme.font

    color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
           Theme.foregroundPrimaryColor
    selectionColor: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.accentColor) :
                    Theme.accentColor
    selectedTextColor: color
    placeholderTextColor: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundSecondaryColor) :
                          Theme.foregroundSecondaryColor
    verticalAlignment: Text.AlignVCenter

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
    Behavior on placeholderTextColor {
        ColorAnimation {
            duration: Theme.colorAnimationDuration
            easing.type: Easing.OutCubic
        }
    }

    Text {
        id: placeholder
        x: control.leftPadding
        y: control.topPadding
        width: control.width - (control.leftPadding + control.rightPadding)
        height: control.height - (control.topPadding + control.bottomPadding)

        text: control.placeholderText
        font: control.font
        color: control.placeholderTextColor
        verticalAlignment: control.verticalAlignment
        visible: !control.length && !control.preeditText && (!control.activeFocus || control.horizontalAlignment !== Qt.AlignHCenter)
        elide: Text.ElideRight
        renderType: control.renderType
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 28
        border.width: control.activeFocus ? 2 : 1
        color: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.textFieldColor) :
               Theme.textFieldColor
        border.color: control.activeFocus ? control.ThemedItem.controlType === SVS.CT_Normal ? Theme.accentColor : Theme.controlColor(control.ThemedItem.controlType) : Theme.borderColor
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

    ColorImage {
        width: control.ThemedItem.icon.width
        height: control.ThemedItem.icon.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 8
        source: control.ThemedItem.icon.source
        color: control.ThemedItem.icon.color
    }
}
