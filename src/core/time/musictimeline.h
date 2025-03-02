#ifndef MUSICTIMELINE_H
#define MUSICTIMELINE_H

#include <QObject>

#include <SVSCraftCore/svscraftcoreglobal.h>

namespace SVS {

    class MusicTimeSignature;
    class MusicTime;
    class PersistentMusicTime;

    class MusicTimelinePrivate;

    class SVSCRAFT_CORE_EXPORT MusicTimeline : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(MusicTimeline)
    public:
        explicit MusicTimeline(QObject *parent = nullptr);
        explicit MusicTimeline(int resolution, QObject *parent = nullptr);
        ~MusicTimeline() override;

        void setTimeSignature(int bar, const MusicTimeSignature &timeSignature);
        inline void removeTimeSignature(int bar) {
            removeTimeSignatures({bar});
        }
        void setTimeSignatures(const QList<QPair<int, MusicTimeSignature>> &timeSignatureList);
        void removeTimeSignatures(const QList<int> &bars);
        QMap<int, MusicTimeSignature> timeSignatures() const;
        MusicTimeSignature timeSignatureAt(int bar) const;
        int nearestBarWithTimeSignatureTo(int bar) const;

        inline void setTempo(int tick, double tempo) {
            setTempi({{tick, tempo}});
        }
        inline void removeTempo(int tick) {
            removeTempi({tick});
        }
        void setTempi(const QList<QPair<int, double>> &tempos);
        void removeTempi(const QList<int> &ticks);
        QMap<int, double> tempi() const;
        double tempoAt(int tick) const;
        int nearestTickWithTempoTo(int tick) const;

        PersistentMusicTime create(const MusicTime &time) const;
        PersistentMusicTime create(int measure, int beat, int tick) const;
        PersistentMusicTime create(QStringView str, bool *ok = nullptr) const;
        PersistentMusicTime create(double millisecond) const;

        int ticksPerQuarterNote() const;
        void setTicksPerQuarterNote(int ticksPerQuarterNote);

    signals:
        void timeSignaturesChanged();
        void tempiChanged();
        void ticksPerQuarterNoteChanged(int ticksPerQuarterNote);
        void changed();

    protected:
        MusicTimeline(MusicTimelinePrivate &d, QObject *parent = nullptr);

        QScopedPointer<MusicTimelinePrivate> d_ptr;
    };

}

#endif // MUSICTIMELINE_H