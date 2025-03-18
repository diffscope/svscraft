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

import SVSCraft.UIComponents

Rectangle {
    id: handle
    required property Item controlItem
    required property double visualPosition
    required property bool pressed
    required property bool hovered

    property double _animatedVisualPosition: visualPosition
    property bool _doubleClicked: false
    property bool _doubleClickTriggered: false
    onVisualPositionChanged: () => {
        if (!_doubleClickTriggered)
            handle._doubleClicked = false
    }
    Behavior on _animatedVisualPosition {
        enabled: handle._doubleClicked
        NumberAnimation {
            duration: Theme.visualEffectAnimationDuration
            easing.type: Easing.OutCubic
            onRunningChanged: () => {
                if (!running) {
                    handle._doubleClicked = false
                }
            }
        }
    }
    x: controlItem.leftPadding + (controlItem.horizontal ? _animatedVisualPosition * (controlItem.availableWidth - width) : (controlItem.availableWidth - width) / 2)
    y: controlItem.topPadding + (controlItem.horizontal ? (controlItem.availableHeight - height) / 2 : _animatedVisualPosition * (controlItem.availableHeight - height))
    implicitWidth: 12
    implicitHeight: 12
    radius: width / 2
    width: implicitWidth * (!controlItem.enabled ? 1.1 : pressed ? 1 : hovered ? 1.25 : 1.1)
    height: width
    color: !controlItem.enabled ? Theme.controlDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
           pressed ? Theme.controlPressedColorChange.apply(Theme.foregroundPrimaryColor) :
           hovered ? Theme.controlHoveredColorChange.apply(Theme.foregroundPrimaryColor) :
           Theme.foregroundPrimaryColor
    Behavior on width {
        NumberAnimation {
            duration: Theme.visualEffectAnimationDuration
            easing.type: Easing.OutCubic
        }
    }
    Behavior on color {
        ColorAnimation {
            duration: Theme.colorAnimationDuration
            easing.type: Easing.OutCubic
        }
    }
}