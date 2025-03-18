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

T.RangeSlider {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, first.implicitHandleWidth + leftPadding + rightPadding, second.implicitHandleWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, first.implicitHandleHeight + topPadding + bottomPadding, second.implicitHandleHeight + topPadding + bottomPadding)

    padding: 6

    Rectangle {
        width: control.width
        height: control.height
        border.width: control.visualFocus ? 2 : 0
        color: "transparent"
        border.color: Theme.navigationColor
        radius: 4
    }

    first.handle: SliderHandle {
        controlItem: control
        visualPosition: control.first.visualPosition
        pressed: control.first.pressed
        hovered: control.first.hovered
    }

    second.handle: SliderHandle {
        controlItem: control
        visualPosition: control.second.visualPosition
        pressed: control.second.pressed
        hovered: control.second.hovered
    }

    background: SliderBackground {
        controlItem: control
        start: control.first.handle._animatedVisualPosition
        end: control.second.handle._animatedVisualPosition
    }

    TapHandler {
        onDoubleTapped: (eventPoint) => {
            if (!Theme.doubleClickResetEnabled)
                return
            GlobalHelper.ungrabMouse(control)
            let p = eventPoint.position
            let visualClickedPosition = control.horizontal ? (p.x - control.leftPadding) / control.availableWidth : (p.y - control.rightPadding) / control.availableHeight
            let clickedPosition = control.vertical || control.mirrored ? 1 - visualClickedPosition : visualClickedPosition;
            let clickedValue = control.valueAt(clickedPosition)
            let [node, targetValue] = Math.abs(clickedValue - control.first.value) <= Math.abs(clickedValue - control.second.value) ? [control.first, control.from] : [control.second, control.to]
            node.handle._doubleClicked = node.handle._doubleClickTriggered = true
            GlobalHelper.setProperty(node, "value", targetValue)
            node.handle._doubleClickTriggered = false
        }
    }
}
