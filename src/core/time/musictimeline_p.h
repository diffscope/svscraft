#ifndef MUSICTIMELINEPRIVATE_H
#define MUSICTIMELINEPRIVATE_H

#include <QMap>
#include <QSet>

#include <SVSCraftCore/musictimeline.h>
#include <SVSCraftCore/private/musictime_p.h>
#include <SVSCraftCore/musictimesignature.h>

namespace SVS {

    class SVSCRAFT_CORE_EXPORT MusicTimelinePrivate {
    public:
        Q_DECLARE_PUBLIC(MusicTimeline)
        MusicTimelinePrivate();
        virtual ~MusicTimelinePrivate();

        void init();

        MusicTimeline *q_ptr;

        static int resolution;

        QMap<int, MusicTimeSignature> timeSignatureMap; // bar position -> time signature
        QMap<int, int> measureMap;                      // tick position -> bar position
        QMap<int, int> revMeasureMap;                   // bar position -> tick position
        QMap<int, double> tempoMap;                     // tick position -> tempo
        QMap<int, double> msecSumMap;                   // tick position -> msecs in total
        QMap<double, int> revMsecSumMap;                // msecs in total -> tick position

        mutable QSet<PersistentMusicTimeData *> mbtCachedMusicTimes;
        mutable QSet<PersistentMusicTimeData *> msecCachedMusicTimes;

        void updateMeasureMap(int barFrom);
        void updateMsecSumMap(int tickFrom);

        void timeSignatureChanged();
        void tempoChanged();

        int findNearestTickWithTimeSignature(int tick) const;
        double findNearestMsecWithTempo(double msec) const;

        MusicTime tickToTime(int totalTick) const;
        double tickToMsec(int totalTick) const;

        int timeToTick(int measure, int beat, int tick) const;
        int stringToTick(QStringView str, bool *ok) const;
        int msecToTick(double msec) const;
    };

}

#endif // MUSICTIMELINEPRIVATE_H
