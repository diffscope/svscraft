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
import SVSCraft.UIComponents.impl

SpinBox {
    id: control

    property alias quarterNoteWidth: validator.quarterNoteWidth
    property alias tickWidth: validator.tickWidth
    validator: MusicTimeOffsetValidator {
        id: validator
        bottom: control.from
        top: control.to
    }
    textFromValue: function(value) {
        return SVS.musicTimeOffset(value).toString(validator.quarterNoteWidth, validator.tickWidth)
    }
    valueFromText: function(text) {
        return SVS.musicTimeOffset(text).totalTick
    }
    from: 0
    to: 2147483647
}
