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

#include "SVSQmlNamespace_p.h"

#include <SVSCraftCore/DecibelLinearizer.h>
#include <SVSCraftCore/MusicPitch.h>
#include <SVSCraftCore/MusicTime.h>

namespace SVS {

    double SVSQmlNamespace::decibelToLinearValue(double decibel, double factor) {
        return DecibelLinearizer::decibelToLinearValue(decibel, factor);
    }
    double SVSQmlNamespace::linearValueToDecibel(double linearValue, double factor) {
        return DecibelLinearizer::linearValueToDecibel(linearValue, factor);
    }
    double SVSQmlNamespace::decibelsToGain(double decibels, double minusInfinityDb) {
        return decibels > minusInfinityDb ? std::pow (10.0, decibels * .05) : .0;
    }
    double SVSQmlNamespace::gainToDecibels(double gain, double minusInfinityDb) {
        return gain > .0f ? qMax(minusInfinityDb, std::log10(gain) * 20.0) : minusInfinityDb;
    }
    MusicPitch SVSQmlNamespace::musicPitch(int key) {
        return MusicPitch(static_cast<qint8>(key));
    }
    MusicPitch SVSQmlNamespace::musicPitch(int key, int octave) {
        return MusicPitch(static_cast<qint8>(key), static_cast<qint8>(octave));
    }
    MusicPitch SVSQmlNamespace::musicPitch(const QString &s) {
        return MusicPitch::fromString(s);
    }
    MusicTime SVSQmlNamespace::musicTime(int measure, int beat, int tick) {
        return MusicTime(measure, beat, tick);
    }
    MusicTime SVSQmlNamespace::musicTime(const QString &s) {
        return MusicTime::fromString(s);
    }
    MusicTimeOffset SVSQmlNamespace::musicTimeOffset(int quarterNote, int tick) {
        return MusicTimeOffset(quarterNote, tick);
    }
    MusicTimeOffset SVSQmlNamespace::musicTimeOffset(int totalTick) {
        return MusicTimeOffset(totalTick);
    }
    MusicTimeOffset SVSQmlNamespace::musicTimeOffset(const QString &s) {
        return MusicTimeOffset::fromString(s);
    }

}

#include "moc_SVSQmlNamespace_p.cpp"
