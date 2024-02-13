#ifndef LONGTIME_H
#define LONGTIME_H

#include <QMetaType>
#include <QStringList>

#include <SVSCraftCore/svscraftcoreglobal.h>

namespace SVS {

    class SVSCRAFT_CORE_EXPORT LongTime {
    public:
        constexpr LongTime();
        constexpr explicit LongTime(int msec);
        constexpr LongTime(int minute, int second, int msec);

        constexpr int minute() const;
        constexpr int second() const;
        constexpr int msec() const;
        constexpr int totalMsec() const;

        QString toString(int minuteWidth = 1, int secondWidth = 2, int msecWidth = 3) const;
        static LongTime fromString(QStringView s, bool *ok = nullptr);

        constexpr bool operator==(const LongTime &other) const;
        constexpr bool operator!=(const LongTime &other) const;
        constexpr bool operator<(const LongTime &other) const;
        constexpr bool operator<=(const LongTime &other) const;
        constexpr bool operator>(const LongTime &other) const;
        constexpr bool operator>=(const LongTime &other) const;

        friend SVSCRAFT_CORE_EXPORT QDebug operator<<(QDebug debug, const LongTime &lt);

    private:
        int t;
    };

    constexpr LongTime::LongTime() : t(0) {
    }

    constexpr LongTime::LongTime(int msec) : t(qMax(msec, 0)) {
    }

    constexpr LongTime::LongTime(int minute, int second, int msec) : LongTime(60000 * minute + 1000 * second + msec) {
    }

    constexpr inline int LongTime::minute() const {
        return t / 60000;
    }

    constexpr inline int LongTime::second() const {
        return t % 60000 / 1000;
    }

    constexpr inline int LongTime::msec() const {
        return t % 1000;
    }

    constexpr inline int LongTime::totalMsec() const {
        return t;
    }

    constexpr bool LongTime::operator==(const LongTime &other) const {
        return t == other.t;
    }

    constexpr bool LongTime::operator!=(const LongTime &other) const {
        return t != other.t;
    }

    constexpr bool LongTime::operator<(const LongTime &other) const {
        return t < other.t;
    }

    constexpr bool LongTime::operator<=(const LongTime &other) const {
        return t <= other.t;
    }

    constexpr bool LongTime::operator>(const LongTime &other) const {
        return t > other.t;
    }

    constexpr bool LongTime::operator>=(const LongTime &other) const {
        return t >= other.t;
    }

}

SVSCRAFT_CORE_EXPORT uint qHash(const SVS::LongTime &time, size_t seed);

Q_DECLARE_METATYPE(SVS::LongTime)

#endif // LONGTIME_H
