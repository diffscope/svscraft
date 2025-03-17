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

#ifndef SVSCRAFT_MUSICTIMELINE_P_H
#define SVSCRAFT_MUSICTIMELINE_P_H

#include <QMap>
#include <QSet>

#include <SVSCraftCore/MusicTimeline.h>
#include <SVSCraftCore/private/MusicTime_p.h>
#include <SVSCraftCore/MusicTimeSignature.h>

namespace SVS {

    class SVSCRAFT_CORE_EXPORT MusicTimelinePrivate {
    public:
        Q_DECLARE_PUBLIC(MusicTimeline)
        MusicTimelinePrivate();
        virtual ~MusicTimelinePrivate();

        void init();

        MusicTimeline *q_ptr;

        int resolution = 480;

        QMap<int, MusicTimeSignature> timeSignatureMap; // bar position -> time signature
        QMap<int, int> measureMap;                      // tick position -> bar position
        QMap<int, int> revMeasureMap;                   // bar position -> tick position
        QMap<int, double> tempoMap;                     // tick position -> tempo
        QMap<int, double> msecSumMap;                   // tick position -> msecs in total
        QMap<double, int> revMsecSumMap;                // msecs in total -> tick position

        mutable QSet<PersistentMusicTimeData *> mbtCachedMusicTimes;
        mutable QSet<PersistentMusicTimeData *> msecCachedMusicTimes;

        void updateMeasureMap(int barFrom);
        void updateMsecSumMap(int tickFrom);

        void timeSignatureChanged();
        void tempoChanged();

        int findNearestTickWithTimeSignature(int tick) const;
        double findNearestMsecWithTempo(double msec) const;

        MusicTime tickToTime(int totalTick) const;
        double tickToMsec(int totalTick) const;

        int timeToTick(int measure, int beat, int tick) const;
        int stringToTick(QStringView str, bool *ok) const;
        int msecToTick(double msec) const;
    };

}

#endif // SVSCRAFT_MUSICTIMELINE_P_H
