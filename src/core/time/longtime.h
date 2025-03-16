#ifndef SVSCRAFT_LONGTIME_H
#define SVSCRAFT_LONGTIME_H

#include <QMetaType>

#include <SVSCraftCore/svscraftcoreglobal.h>

namespace SVS {

    class SVSCRAFT_CORE_EXPORT LongTime {
        Q_GADGET
        Q_PROPERTY(bool negative READ negative CONSTANT)
        Q_PROPERTY(int minute READ minute CONSTANT)
        Q_PROPERTY(int second READ second CONSTANT)
        Q_PROPERTY(int millisecond READ millisecond CONSTANT)
        Q_PROPERTY(int totalMillisecond READ totalMillisecond CONSTANT)
    public:
        constexpr LongTime() : t(0) {
        }
        constexpr explicit LongTime(int msec) : t(msec) {
        }
        constexpr LongTime(int minute, int second, int millisecond, bool negative = false) : LongTime((negative ? -1 : 1) * (60000 * minute + 1000 * second + millisecond)) {
        }

        constexpr int minute() const {
            return qAbs(t) / 60000;
        }
        constexpr int second() const {
            return qAbs(t) % 60000 / 1000;
        }
        constexpr int millisecond() const {
            return qAbs(t) % 1000;
        }
        constexpr bool negative() const {
            return t < 0;
        }
        constexpr int totalMillisecond() const {
            return t;
        }

        Q_INVOKABLE QString toString(int minuteWidth = 1, int secondWidth = 2, int msecWidth = 3) const;
        static LongTime fromString(QStringView s, bool *ok = nullptr);

        constexpr bool operator==(const LongTime &other) const = default;
        constexpr auto operator<=>(const LongTime &) const = default;

        friend SVSCRAFT_CORE_EXPORT QDebug operator<<(QDebug debug, const LongTime &lt);

    private:
        int t;
    };

}

SVSCRAFT_CORE_EXPORT uint qHash(const SVS::LongTime &time, size_t seed);

#endif // SVSCRAFT_LONGTIME_H
