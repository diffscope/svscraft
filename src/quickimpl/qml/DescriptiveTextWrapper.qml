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
import SVSCraft.UIComponents

Item {
    id: wrapper
    required property QtObject descriptiveText
    required property Item attachee

    Binding {
        wrapper.attachee.ToolTip.visible: wrapper.descriptiveText.activated && wrapper.descriptiveText.toolTip
        wrapper.attachee.ToolTip.text: wrapper.descriptiveText.toolTip
        wrapper.attachee.ToolTip.delay: Theme.toolTipDelay
        wrapper.attachee.ToolTip.timeout: Theme.toolTipTimeout
        wrapper.attachee.Accessible.description: wrapper.descriptiveText.toolTip && wrapper.descriptiveText.statusTip ? wrapper.descriptiveText.toolTip + "\n" + wrapper.descriptiveText.statusTip : (wrapper.descriptiveText.toolTip || wrapper.descriptiveText.statusTip)
    }
}