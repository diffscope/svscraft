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

#ifndef SVSCRAFT_MUSICMODE_H
#define SVSCRAFT_MUSICMODE_H

#include <SVSCraftCore/MusicPitch.h>

namespace SVS {

    class SVSCRAFT_CORE_EXPORT MusicMode {
    public:
        enum Mode {
            Ionian = (1 << 0) | (1 << 2) | (1 << 4) | (1 << 5) | (1 << 7) | (1 << 9) | (1 << 11),
            Dorian = (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 7) | (1 << 9) | (1 << 10),
            Phrygian = (1 << 0) | (1 << 1) | (1 << 3) | (1 << 5) | (1 << 7) | (1 << 8) | (1 << 10),
            Lydian = (1 << 0) | (1 << 2) | (1 << 4) | (1 << 6) | (1 << 7) | (1 << 9) | (1 << 11),
            Mixolydian = (1 << 0) | (1 << 2) | (1 << 4) | (1 << 5) | (1 << 7) | (1 << 9) | (1 << 10),
            Aeolian = (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 7) | (1 << 8) | (1 << 10),
            Locrian = (1 << 0) | (1 << 1) | (1 << 3) | (1 << 5) | (1 << 6) | (1 << 8) | (1 << 10),

            Gong = (1 << 0) | (1 << 2) | (1 << 4) | (1 << 7) | (1 << 9),
            Shang = (1 << 0) | (1 << 2) | (1 << 5) | (1 << 7) | (1 << 10),
            Jue = (1 << 0) | (1 << 3) | (1 << 5) | (1 << 8) | (1 << 10),
            Zhi = (1 << 0) | (1 << 2) | (1 << 5) | (1 << 7) | (1 << 9),
            Yu = (1 << 0) | (1 << 3) | (1 << 5) | (1 << 7) | (1 << 10),
        };

        constexpr MusicMode() : flags(1) {
        }

        constexpr MusicMode(Mode mode) : flags(mode) {
        }

        constexpr MusicMode &operator=(Mode mode) {
            flags = mode;
            return *this;
        }

        constexpr explicit MusicMode(int flags) : flags(flags) {
            Q_ASSERT(flags & 1);
        }

        constexpr int keyCount() const {
            int cnt = 0;
            for (int i = 0; i < 12; i++) {
                cnt += ((1 << i) & flags) ? 1 : 0;
            }
            return cnt;
        }

        QList<MusicPitch> scale(MusicPitch tonic) const;

    private:
        int flags;
    };
    
}

Q_DECLARE_METATYPE(SVS::MusicMode)

#endif // SVSCRAFT_MUSICMODE_H
