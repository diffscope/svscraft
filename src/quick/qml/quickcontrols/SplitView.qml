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

    font: Theme.font

    handle: Rectangle {
        id: handleDelegate
        implicitWidth: Theme.splitterHandleSize
        implicitHeight: Theme.splitterHandleSize
        color: control.enabled && T.SplitHandle.pressed ? Theme.accentColor : Theme.dividerStrokeColor(control.ThemedItem.dividerStroke, Theme.splitterColor)
        containmentMask: handleMouse
        visible: control.ThemedItem.splitHandleVisible

        MouseArea {
            id: handleMouse

            readonly property bool horizontal: control.orientation === Qt.Horizontal
            readonly property real hitMargin: 3

            enabled: control.ThemedItem.splitHandleEnabled
            x: (handleDelegate.width - width) / 2
            y: (handleDelegate.height - height) / 2
            width: enabled ? handleDelegate.width + (horizontal ? 2 * hitMargin : 0) : 0
            height: enabled ? handleDelegate.height + (horizontal ? 0 : 2 * hitMargin) : 0
            cursorShape: horizontal ? Qt.SplitHCursor : Qt.SplitVCursor
        }
    }
}
