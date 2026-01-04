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

    font: Theme.font

    Rectangle {
        width: control.width
        height: control.height
        border.width: control.visualFocus ? 2 : 0
        color: "transparent"
        border.color: Theme.navigationColor
        radius: 4
    }

    handle: SliderHandle {
        controlItem: control
        visualPosition: control.visualPosition
        pressed: control.pressed
        hovered: control.hovered
    }

    background: SliderBackground {
        controlItem: control
        property double trackStart: -Math.min(control.from, control.to) / Math.abs(control.to - control.from)
        start: control.vertical || control.mirrored ? 1 - trackStart : trackStart
        end: control.handle._animatedVisualPosition
    }

    TapHandler {
        onDoubleTapped: () => {
            if (!Theme.doubleClickResetEnabled)
                return
            if (from > 0 && to > 0 || from < 0 && to < 0)
                return
            GlobalHelper.ungrabMouse(control)
            control.handle._doubleClicked = control.handle._doubleClickTriggered = true
            GlobalHelper.setProperty(control, "value", 0)
            control.ThemedItem.doubleClickReset()
            control.handle._doubleClickTriggered = false
        }
    }

}
