#include "musictimeline.h"
#include "musictimeline_p.h"

#include <QDataStream>
#include <QDebug>
#include <QRegularExpression>

namespace SVS {
    
    MusicTimelinePrivate::~MusicTimelinePrivate() {
    }

    void MusicTimelinePrivate::init() {
        timeSignatureMap = {
            {0, {4, 4}}
        };
        tempoMap = {
            {0, 120}
        };
        measureMap = {
            {0, 0}
        };
        revMeasureMap = {
            {0, 0}
        };
        msecSumMap = {
            {0, 0}
        };
        revMsecSumMap = {
            {0, 0}
        };
    }

    void MusicTimelinePrivate::updateMeasureMap(int barFrom) {
        auto iter = timeSignatureMap.lowerBound(barFrom);
        if (iter == timeSignatureMap.begin())
            iter++;
        for (auto it = measureMap.upperBound(revMeasureMap[std::prev(iter).key()]); it != measureMap.end();) {
            revMeasureMap.remove(it.value());
            it = measureMap.erase(it);
        }
        for (; iter != timeSignatureMap.end(); iter++) {
            auto prevMeasure = std::prev(iter).key();
            auto prevTimeSig = std::prev(iter).value();
            auto prevTickPos = revMeasureMap[prevMeasure];
            auto currentTickPos = prevTickPos + (iter.key() - prevMeasure) * prevTimeSig.ticksPerBar(resolution);
            measureMap[currentTickPos] = iter.key();
            revMeasureMap[iter.key()] = currentTickPos;
        }
    }

    void MusicTimelinePrivate::updateMsecSumMap(int tickFrom) {
        auto iter = tempoMap.lowerBound(tickFrom);
        if (iter == tempoMap.begin())
            iter++;
        for (auto it = msecSumMap.upperBound(std::prev(iter).key()); it != msecSumMap.end();) {
            revMsecSumMap.remove(it.value());
            it = msecSumMap.erase(it);
        }
        for (; iter != tempoMap.end(); iter++) {
            auto prevTick = std::prev(iter).key();
            auto prevTempo = std::prev(iter).value();
            auto prevMsec = msecSumMap[prevTick];
            auto currentMsec = prevMsec + (iter.key() - prevTick) * (60.0 * 1000.0) / (resolution * prevTempo);
            msecSumMap[iter.key()] = currentMsec;
            revMsecSumMap[currentMsec] = iter.key();
        }
    }

    void MusicTimelinePrivate::timeSignatureChanged() {
        Q_Q(MusicTimeline);
        for (auto t : qAsConst(mbtCachedMusicTimes)) {
            t->cache.clearMbt();
        }
        mbtCachedMusicTimes.clear();

        emit q->timeSignatureChanged();
        emit q->changed();
    }

    void MusicTimelinePrivate::tempoChanged() {
        Q_Q(MusicTimeline);
        for (auto t : qAsConst(msecCachedMusicTimes)) {
            t->cache.clearMsec();
        }
        msecCachedMusicTimes.clear();

        emit q->tempoChanged();
        emit q->changed();
    }

    int MusicTimelinePrivate::findNearestTickWithTimeSignature(int tick) const {
        if (tick < 0)
            return 0;
        auto it = measureMap.upperBound(tick);
        if (it != measureMap.end()) {
            return std::prev(it).key();
        }
        return measureMap.lastKey();
    }

    double MusicTimelinePrivate::findNearestMsecWithTempo(double msec) const {
        if (msec < 0)
            return 0;
        auto it = revMsecSumMap.upperBound(msec);
        if (it != revMsecSumMap.end()) {
            return std::prev(it).key();
        }
        return revMsecSumMap.lastKey();
    }

    MusicTimeline::MusicTimeline(QObject *parent) : MusicTimeline(*new MusicTimelinePrivate(), parent) {
    }

    MusicTimeline::~MusicTimeline() {
    }
    void MusicTimeline::setTimeSignature(int bar, const MusicTimeSignature &timeSignature) {
        setTimeSignatures({{bar, timeSignature}});
    }
    void MusicTimeline::setTimeSignatures(const QList<QPair<int, MusicTimeSignature>> &timeSignatureList) {
        Q_D(MusicTimeline);

        bool isChanged = false;
        int minPos = std::numeric_limits<int>::max();
        for (const auto &pair : timeSignatureList) {
            Q_ASSERT(pair.first >= 0 && pair.second.isValid());
            auto it = d->timeSignatureMap.find(pair.first);
            if (it == d->timeSignatureMap.end()) {
                d->timeSignatureMap.insert(pair.first, pair.second);
            } else if (it.value() == pair.second) {
                // identical to current time signature
                continue;
            } else {
                it.value() = pair.second;
            }
            isChanged = true;
            minPos = qMin(minPos, pair.first);
        }

        if (isChanged) {
            d->updateMeasureMap(minPos);
            d->timeSignatureChanged();
        }
    }

