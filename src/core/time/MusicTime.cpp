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

}

#include "moc_MusicTime.cpp"
