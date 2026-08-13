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
import QtQuick.Controls.impl as Impl

import SVSCraft
import SVSCraft.UIComponents

Impl.IconLabel { // TODO avoid Qt private implementation in future
    id: control

    spacing: 4

    icon.width: 16
    icon.height: 16
    icon.color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundColor(control.ThemedItem.foregroundLevel)) : Theme.foregroundColor(control.ThemedItem.foregroundLevel)

    Accessible.role: Accessible.StaticText
    // Accessible.name: text

    font: Theme.font

    color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundColor(control.ThemedItem.foregroundLevel)) : Theme.foregroundColor(control.ThemedItem.foregroundLevel)
}
