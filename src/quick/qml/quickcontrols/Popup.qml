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
import QtQuick.Effects
import QtQuick.Controls.impl

import SVSCraft
import SVSCraft.UIComponents

T.Popup {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding)

    padding: 8

    background: Item {
        Rectangle {
            id: backgroundArea
            anchors.fill: parent
            color: Theme.backgroundColor(control.ThemedItem.backgroundLevel)
            border.color: Theme.borderColor
            radius: 4
        }
        MultiEffect {
            source: backgroundArea
            anchors.fill: parent
            shadowEnabled: true
            shadowColor: Theme.shadowColor
        }
    }

    T.Overlay.modal: Rectangle {
        color: Color.transparent(Theme.shadowColor, 0.5)
    }

    T.Overlay.modeless: Rectangle {
        color: Color.transparent(Theme.shadowColor, 0.12)
    }
}