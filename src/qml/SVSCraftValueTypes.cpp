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

#include "SVSCraftValueTypes_p.h"

#include <QVariant>

namespace SVS {
    LongTimeForeign::LongTimeForeign(const QVariantMap &time) {
        v = LongTime(time.value("minute").toInt(), time.value("second").toInt(), time.value("millisecond").toInt());
    }

    MusicTimeForeign::MusicTimeForeign(const QVariantMap &time) {
        v = MusicTime(time.value("measure").toInt(), time.value("beat").toInt(), time.value("tick").toInt());
    }
    MusicTimeSignatureForeign::MusicTimeSignatureForeign(const QVariantMap &signature) {
        v = MusicTimeSignature(signature.value("numerator").toInt(), signature.value("denominator").toInt());
    }
    MusicPitchForeign::MusicPitchForeign(const QVariantMap &pitch) {
        auto key = pitch.value("key").toInt();
        auto octave = pitch.value("octave").toInt();
        auto prediction = key >= 0 && key < 12 && (octave == -1 || (octave >= 0 && key + octave * 12 >= 0 && key + octave * 12 < 128));
        if (!prediction) {
            v = {};
        } else {
            v = MusicPitch(key, octave);
        }
    }
}

#include "moc_SVSCraftValueTypes_p.cpp"