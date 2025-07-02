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

import SVSCraft
import SVSCraft.UIComponents

Action {
    id: action
    enum MenuDisplay {
        Top,
        Bottom
    }
    property Menu menu: null
    property Item parentItem: null
    property int menuDisplay: MenuAction.Bottom
    readonly property Action dummyAction: Action {}
    icon: menu?.icon ?? dummyAction.icon
    text: menu?.title ?? ""
    onTriggered: () => {
        if (menu) {
            menu.popup(parentItem, 0, !parentItem ? 0 : menuDisplay === MenuAction.Bottom ? parentItem.height : -menu.height)
        }
    }
}