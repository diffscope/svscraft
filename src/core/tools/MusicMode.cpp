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

#include "MusicMode.h"

namespace SVS {

    QList<MusicPitch> MusicMode::scale(MusicPitch tonic) const {
        QList<MusicPitch> pitches;
        pitches.reserve(12);
        for (int i = 0; i < 12; i++) {
            if ((1 << i) & m_mask) {
                if (tonic.isWildcard()) {
                    pitches.append(MusicPitch(qint8(tonic.key() + i % 12), MusicPitch::Wildcard));
                } else if (tonic.pitch() + i < 128) {
                    pitches.append(MusicPitch(qint8(tonic.pitch() + i)));
                }
            }
        }
        return pitches;
    }

    MusicPitch::Key MusicMode::detectTonality(const QList<MusicPitch> &notes) const {
        int bestTonic = 0;
        int bestCount = -1;
        for (int tonic = 0; tonic < 12; ++tonic) {
            int count = 0;
            for (const auto &note : notes) {
                const int interval = (note.key() - tonic + 12) % 12;
                if (m_mask & (1 << interval)) {
                    ++count;
                }
            }
            if (count > bestCount) {
                bestTonic = tonic;
                bestCount = count;
            }
        }
        return static_cast<MusicPitch::Key>(bestTonic);
    }

}
