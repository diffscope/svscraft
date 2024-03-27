#ifndef MUSICTIME_H
#define MUSICTIME_H

#include <QSharedPointer>

#include <SVSCraftCore/svscraftcoreglobal.h>

namespace SVS {

    class MusicTimeline;

    class MusicTimelinePrivate;

    class PersistentMusicTimeData;

    class MusicTime {
    public:
        constexpr inline MusicTime() noexcept;
        constexpr inline MusicTime(int measure, int beat, int tick);

        constexpr inline int measure() const;

        constexpr inline int beat() const;

        constexpr inline int tick() const;

        constexpr inline bool isValid() const;

        QString toString(int measureWidth = 1, int beatWidth = 1, int tickWidth = 3) const;

    private:
        int m, b, t;

        friend SVSCRAFT_CORE_EXPORT QDebug operator<<(QDebug debug, const MusicTime &time);
    };

    constexpr MusicTime::MusicTime() noexcept : m(0), b(0), t(0) {
    }

    constexpr MusicTime::MusicTime(int measure, int beat, int tick) : m(measure), b(beat), t(tick) {
    }

    constexpr int MusicTime::measure() const {
        return m;
    }

    constexpr int MusicTime::beat() const {
        return b;
    }

    constexpr int MusicTime::tick() const {
        return t;
    }

    constexpr bool MusicTime::isValid() const {
        return m >= 0 && b >= 0 && t >= 0;
    }

    class SVSCRAFT_CORE_EXPORT PersistentMusicTime {
    public:
        PersistentMusicTime();
        ~PersistentMusicTime();

        PersistentMusicTime(const PersistentMusicTime &other);
        PersistentMusicTime &operator=(const PersistentMusicTime &other);

        PersistentMusicTime(PersistentMusicTime &&other) noexcept;
        PersistentMusicTime &operator=(PersistentMusicTime &&other) noexcept {
            swap(other);
            return *this;
        }

        void swap(PersistentMusicTime &other) noexcept {
            qSwap(d, other.d);
        }

    public:
        const MusicTimeline *timeline() const;

        int measure() const;
        int beat() const;
        int tick() const;

        inline MusicTime toTime() const;

        double msec() const;
        int totalTick() const;

    public:
        inline bool operator==(const PersistentMusicTime &other) const;
        inline bool operator!=(const PersistentMusicTime &other) const;
        inline bool operator<(const PersistentMusicTime &other) const;
        inline bool operator>(const PersistentMusicTime &other) const;
        inline bool operator<=(const PersistentMusicTime &other) const;
        inline bool operator>=(const PersistentMusicTime &other) const;

        PersistentMusicTime operator+(int t) const;
        PersistentMusicTime operator-(int t) const;
        PersistentMusicTime &operator+=(int t);
        PersistentMusicTime &operator-=(int t);

        QString toString(int measureWidth = 1, int beatWidth = 1, int tickWidth = 3) const;

        friend SVSCRAFT_CORE_EXPORT QDebug operator<<(QDebug debug, const PersistentMusicTime &mt);

    private:
        explicit PersistentMusicTime(PersistentMusicTimeData *d);

        QSharedPointer<PersistentMusicTimeData> d;

        friend class MusicTimeline;
    };

    MusicTime PersistentMusicTime::toTime() const {
        return {measure(), beat(), tick()};
    }

    inline bool PersistentMusicTime::operator==(const PersistentMusicTime &other) const {
        return totalTick() == other.totalTick();
    }

    inline bool PersistentMusicTime::operator!=(const PersistentMusicTime &other) const {
        return totalTick() != other.totalTick();
    }

    inline bool PersistentMusicTime::operator<(const PersistentMusicTime &other) const {
        return totalTick() < other.totalTick();
    }

    inline bool PersistentMusicTime::operator>(const PersistentMusicTime &other) const {
        return totalTick() > other.totalTick();
    }

    inline bool PersistentMusicTime::operator<=(const PersistentMusicTime &other) const {
        return totalTick() <= other.totalTick();
    }

    inline bool PersistentMusicTime::operator>=(const PersistentMusicTime &other) const {
        return totalTick() >= other.totalTick();
    }

}

Q_DECLARE_METATYPE(SVS::MusicTime)

Q_DECLARE_METATYPE(SVS::PersistentMusicTime)

#endif // MUSICTIME_H
