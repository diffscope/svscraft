#include "longtime.h"

#include <QDebug>
#include <QRegularExpression>
#include <QTextStream>

namespace {
    struct initializer {
        initializer() {
            qRegisterMetaType<SVS::LongTime>();
        }
    } _;
}

namespace SVS {

    static inline int min2ms(int t) {
        return t * 60000;
    }

    static inline int sec2ms(double t) {
        return t * 1000;
    }

    static inline int h2ms(int t) {
        return t * 3600000;
    }

    LongTime::LongTime(int minute, int second, int msec) : LongTime(min2ms(minute) + sec2ms(second) + msec) {
    }

    QString LongTime::toString(int minuteWidth, int secondWidth, int msecWidth) const {
        QString str;
        QTextStream textStream(&str);
        textStream.setPadChar('0');
        textStream.setFieldAlignment(QTextStream::AlignRight);
        textStream.setFieldWidth(minuteWidth);
        textStream << minute();
        textStream.setFieldWidth(0);
        textStream << ":";
        textStream.setFieldWidth(secondWidth);
        textStream << second();
        textStream.setFieldWidth(0);
        textStream << ".";
        textStream.setFieldWidth(msecWidth);
        textStream << msec();
        textStream.flush();
        return str;
    }

    LongTime LongTime::fromString(const QString &s) {
        QRegularExpression rx(
            R"(^\s*(\d*)\s*([:\x{ff1a}]?)\s*(\d*)\s*([:\x{ff1a}]?)\s*(\d*)\s*[.\x{3002}\x{ff0e}]?\s*(\d*)\s*$)");
        auto match = rx.match(s);
        if (!match.hasMatch())
            return {};

        LongTime res;
        auto &t = res.t;

        auto cap1 = match.captured(1);
        auto capColon1 = match.captured(2);
        auto cap2 = match.captured(3);
        auto capColon2 = match.captured(4);
        auto cap3 = match.captured(5);
        auto cap4 = match.captured(6);
        if (cap4.isEmpty()) {
            if (cap2.isEmpty() && cap3.isEmpty()) {
                t = sec2ms(cap1.toInt());
            } else if (cap3.isEmpty()) {
                t = min2ms(cap1.toInt()) + sec2ms(cap2.toInt());
            } else {
                t = h2ms(cap1.toInt()) + min2ms(cap2.toInt()) + sec2ms(cap3.toInt());
            }
        } else {
            if (capColon1.isEmpty() && capColon2.isEmpty()) {
                t = sec2ms((cap1 + '.' + cap4).toDouble());
            } else if (capColon2.isEmpty()) {
                t = min2ms(cap1.toInt()) + sec2ms((cap2 + '.' + cap4).toDouble());
            } else {
                t = h2ms(cap1.toInt()) + min2ms(cap2.toInt()) + sec2ms((cap3 + '.' + cap4).toDouble());
            }
        }
        return res;
    }

    QDebug operator<<(QDebug debug, const SVS::LongTime &lt) {
        QDebugStateSaver saver(debug);
        debug.noquote().nospace() << "LongTime(" << lt.toString() << ")";
        return debug;
    }

}

uint qHash(const SVS::LongTime &time, size_t seed) {
    return qHash(time.totalMsec(), seed);
}
