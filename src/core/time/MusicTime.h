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

#ifndef SVSCRAFT_MUSICTIME_H
#define SVSCRAFT_MUSICTIME_H

#include <QSharedPointer>

#include <SVSCraftCore/SVSCraftCoreGlobal.h>

namespace SVS {

    class MusicTimeline;

    class MusicTimelinePrivate;

    class PersistentMusicTimeData;

    class SVSCRAFT_CORE_EXPORT MusicTime {
        Q_GADGET
        Q_PROPERTY(int measure READ measure CONSTANT)
        Q_PROPERTY(int beat READ beat CONSTANT)
        Q_PROPERTY(int tick READ tick CONSTANT)
        Q_PROPERTY(bool isValid READ isValid CONSTANT)
    public:
        constexpr inline MusicTime() : MusicTime(0, 0, 0) {
        }
        constexpr inline MusicTime(int measure, int beat, int tick) : m(measure), b(beat), t(tick) {
        }

        constexpr inline int measure() const {
            return m;
        }

        constexpr inline int beat() const {
            return b;
        }

        constexpr inline int tick() const {
            return t;
        }

        constexpr inline bool isValid() const {
            return m >= 0 && b >= 0 && t >= 0;
        }

        static MusicTime fromString(QStringView str, bool *ok = nullptr);
        Q_INVOKABLE QString toString(int measureWidth = 1, int beatWidth = 1, int tickWidth = 3) const;

    private:
        int m, b, t;

        friend SVSCRAFT_CORE_EXPORT QDebug operator<<(QDebug debug, const MusicTime &time);
    };

    class SVSCRAFT_CORE_EXPORT PersistentMusicTime {
        Q_GADGET
        Q_PROPERTY(MusicTimeline *timeline READ timeline CONSTANT)
        Q_PROPERTY(int measure READ measure)
        Q_PROPERTY(int beat READ beat)
        Q_PROPERTY(int tick READ tick)
        Q_PROPERTY(MusicTime musicTime READ toTime)
        Q_PROPERTY(double millisecond READ millisecond)
        Q_PROPERTY(int totalTick READ totalTick CONSTANT)
        Q_PROPERTY(bool isValid READ isValid CONSTANT)

    public:
        PersistentMusicTime();
        ~PersistentMusicTime();
        PersistentMusicTime(const PersistentMusicTime &other);
        PersistentMusicTime &operator=(const PersistentMusicTime &other);
        PersistentMusicTime(PersistentMusicTime &&other) noexcept;
        PersistentMusicTime &operator=(PersistentMusicTime &&other) noexcept;

        MusicTimeline *timeline() const;

        int measure() const;
        int beat() const;
        int tick() const;

        inline MusicTime toTime() const {
            return {measure(), beat(), tick()};
        }

        double millisecond() const;
        int totalTick() const;

        bool isValid() const;

        inline bool operator==(const PersistentMusicTime &other) const {
            return totalTick() == other.totalTick();
        }
        inline auto operator<=>(const PersistentMusicTime &other) const {
            return totalTick() <=> other.totalTick();
        }

        PersistentMusicTime operator+(int t) const;
        PersistentMusicTime operator-(int t) const;
        PersistentMusicTime &operator+=(int t);
        PersistentMusicTime &operator-=(int t);

        Q_INVOKABLE inline QString toString(int measureWidth = 1, int beatWidth = 1, int tickWidth = 3) const {
            return toTime().toString(measureWidth, beatWidth, tickWidth);
        }

        friend SVSCRAFT_CORE_EXPORT QDebug operator<<(QDebug debug, const PersistentMusicTime &mt);

    private:
        explicit PersistentMusicTime(PersistentMusicTimeData *d);

        QSharedPointer<PersistentMusicTimeData> d;

        friend class MusicTimeline;
    };

}

#endif // SVSCRAFT_MUSICTIME_H
