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
    id: background
    
    required property Item controlItem
    required property double start
    required property double end
    
    x: controlItem.leftPadding + (controlItem.horizontal ? 0 : (controlItem.availableWidth - width) / 2)
    y: controlItem.topPadding + (controlItem.horizontal ? (controlItem.availableHeight - height) / 2 : 0)
    implicitWidth: controlItem.horizontal ? 200 : 4
    implicitHeight: controlItem.horizontal ? 4 : 200
    width: controlItem.horizontal ? controlItem.availableWidth : implicitWidth
    height: controlItem.horizontal ? implicitHeight : controlItem.availableHeight
    radius: 2
    color: !controlItem.enabled ? Theme.controlDisabledColorChange.apply(Theme.buttonColor) :
           Theme.buttonColor
    Behavior on color {
        ColorAnimation {
            duration: Theme.colorAnimationDuration
            easing.type: Easing.OutCubic
        }
    }

    Rectangle {
        x: controlItem.horizontal ? Math.min(background.start, background.end) * parent.width : 0
        y: controlItem.horizontal ? 0 : Math.min(background.start, background.end) * parent.height
        width: controlItem.horizontal ? Math.abs(background.start - background.end) * parent.width : 4
        height: controlItem.horizontal ? 4 : Math.abs(background.start - background.end) * parent.height

        radius: 2
        color: !controlItem.enabled ? Theme.controlDisabledColorChange.apply(Theme.accentColor) :
               Theme.accentColor
        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
    }
}