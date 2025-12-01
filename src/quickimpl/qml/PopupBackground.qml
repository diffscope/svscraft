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

Item {
    id: backgroundItem
    property Item shadowItem: null
    property T.Popup control: null
    Rectangle {
        id: backgroundArea
        anchors.fill: parent
        color: Theme.backgroundColor(control?.ThemedItem.backgroundLevel ?? SVS.BL_Primary)
        border.color: Theme.borderColor
        radius: 4
    }
    Component {
        id: shadowComponent
        MultiEffect {
            source: backgroundArea
            anchors.fill: parent
            shadowEnabled: true
            shadowColor: Theme.shadowColor
        }
    }
    Component.onCompleted: () => {
        backgroundItem.shadowItem = shadowComponent.createObject(backgroundItem)
    }
    // FIXME
    Connections {
        target: control
        function onAboutToShow() {
            if (backgroundItem.shadowItem) {
                backgroundItem.shadowItem.destroy()
            }
            backgroundItem.shadowItem = shadowComponent.createObject(backgroundItem)
        }
        function onClosed() {
            if (backgroundItem.shadowItem) {
                backgroundItem.shadowItem.destroy()
            }
        }
    }

}