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
import QtQuick.Controls.impl

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

ToolTip {
    id: control

    property string shortcut: ""

    contentItem: RowLayout {
        spacing: 8
        MnemonicLabel {
            text: control.text
            font: control.font
            wrapMode: Text.Wrap
            color: Theme.foregroundPrimaryColor
        }
        Text {
            visible: control.shortcut.length !== 0
            Shortcut {
                id: keySequenceShortcutHelper
                enabled: false
                sequence: control.shortcut
            }
            text: keySequenceShortcutHelper.nativeText
            font: control.font
            color: Theme.foregroundSecondaryColor
        }
    }
}