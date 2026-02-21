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

#include "MusicTime.h"
#include "MusicTime_p.h"
#include "MusicTimeline_p.h"

#include <QDebugStateSaver>
#include <QRegularExpression>
#include <QTextStream>

namespace SVS {

    MusicTime MusicTime::fromString(QStringView str, bool *ok) {
        QRegularExpression rx(R"(^\s*(\d*)\s*[:\x{ff1a}]?\s*(\d*)\s*[:\x{ff1a}]?\s*(\d*)\s*$)");
        auto match = rx.matchView(str);
        if (!match.hasMatch()) {
            if (ok)
                *ok = false;
            return {};
        }
        if (ok)
            *ok = true;
        return {match.capturedView(1).isEmpty() ? 0 : (match.capturedView(1).toInt() - 1), match.capturedView(2).isEmpty() ? 0 : (match.capturedView(2).toInt() - 1), match.capturedView(3).toInt()};
    }
    QString MusicTime::toString(int measureWidth, int beatWidth, int tickWidth) const {
        if (!isValid())
            return {};
        QString str;
        QTextStream textStream(&str);
        textStream.setPadChar('0');
        textStream.setFieldAlignment(QTextStream::AlignRight);
        textStream.setFieldWidth(measureWidth);
        textStream << measure() + 1;
        textStream.setFieldWidth(0);
        textStream << ":";
        textStream.setFieldWidth(beatWidth);
        textStream << beat() + 1;
        textStream.setFieldWidth(0);
        textStream << ":";
        textStream.setFieldWidth(tickWidth);
        textStream << tick();
        textStream.flush();
        return str;
    }

    QDebug operator<<(QDebug debug, const MusicTime &time) {
        QDebugStateSaver saver(debug);
        qDebug().noquote() << "MusicTime(" << time.m << ", " << time.b << ", " << time.t << ")";
        return debug;
    }

    PersistentMusicTimeData::PersistentMusicTimeData(const MusicTimeline *timeline, const MusicTimelinePrivate *td,
                                                     int totalTick)
        : timeline(const_cast<MusicTimeline *>(timeline)), td(td), totalTick(totalTick) {
    }

    PersistentMusicTimeData::~PersistentMusicTimeData() {
        if (!timeline)
            return;
        td->mbtCachedMusicTimes.remove(this);
        td->msecCachedMusicTimes.remove(this);
    }

    void PersistentMusicTimeData::ensureMbtCached() {
        if (!cache.isMbtNull())
            return;
        if (!isValid()) {
            cache.measure = cache.beat = cache.tick = -1;
            return;
        }

        auto ret = td->tickToTime(totalTick);
        cache.measure = ret.measure();
        cache.beat = ret.beat();
        cache.tick = ret.tick();
        td->mbtCachedMusicTimes.insert(this);
    }

    void PersistentMusicTimeData::ensureMsecCached() {
        if (!cache.isMsecNull())
            return;
        if (!isValid()) {
            cache.msec = -1;
            return;
        }

        cache.msec = td->tickToMsec(totalTick);
        td->msecCachedMusicTimes.insert(this);
    }

    PersistentMusicTime::PersistentMusicTime() = default;
    PersistentMusicTime::~PersistentMusicTime() = default;
    PersistentMusicTime::PersistentMusicTime(const PersistentMusicTime &other) = default;
    PersistentMusicTime &PersistentMusicTime::operator=(const PersistentMusicTime &other) = default;

    PersistentMusicTime::PersistentMusicTime(PersistentMusicTime &&other) noexcept = default;
    PersistentMusicTime &PersistentMusicTime::operator=(PersistentMusicTime &&other) noexcept = default;

    MusicTimeline *PersistentMusicTime::timeline() const {
        if (!d)
            return nullptr;
        return d->timeline;
    }

    int PersistentMusicTime::measure() const {
        if (!d)
            return 0;
        d->ensureMbtCached();
        return d->cache.measure;
    }

    int PersistentMusicTime::beat() const {
        if (!d)
            return 0;
        d->ensureMbtCached();
        return d->cache.beat;
    }

    int PersistentMusicTime::tick() const {
        if (!d)
            return 0;
        d->ensureMbtCached();
        return d->cache.tick;
    }

    double PersistentMusicTime::millisecond() const {
        if (!d)
            return 0;
        d->ensureMsecCached();
        return d->cache.msec;
    }

    int PersistentMusicTime::totalTick() const {
        if (!d)
            return 0;
        return d->totalTick;
    }
    bool PersistentMusicTime::isValid() const {
        return d && d->isValid();
    }
    PersistentMusicTime PersistentMusicTime::operator+(int t) const {
        if (!d)
            return {};
        return d->timeline->create(0, 0, d->totalTick + t);
    }

    PersistentMusicTime PersistentMusicTime::operator-(int t) const {
        if (!d)
            return {};
        return d->timeline->create(0, 0, d->totalTick - t);
    }

    PersistentMusicTime &PersistentMusicTime::operator+=(int t) {
        return (*this = *this + t);
    }

