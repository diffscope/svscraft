#include "longtime.h"

#include <QDebug>
#include <QRegularExpression>
#include <QTextStream>

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

    LongTime LongTime::fromString(QStringView s, bool *ok) {
        QRegularExpression rx(
            R"(^\s*(\d*)\s*([:\x{ff1a}]?)\s*(\d*)\s*([:\x{ff1a}]?)\s*(\d*)\s*[.\x{3002}\x{ff0e}]?\s*(\d*)\s*$)");
        auto match = rx.match(s);
        if (!match.hasMatch()) {
            if (ok)
                *ok = false;
            return {};
        }

        LongTime res;
        auto &t = res.t;

        auto cap1 = match.capturedView(1);
        auto capColon1 = match.capturedView(2);
        auto cap2 = match.capturedView(3);
        auto capColon2 = match.capturedView(4);
        auto cap3 = match.capturedView(5);
        auto cap4 = match.capturedView(6);
        if (cap4.isEmpty()) {
            if (cap2.isEmpty() && cap3.isEmpty()) {
                t = sec2ms(cap1.toInt());
            } else if (cap3.isEmpty()) {
                t = min2ms(cap1.toInt()) + sec2ms(cap2.toInt());
            } else {
                t = h2ms(cap1.toInt()) + min2ms(cap2.toInt()) + sec2ms(cap3.toInt());
            }
        } else {
            int msec = sec2ms(("." + cap4.toString()).toDouble());
            if (capColon1.isEmpty() && capColon2.isEmpty()) {
                t = sec2ms(cap1.toInt()) + msec;
            } else if (capColon2.isEmpty()) {
                t = min2ms(cap1.toInt()) + sec2ms(cap2.toInt()) + msec;
            } else {
                t = h2ms(cap1.toInt()) + min2ms(cap2.toInt()) + sec2ms(cap3.toInt()) + msec;
            }
        }
        if (ok)
            *ok = true;
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
