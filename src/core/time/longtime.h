#ifndef LONGTIME_H
#define LONGTIME_H

#include <QMetaType>
#include <QStringList>

#include <SVSCraftCore/svscraftcoreglobal.h>

namespace SVS {

    class SVSCRAFT_CORE_EXPORT LongTime {
    public:
        Q_DECL_CONSTEXPR LongTime();
        Q_DECL_CONSTEXPR explicit LongTime(int msec);
        Q_DECL_CONSTEXPR LongTime(int minute, int second, int msec);

        Q_DECL_CONSTEXPR int minute() const;
        Q_DECL_CONSTEXPR int second() const;
        Q_DECL_CONSTEXPR int msec() const;
        Q_DECL_CONSTEXPR int totalMsec() const;

        QString toString(int minuteWidth = 1, int secondWidth = 2, int msecWidth = 3) const;
        static LongTime fromString(QStringView s, bool *ok = nullptr);

        Q_DECL_CONSTEXPR bool operator==(const LongTime &other) const;
        Q_DECL_CONSTEXPR bool operator!=(const LongTime &other) const;
        Q_DECL_CONSTEXPR bool operator<(const LongTime &other) const;
        Q_DECL_CONSTEXPR bool operator<=(const LongTime &other) const;
        Q_DECL_CONSTEXPR bool operator>(const LongTime &other) const;
        Q_DECL_CONSTEXPR bool operator>=(const LongTime &other) const;

        friend SVSCRAFT_CORE_EXPORT QDebug operator<<(QDebug debug, const LongTime &lt);

    private:
        int t;
    };

    Q_DECL_CONSTEXPR LongTime::LongTime() : t(0) {
    }

    Q_DECL_CONSTEXPR LongTime::LongTime(int msec) : t(qMax(msec, 0)) {
    }

    Q_DECL_CONSTEXPR LongTime::LongTime(int minute, int second, int msec) : LongTime(60000 * minute + 1000 * second + msec) {
    }

    Q_DECL_CONSTEXPR inline int LongTime::minute() const {
        return t / 60000;
    }

    Q_DECL_CONSTEXPR inline int LongTime::second() const {
        return t % 60000 / 1000;
    }

    Q_DECL_CONSTEXPR inline int LongTime::msec() const {
        return t % 1000;
    }

    Q_DECL_CONSTEXPR inline int LongTime::totalMsec() const {
        return t;
    }

    Q_DECL_CONSTEXPR bool LongTime::operator==(const LongTime &other) const {
        return t == other.t;
    }

    Q_DECL_CONSTEXPR bool LongTime::operator!=(const LongTime &other) const {
        return t != other.t;
    }

    Q_DECL_CONSTEXPR bool LongTime::operator<(const LongTime &other) const {
        return t < other.t;
    }

    Q_DECL_CONSTEXPR bool LongTime::operator<=(const LongTime &other) const {
        return t <= other.t;
    }

    Q_DECL_CONSTEXPR bool LongTime::operator>(const LongTime &other) const {
        return t > other.t;
    }

    Q_DECL_CONSTEXPR bool LongTime::operator>=(const LongTime &other) const {
        return t >= other.t;
    }

}

SVSCRAFT_CORE_EXPORT uint qHash(const SVS::LongTime &time, size_t seed);

Q_DECLARE_METATYPE(SVS::LongTime)

#endif // LONGTIME_H