    PersistentMusicTime &PersistentMusicTime::operator-=(int t) {
        return (*this = *this - t);
    }
    PersistentMusicTime PersistentMusicTime::floorMeasure() const {
        if (!d)
            return {};
        d->ensureMbtCached();
        if (d->cache.beat == 0 && d->cache.tick == 0) {
            return *this;
        }
        return d->timeline->create(d->cache.measure, 0, 0);
    }
    PersistentMusicTime PersistentMusicTime::ceilMeasure() const {
        if (!d)
            return {};
        d->ensureMbtCached();
        if (d->cache.beat == 0 && d->cache.tick == 0) {
            return *this;
        }
        return d->timeline->create(d->cache.measure + 1, 0, 0);
    }
    PersistentMusicTime PersistentMusicTime::roundMeasure() const {
        if (!d)
            return {};
        d->ensureMbtCached();
        if (d->cache.beat == 0 && d->cache.tick == 0) {
            return *this;
        }
        auto timeSignature = d->timeline->timeSignatureAt(d->cache.measure);
        double b = d->cache.beat + (d->cache.tick * timeSignature.denominator() / 4.0) / d->timeline->ticksPerQuarterNote();
        if (b < 0.5 * timeSignature.numerator()) {
            return d->timeline->create(d->cache.measure, 0, 0);
        } else {
            return d->timeline->create(d->cache.measure + 1, 0, 0);
        }
    }
    PersistentMusicTime PersistentMusicTime::floorBeat() const {
        if (!d)
            return {};
        d->ensureMbtCached();
        if (d->cache.tick == 0) {
            return *this;
        }
        return d->timeline->create(d->cache.measure, d->cache.beat, 0);
    }
    PersistentMusicTime PersistentMusicTime::ceilBeat() const {
        if (!d)
            return {};
        d->ensureMbtCached();
        if (d->cache.tick == 0) {
            return *this;
        }
        return d->timeline->create(d->cache.measure, d->cache.beat + 1, 0);
    }
    PersistentMusicTime PersistentMusicTime::roundBeat() const {
        if (!d)
            return {};
        d->ensureMbtCached();
        if (d->cache.tick == 0) {
            return *this;
        }
        auto timeSignature = d->timeline->timeSignatureAt(d->cache.measure);
        if (d->cache.tick < 0.5 * d->timeline->ticksPerQuarterNote() * 4 / timeSignature.denominator()) {
            return d->timeline->create(d->cache.measure, d->cache.beat, 0);
        } else {
            return d->timeline->create(d->cache.measure, d->cache.beat + 1, 0);
        }
    }
    PersistentMusicTime PersistentMusicTime::previousMeasure() const {
        if (!d)
            return {};
        d->ensureMbtCached();
        if (d->cache.beat == 0 && d->cache.tick == 0) {
            return d->timeline->create(d->cache.measure - 1, 0, 0);
        } else {
            return d->timeline->create(d->cache.measure, 0, 0);
        }
    }
    PersistentMusicTime PersistentMusicTime::nextMeasure() const {
        if (!d)
            return {};
        d->ensureMbtCached();
        return d->timeline->create(d->cache.measure + 1, 0, 0);
    }
    PersistentMusicTime PersistentMusicTime::previousBeat() const {
        if (!d)
            return {};
        d->ensureMbtCached();
        if (d->cache.tick == 0) {
            return d->timeline->create(d->cache.measure, d->cache.beat - 1, 0);
        } else {
            return d->timeline->create(d->cache.measure, d->cache.beat, 0);
        }
    }
    PersistentMusicTime PersistentMusicTime::nextBeat() const {
        if (!d)
            return {};
        d->ensureMbtCached();
        return d->timeline->create(d->cache.measure, d->cache.beat + 1, 0);
    }

    QDebug operator<<(QDebug debug, const PersistentMusicTime &mt) {
        QDebugStateSaver saver(debug);
        debug.nospace() << "MusicTime(";
        debug << "tick=" << mt.totalTick() << ", "
              << "mbt="
              << "(" << mt.measure() << ", " << mt.beat() << ", " << mt.tick() << ", " << mt.toString() << "), "
              << "msec=" << mt.millisecond() << ")";
        return debug;
    }

    PersistentMusicTime::PersistentMusicTime(SVS::PersistentMusicTimeData *d) : d(d) {
    }

    MusicTimeOffset MusicTimeOffset::fromString(QStringView str, bool *ok) {
        QRegularExpression rx(R"(^\s*(\d*)\s*[:\x{ff1a}]?\s*(\d*)\s*$)");
        auto match = rx.matchView(str);
        if (!match.hasMatch()) {
            if (ok)
                *ok = false;
            return {};
        }
        if (ok)
            *ok = true;
        return {match.capturedView(1).isEmpty() ? 0 : match.capturedView(1).toInt(), match.capturedView(2).toInt()};
    }

    QString MusicTimeOffset::toString(int quarterNoteWidth, int tickWidth) const {
        if (!isValid())
            return {};
        QString str;
        QTextStream textStream(&str);
        textStream.setPadChar('0');
        textStream.setFieldAlignment(QTextStream::AlignRight);
        textStream.setFieldWidth(quarterNoteWidth);
        textStream << quarterNote();
        textStream.setFieldWidth(0);
        textStream << ":";
        textStream.setFieldWidth(tickWidth);
        textStream << tick();
        textStream.flush();
        return str;
    }

    QDebug operator<<(QDebug debug, const MusicTimeOffset &offset) {
        QDebugStateSaver saver(debug);
        debug.nospace() << "MusicTimeOffset(" << offset.totalTick() << ", " << offset.toString() << ")";
        return debug;
    }

}

#include "moc_MusicTime.cpp"
