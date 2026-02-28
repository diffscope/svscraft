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

#include "MusicModeInfo.h"

#include <QCoreApplication>

namespace SVS {

    constexpr MusicMode m(std::initializer_list<int> scale) {
        int mask = 0;
        for (int i : scale) {
            mask |= 1 << i;
        }
        return MusicMode(mask);
    }

    QList<MusicModeInfo> MusicModeInfo::getBuiltInMusicModeInfoList() {
        return {
            {m({0, 2, 4, 5, 7, 9, 11}), QCoreApplication::translate("SVS::MusicModeInfo", "Major")},
            {m({0, 2, 3, 5, 7, 8, 10}), QCoreApplication::translate("SVS::MusicModeInfo", "Minor")},
            {m({0, 2, 4, 5, 7, 8, 11}), QCoreApplication::translate("SVS::MusicModeInfo", "Harmonic Major")},
            {m({0, 2, 3, 5, 7, 8, 11}), QCoreApplication::translate("SVS::MusicModeInfo", "Harmonic Minor")},
            {m({0, 2, 4, 5, 7, 8, 10}), QCoreApplication::translate("SVS::MusicModeInfo", "Melodic Major")},
            {m({0, 2, 3, 5, 7, 9, 11}), QCoreApplication::translate("SVS::MusicModeInfo", "Melodic Minor")},
            {m({0, 2, 3, 5, 7, 9, 10}), QCoreApplication::translate("SVS::MusicModeInfo", "Dorian")},
            {m({0, 1, 3, 5, 7, 8, 10}), QCoreApplication::translate("SVS::MusicModeInfo", "Phrygian")},
            {m({0, 2, 4, 6, 7, 9, 11}), QCoreApplication::translate("SVS::MusicModeInfo", "Lydian")},
            {m({0, 2, 4, 5, 7, 9, 10}), QCoreApplication::translate("SVS::MusicModeInfo", "Mixolydian")},
            {m({0, 1, 3, 5, 6, 8, 10}), QCoreApplication::translate("SVS::MusicModeInfo", "Locrian")},
            {MusicMode(0), QCoreApplication::translate("SVS::MusicModeInfo", "Atonal")},
        };
    }

}
