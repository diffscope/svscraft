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

#include "LongTime.h"

#include <QDebug>
#include <QRegularExpression>
#include <QTextStream>

namespace SVS {

    static constexpr qint64 min2ms(qint64 t) {
        return t * 60000;
    }

    static constexpr qint64 sec2ms(double t) {
        return t * 1000;
    }

    static constexpr qint64 h2ms(qint64 t) {
        return t * 3600000;
    }


    QString LongTime::toString(int minuteWidth, int secondWidth, int msecWidth) const {
        QString str;
        QTextStream textStream(&str);
        textStream.setPadChar('0');
        textStream.setFieldAlignment(QTextStream::AlignRight);
        if (negative())
            textStream << '-';
        textStream.setFieldWidth(minuteWidth);
        textStream << minute();
        textStream.setFieldWidth(0);
        textStream << ":";
        textStream.setFieldWidth(secondWidth);
        textStream << second();
        textStream.setFieldWidth(0);
        textStream << ".";
        textStream.setFieldWidth(msecWidth);
        textStream << millisecond();
        textStream.flush();
        return str;
    }

    LongTime LongTime::fromString(QStringView s, bool *ok) {
        if (ok)
            *ok = false;
        QRegularExpression rx(
            R"(^\s*(-?)\s*(\d{0,10})\s*([:\x{ff1a}]?)\s*(\d{0,10})\s*([:\x{ff1a}]?)\s*(\d{0,10})\s*[.\x{3002}\x{ff0e}]?\s*(\d{0,10})\s*$)");
        auto match = rx.match(s);
        if (!match.hasMatch()) {
            return {};
        }

        qint64 t = 0;

        auto capMinus = match.capturedView(1);
        auto cap1 = match.capturedView(2);
        auto capColon1 = match.capturedView(3);
        auto cap2 = match.capturedView(4);
        auto capColon2 = match.capturedView(5);
        auto cap3 = match.capturedView(6);
        auto cap4 = match.capturedView(7);
        if (cap4.isEmpty()) {
            if (cap2.isEmpty() && cap3.isEmpty()) {
                t = sec2ms(cap1.toInt());
            } else if (cap3.isEmpty()) {
                t = min2ms(cap1.toInt()) + sec2ms(cap2.toInt());
            } else {
                t = h2ms(cap1.toInt()) + min2ms(cap2.toInt()) + sec2ms(cap3.toInt());
            }
        } else {
            auto msec = sec2ms(("." + cap4.toString()).toDouble());
            if (capColon1.isEmpty() && capColon2.isEmpty()) {
                t = sec2ms(cap1.toInt()) + msec;
            } else if (capColon2.isEmpty()) {
                t = min2ms(cap1.toInt()) + sec2ms(cap2.toInt()) + msec;
            } else {
                t = h2ms(cap1.toInt()) + min2ms(cap2.toInt()) + sec2ms(cap3.toInt()) + msec;
            }
        }
        if (!capMinus.isEmpty())
            t *= -1;
        if (t > std::numeric_limits<int>::max() || t <= std::numeric_limits<int>::min()) {
            return {};
        }
        if (ok)
            *ok = true;
        return LongTime(static_cast<int>(t));
    }

    QDebug operator<<(QDebug debug, const SVS::LongTime &lt) {
        QDebugStateSaver saver(debug);
        debug.noquote().nospace() << "LongTime(" << lt.toString() << ")";
        return debug;
    }

}

uint qHash(const SVS::LongTime &time, size_t seed) {
    return qHash(time.totalMillisecond(), seed);
}
