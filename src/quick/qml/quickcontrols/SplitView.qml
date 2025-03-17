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

import SVSCraft
import SVSCraft.UIComponents

T.SplitView {
    id: control
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    handle: Rectangle {
        id: handleDelegate
        implicitWidth: 1
        implicitHeight: 1
        color: control.enabled && T.SplitHandle.pressed ? Theme.accentColor : Theme.splitterColor
        containmentMask: Item {
            x: (handleDelegate.width - width) / 2
            property double _size: control.ThemedItem.splitHandleEnabled ? 5 : 0
            implicitWidth: control.orientation === Qt.Horizontal ? _size : control.width
            implicitHeight: control.orientation === Qt.Horizontal ? control.height : _size
        }
        visible: control.ThemedItem.splitHandleVisible
    }
}