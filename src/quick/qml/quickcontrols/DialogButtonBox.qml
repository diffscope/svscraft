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

import SVSCraft.UIComponents

DialogButtonBoxTemplate {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
        layoutImplicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
        layoutImplicitHeight + topPadding + bottomPadding)
    contentWidth: layoutImplicitWidth
    contentHeight: layoutImplicitHeight

    spacing: 12
    padding: 12
    topPadding: 0
    alignment: Qt.AlignRight

    delegate: Button {}

    contentItem: Item {}

    background: Item {
        implicitHeight: 48
    }
}
