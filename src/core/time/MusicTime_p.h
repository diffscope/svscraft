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

#ifndef SVSCRAFT_MUSICTIME_P_H
#define SVSCRAFT_MUSICTIME_P_H

#include <QPointer>

#include <SVSCraftCore/MusicTime.h>

namespace SVS {

    class MusicTimelinePrivate;

    struct MusicTimeCache {
        MusicTimeCache() : measure(-1), beat(-1), tick(-1), msec(-1) {
        }

        inline bool isMbtNull() const {
            return measure < 0;
        }

        inline bool isMsecNull() const {
            return msec < 0;
        }

        inline void clearMbt() {
            measure = -1;
        }

        inline void clearMsec() {
            msec = -1;
        }

        int measure;
        int beat;
        int tick;
        double msec;
    };

    class PersistentMusicTimeData {
    public:
        PersistentMusicTimeData(const MusicTimeline *timeline, const MusicTimelinePrivate *td, int totalTick);
        ~PersistentMusicTimeData();

        QPointer<MusicTimeline> timeline;
        const MusicTimelinePrivate *td;

        int totalTick;
        MusicTimeCache cache;

        void ensureMbtCached();
        void ensureMsecCached();

        inline bool isValid() const {
            return timeline && totalTick >= 0;
        }
    };

}

#endif // SVSCRAFT_MUSICTIME_P_H
