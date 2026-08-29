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

    namespace {
        constexpr MusicMode m(std::initializer_list<int> scale) {
            int mask = 0;
            for (int i : scale) {
                mask |= 1 << i;
            }
            return MusicMode(mask);
        }

        constexpr char majorModeName[] = QT_TRANSLATE_NOOP("SVS::MusicModeInfo", "Major");
        constexpr char minorModeName[] = QT_TRANSLATE_NOOP("SVS::MusicModeInfo", "Minor");
        constexpr char harmonicMajorModeName[] = QT_TRANSLATE_NOOP("SVS::MusicModeInfo", "Harmonic Major");
        constexpr char harmonicMinorModeName[] = QT_TRANSLATE_NOOP("SVS::MusicModeInfo", "Harmonic Minor");
        constexpr char melodicMajorModeName[] = QT_TRANSLATE_NOOP("SVS::MusicModeInfo", "Melodic Major");
        constexpr char melodicMinorModeName[] = QT_TRANSLATE_NOOP("SVS::MusicModeInfo", "Melodic Minor");
        constexpr char dorianModeName[] = QT_TRANSLATE_NOOP("SVS::MusicModeInfo", "Dorian");
        constexpr char phrygianModeName[] = QT_TRANSLATE_NOOP("SVS::MusicModeInfo", "Phrygian");
        constexpr char lydianModeName[] = QT_TRANSLATE_NOOP("SVS::MusicModeInfo", "Lydian");
        constexpr char mixolydianModeName[] = QT_TRANSLATE_NOOP("SVS::MusicModeInfo", "Mixolydian");
        constexpr char locrianModeName[] = QT_TRANSLATE_NOOP("SVS::MusicModeInfo", "Locrian");
        constexpr char atonalModeName[] = QT_TRANSLATE_NOOP("SVS::MusicModeInfo", "Atonal");

    }


    QList<MusicModeInfo> MusicModeInfo::getBuiltInMusicModeInfoList() {
        return {
            {m({0, 2, 4, 5, 7, 9, 11}), QCoreApplication::translate("SVS::MusicModeInfo", majorModeName)},
            {m({0, 2, 3, 5, 7, 8, 10}), QCoreApplication::translate("SVS::MusicModeInfo", minorModeName)},
            {m({0, 2, 4, 5, 7, 8, 11}), QCoreApplication::translate("SVS::MusicModeInfo", harmonicMajorModeName)},
            {m({0, 2, 3, 5, 7, 8, 11}), QCoreApplication::translate("SVS::MusicModeInfo", harmonicMinorModeName)},
            {m({0, 2, 4, 5, 7, 8, 10}), QCoreApplication::translate("SVS::MusicModeInfo", melodicMajorModeName)},
            {m({0, 2, 3, 5, 7, 9, 11}), QCoreApplication::translate("SVS::MusicModeInfo", melodicMinorModeName)},
            {m({0, 2, 3, 5, 7, 9, 10}), QCoreApplication::translate("SVS::MusicModeInfo", dorianModeName)},
            {m({0, 1, 3, 5, 7, 8, 10}), QCoreApplication::translate("SVS::MusicModeInfo", phrygianModeName)},
            {m({0, 2, 4, 6, 7, 9, 11}), QCoreApplication::translate("SVS::MusicModeInfo", lydianModeName)},
            {m({0, 2, 4, 5, 7, 9, 10}), QCoreApplication::translate("SVS::MusicModeInfo", mixolydianModeName)},
            {m({0, 1, 3, 5, 6, 8, 10}), QCoreApplication::translate("SVS::MusicModeInfo", locrianModeName)},
            {MusicMode(0), QCoreApplication::translate("SVS::MusicModeInfo", atonalModeName)},
        };
    }

}
