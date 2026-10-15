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
import QtQuick.Controls
import QtQuick.Controls.impl

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Pane {
    id: pane
    property Item header: null
    property string title: ""
    property string description: ""
    property qQuickIcon icon
    property Menu menu: null
    property int badgeType: SVS.CT_Accent
    property int badgeNumber: 0
    property string panelKey: ""
    property size minimumSize: Qt.size(0, 0)
    property size preferredSize: Qt.size(-1, -1)

    icon.width: 16
    icon.height: 16
    icon.color: Theme.foregroundPrimaryColor

    property DockingPanel panel: DockingPanel {
        key: pane.panelKey
        contentItem: pane
    }

    Binding {
        when: pane.panel?.contentItem === pane
        pane.panel.title: pane.title
        pane.panel.description: pane.description
        pane.panel.iconSource: pane.icon.source
        pane.panel.iconColor: pane.icon.color
        pane.panel.badgeNumber: pane.badgeNumber
        pane.panel.badgeType: pane.badgeType
        pane.panel.minimumSize: pane.minimumSize
        pane.panel.preferredSize: pane.preferredSize
        pane.panel.headerItem: pane.header
    }

    background: Item {}
    Docking.panel: panel

    Accessible.name: title
    Accessible.description: description
}
