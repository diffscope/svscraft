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

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

T.Slider {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitHandleWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitHandleHeight + topPadding + bottomPadding)

    padding: 6

    Rectangle {
        width: control.width
        height: control.height
        border.width: control.visualFocus ? 2 : 0
        color: "transparent"
        border.color: Theme.navigationColor
        radius: 4
    }

    handle: Rectangle {
        property double _animatedVisualPosition: control.visualPosition
        property bool _doubleClicked: false
        Connections {
            target: control
            function onVisualPositionChanged() {
                control.handle._doubleClicked = false
            }
        }
        Behavior on _animatedVisualPosition {
            enabled: control.handle._doubleClicked
            NumberAnimation {
                duration: control.Theme.visualEffectAnimationDuration
                easing.type: Easing.OutCubic
                onRunningChanged: () => {
                    if (!running) {
                        control.handle._doubleClicked = false
                    }
                }
            }
        }
        x: control.leftPadding + (control.horizontal ? _animatedVisualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : _animatedVisualPosition * (control.availableHeight - height))
        implicitWidth: 12
        implicitHeight: 12
        radius: width / 2
        width: implicitWidth * (!control.enabled ? 1.1 : control.pressed ? 1 : control.hovered ? 1.25 : 1.1)
        height: width
        color: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
               control.pressed ? Theme.controlPressedColorChange.apply(Theme.foregroundPrimaryColor) :
               control.hovered ? Theme.controlHoveredColorChange.apply(Theme.foregroundPrimaryColor) :
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

    Theme.onDoubleClickResetEnabledChanged: () => {
        if (Theme.doubleClickResetEnabled) {
            AttachedHelper.installDoubleClickEventFilter()
        } else {
            AttachedHelper.removeDoubleClickEventFilter()
        }
    }

    Component.onCompleted: () => {
        if (Theme.doubleClickResetEnabled) {
            AttachedHelper.installDoubleClickEventFilter()
        }
    }

    AttachedHelper.onDoubleClicked: () => {
        if (from > 0 && to > 0 || from < 0 && to < 0)
            return
        control.handle._doubleClicked = true
        AttachedHelper.setProperty("value", 0)
    }

    background: Rectangle {
        x: control.leftPadding + (control.horizontal ? 0 : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : 0)
        implicitWidth: control.horizontal ? 200 : 4
        implicitHeight: control.horizontal ? 4 : 200
        width: control.horizontal ? control.availableWidth : implicitWidth
        height: control.horizontal ? implicitHeight : control.availableHeight
        radius: 2
        color: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.buttonColor) :
               Theme.buttonColor
        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
        scale: control.horizontal && control.mirrored ? -1 : 1

        Rectangle {
            property double trackStart: -Math.min(control.from, control.to) / Math.abs(control.to - control.from)
            property double visualTrackStart: control.vertical || control.mirrored ? 1 - trackStart : trackStart
            x: control.horizontal ? Math.min(trackStart, control.mirrored ? 1 - control.handle._animatedVisualPosition : control.handle._animatedVisualPosition) * parent.width : 0
            y: control.horizontal ? 0 : Math.min(control.handle._animatedVisualPosition, visualTrackStart) * parent.height
            width: control.horizontal ? Math.abs(control.handle._animatedVisualPosition - visualTrackStart) * parent.width : 4
            height: control.horizontal ? 4 : Math.abs(control.handle._animatedVisualPosition - visualTrackStart) * parent.height

            radius: 2
            color: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.accentColor) :
                   Theme.accentColor
            Behavior on color {
                ColorAnimation {
                    duration: Theme.colorAnimationDuration
                    easing.type: Easing.OutCubic
                }
            }
        }
    }
}