    void MusicTimeline::removeTimeSignatures(const QList<int> &bars) {
        Q_D(MusicTimeline);

        bool isChanged = false;
        int minPos = std::numeric_limits<int>::max();
        for (auto barPos : bars) {
            Q_ASSERT(barPos > 0);
            if (!d->timeSignatureMap.remove(barPos)) {
                qWarning() << "SVS::MusicTimeline: Bar" << barPos << "does not have time signature.";
                continue;
            }
            isChanged = true;
            minPos = qMin(minPos, barPos);
        }
        if (isChanged) {
            d->updateMeasureMap(minPos);
            d->timeSignatureChanged();
        }
    }

    QList<QPair<int, MusicTimeSignature>> MusicTimeline::timeSignatures() const {
        Q_D(const MusicTimeline);
        QList<QPair<int, MusicTimeSignature>> list;
        for (auto it = d->timeSignatureMap.begin(); it != d->timeSignatureMap.end(); ++it) {
            list.append({it.key(), it.value()});
        }
        return list;
    }

    QList<int> MusicTimeline::barsWithTimeSignature() const {
        Q_D(const MusicTimeline);
        return d->timeSignatureMap.keys();
    }

    MusicTimeSignature MusicTimeline::timeSignatureAt(int bar) const {
        Q_D(const MusicTimeline);
        return d->timeSignatureMap.value(nearestBarWithTimeSignatureTo(bar));
    }

    int MusicTimeline::nearestBarWithTimeSignatureTo(int bar) const {
        Q_D(const MusicTimeline);
        Q_ASSERT(bar >= 0);
        auto it = d->timeSignatureMap.upperBound(bar);
        if (it != d->timeSignatureMap.end()) {
            return std::prev(it).key();
        }
        return d->timeSignatureMap.lastKey();
    }

    void MusicTimeline::setTempi(const QList<QPair<int, double>> &tempos) {
        Q_D(MusicTimeline);

        bool isChanged = false;
        int minPos = std::numeric_limits<int>::max();
        for (const auto &pair : tempos) {
            Q_ASSERT(pair.first >= 0 && pair.second > 0);
            auto it = d->tempoMap.find(pair.first);
            if (it == d->tempoMap.end()) {
                d->tempoMap.insert(pair.first, pair.second);
            } else if (it.value() == pair.second) {
                // identical to current tempo
                continue;
            } else {
                it.value() = pair.second;
            }
            isChanged = true;
            minPos = qMin(minPos, pair.first);
        }
        if (isChanged) {
            d->updateMsecSumMap(minPos);
            d->tempoChanged();
        }
    }

    void MusicTimeline::removeTempi(const QList<int> &ticks) {
        Q_D(MusicTimeline);
        bool isChanged = false;

        int minPos = std::numeric_limits<int>::max();
        for (const auto tickPos : ticks) {
            Q_ASSERT(tickPos > 0);
            if (!d->tempoMap.remove(tickPos)) {
                qWarning() << "SVS::MusicTimeline: Tick" << tickPos << "does not have tempo.";
                continue;
            }
            isChanged = true;
            minPos = qMin(minPos, tickPos);
        }
        if (isChanged) {
            d->updateMsecSumMap(minPos);
            d->tempoChanged();
        }
    }

    QList<QPair<int, double>> MusicTimeline::tempi() const {
        Q_D(const MusicTimeline);
        QList<QPair<int, double>> list;
        for (auto it = d->tempoMap.begin(); it != d->tempoMap.end(); ++it) {
            list.append({it.key(), it.value()});
        }
        return list;
    }

    QList<int> MusicTimeline::ticksWithTempo() const {
        Q_D(const MusicTimeline);
        return d->tempoMap.keys();
    }

    double MusicTimeline::tempoAt(int tick) const {
        Q_D(const MusicTimeline);
        return d->tempoMap.value(nearestTickWithTempoTo(tick));
    }

