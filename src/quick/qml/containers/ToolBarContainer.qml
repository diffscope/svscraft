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
import QtQuick.Layouts
import QtQuick.Controls.Basic as Basic
import QtQuick.Templates as T

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Basic.Container {
    id: container

    property bool showMenuAboveButton: false
    property bool vertical: false
    property Component toolButtonComponent: ToolButton {
        display: icon.source.toString().length !== 0 ? T.AbstractButton.IconOnly : T.AbstractButton.TextBesideIcon
        DescriptiveText.bindAccessibleDescription: true
    }

    spacing: 5

    function addAction(action: T.Action) {
        addItem(toolButtonComponent.createObject(this, { action }))
    }
    function addMenu(menu: T.Menu) {
        menu.parent = this
        addAction(menuActionComponent.createObject(this, { menu }))
    }
    function insertAction(index: int, action: T.Action) {
        insertItem(index, toolButtonComponent.createObject(this, { action }))
    }
    function insertMenu(index: int, menu: T.Menu) {
        menu.parent = this
        insertAction(index, menuActionComponent.createObject(this, { menu }))
    }
    function removeAction(action: T.Action) {
        if (!action)
            return
        for (let i = count - 1; i >= 0; i--) {
            let item = itemAt(i)
            if (item.action === action) {
                removeItem(item)
                return
            }
        }
    }
    function removeMenu(menu: T.Menu) {
        if (!menu)
            return
        for (let i = count - 1; i >= 0; i--) {
            let item = itemAt(i)
            if (item.action?.menu === menu) {
                removeItem(item)
                return
            }
        }
    }
    function takeAction(index: int) {
        let item = itemAt(i)
        let action = item.action
        if (action) {
            removeItem(item)
        }
        return action
    }
    function takeMenu(index: int) {
        let item = itemAt(i)
        let menu = item.action?.menu
        if (menu) {
            removeItem(item)
        }
        return menu
    }

    Component {
        id: menuActionComponent
        MenuAction {
            menuDisplay: container.showMenuAboveButton ? MenuAction.Top : MenuAction.Bottom
        }
    }

    contentItem: GridLayout {
        columnSpacing: container.spacing
        rowSpacing: container.spacing
        flow: container.vertical ? GridLayout.TopToBottom : GridLayout.LeftToRight
        columns: container.vertical ? 1 : -1
        rows: container.vertical ? -1 : 1
        Repeater {
            model: container.contentModel
        }
    }
}