    int MusicTimeline::nearestTickWithTempoTo(int tick) const {
        Q_D(const MusicTimeline);
        Q_ASSERT(tick >= 0);
        auto it = d->tempoMap.upperBound(tick);
        if (it != d->tempoMap.end()) {
            return std::prev(it).key();
        }
        return d->tempoMap.lastKey();
    }


    MusicTime MusicTimelinePrivate::tickToTime(int totalTick) const {
        int refTick = findNearestTickWithTimeSignature(totalTick);
        int refMeasure = measureMap[refTick];
        MusicTimeSignature timeSig = timeSignatureMap[refMeasure];
        return {
            refMeasure + (totalTick - refTick) / timeSig.ticksPerBar(resolution),
            ((totalTick - refTick) % timeSig.ticksPerBar(resolution)) / timeSig.ticksPerBeat(resolution),
            ((totalTick - refTick) % timeSig.ticksPerBar(resolution)) % timeSig.ticksPerBeat(resolution),
        };
    }

    double MusicTimelinePrivate::tickToMsec(int totalTick) const {
        Q_Q(const MusicTimeline);
        auto refTick = q->nearestTickWithTempoTo(totalTick);
        auto tempo = tempoMap[refTick];
        auto refMsec = msecSumMap[refTick];
        return refMsec + (totalTick - refTick) * (60.0 * 1000.0) / (resolution * tempo);
    }

    int MusicTimelinePrivate::timeToTick(int measure, int beat, int tick) const {
        Q_Q(const MusicTimeline);
        if (measure < 0 || beat < 0 || tick < 0)
            return 0;
        if (measure == 0 && beat == 0) {
            return tick;
        }

        auto timeSig = q->timeSignatureAt(measure);
        auto refMeasure = q->nearestBarWithTimeSignatureTo(measure);
        auto refTick = revMeasureMap[refMeasure];
        tick += refTick + (measure - refMeasure) * timeSig.ticksPerBar(resolution);
        tick += beat * timeSig.ticksPerBeat(resolution);
        return tick;
    }

    int MusicTimelinePrivate::stringToTick(QStringView str, bool *ok) const {
        auto t = MusicTime::fromString(str, ok);
        return timeToTick(t.measure(), t.beat(), t.tick());
    }

    int MusicTimelinePrivate::msecToTick(double msec) const {
        Q_Q(const MusicTimeline);
        if (msec < 0)
            return 0;
        auto refMsec = findNearestMsecWithTempo(msec);
        auto refTick = revMsecSumMap[refMsec];
        auto tempo = tempoMap[refTick];
        auto deltaTick = (int) qRound((msec - refMsec) / (60.0 * 1000.0) * (resolution * tempo));
        return refTick + deltaTick;
    }

    PersistentMusicTime MusicTimeline::create(const MusicTime &time) const {
        return create(time.measure(), time.beat(), time.tick());
    }
    PersistentMusicTime MusicTimeline::create(int measure, int beat, int tick) const {
        Q_D(const MusicTimeline);
        auto container = new PersistentMusicTimeData(this, d, d->timeToTick(measure, beat, tick));
        return PersistentMusicTime(container);
    }

    PersistentMusicTime MusicTimeline::create(QStringView str, bool *ok) const {
        Q_D(const MusicTimeline);
        auto container = new PersistentMusicTimeData(this, d, d->stringToTick(str, ok));
        return PersistentMusicTime(container);
    }

    PersistentMusicTime MusicTimeline::create(double msec) const {
        Q_D(const MusicTimeline);
        auto container = new PersistentMusicTimeData(this, d, d->msecToTick(msec));
        return PersistentMusicTime(container);
    }
    void MusicTimeline::setTicksPerQuarterNote(int ticks) {
        MusicTimelinePrivate::resolution = ticks;
    }
    int MusicTimeline::ticksPerQuarterNote() {
        return MusicTimelinePrivate::resolution;
    }

    int MusicTimelinePrivate::resolution = 480;

    MusicTimelinePrivate::MusicTimelinePrivate() {
        timeSignatureMap = {
            {0, {4, 4}}
        };
        tempoMap = {
            {0, 120}
        };
        measureMap = {
            {0, 0}
        };
        revMeasureMap = {
            {0, 0}
        };
        msecSumMap = {
            {0, 0}
        };
        revMsecSumMap = {
            {0, 0}
        };
    }

    MusicTimeline::MusicTimeline(MusicTimelinePrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
