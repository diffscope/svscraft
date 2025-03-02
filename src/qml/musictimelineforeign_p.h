#ifndef MUSICTIMELINEFOREIGN_P_H
#define MUSICTIMELINEFOREIGN_P_H

#include <qqmlintegration.h>
#include <QJSValue>
#include <QVariant>

#include <SVSCraftCore/musictimeline.h>
#include <SVSCraftCore/musictime.h>
#include <SVSCraftCore/musictimesignature.h>

namespace SVS {

    class MusicTimelineExtended : public QObject {
        Q_OBJECT
        Q_PROPERTY(QVariantMap timeSignatures READ timeSignatures)
        Q_PROPERTY(QVariantMap tempi READ tempi)
        Q_PRIVATE_PROPERTY(timeline, int ticksPerQuarterNote READ ticksPerQuarterNote WRITE setTicksPerQuarterNote NOTIFY ticksPerQuarterNoteChanged)

        MusicTimeline *timeline;
    public:
        explicit inline MusicTimelineExtended(QObject *timeline_) : timeline(qobject_cast<MusicTimeline *>(timeline_)) {
            connect(timeline, &MusicTimeline::timeSignaturesChanged, this, &MusicTimelineExtended::timeSignatures);
            connect(timeline, &MusicTimeline::tempiChanged, this, &MusicTimelineExtended::tempiChanged);
            connect(timeline, &MusicTimeline::ticksPerQuarterNoteChanged, this, &MusicTimelineExtended::ticksPerQuarterNoteChanged);
            connect(timeline, &MusicTimeline::changed, this, &MusicTimelineExtended::changed);
        }
        ~MusicTimelineExtended() override = default;

        Q_INVOKABLE void setTimeSignature(int bar, const QJSValue &signature);
        Q_INVOKABLE void setTimeSignatures(const QJSValueList &value);
        Q_INVOKABLE void removeTimeSignatures(const QVariantList &value);
        QVariantMap timeSignatures() const;
        Q_INVOKABLE SVS::MusicTimeSignature timeSignatureAt(int bar) const;
        Q_INVOKABLE int nearestBarWithTimeSignatureTo(int bar) const;

        Q_INVOKABLE void setTempo(int tick, double tempo);
        Q_INVOKABLE void setTempi(const QJSValueList &value);
        Q_INVOKABLE void removeTempi(const QVariantList &value);
        QVariantMap tempi() const;
        Q_INVOKABLE double tempoAt(int tick) const;
        Q_INVOKABLE int nearestTickWithTempoTo(int tick) const;

        Q_INVOKABLE inline SVS::PersistentMusicTime create(const MusicTime &time) const {
            return timeline->create(time);
        }
        Q_INVOKABLE inline SVS::PersistentMusicTime create(int measure, int beat, int tick) const {
            return timeline->create(measure, beat, tick);
        }
        Q_INVOKABLE SVS::PersistentMusicTime create(QStringView str) const {
            return timeline->create(str);
        }
        Q_INVOKABLE inline SVS::PersistentMusicTime create(double msec) const {
            return timeline->create(msec);
        }

    signals:
        void timeSignaturesChanged();
        void tempiChanged();
        void ticksPerQuarterNoteChanged();
        void changed();

    };

    struct MusicTimelineForeign {
        Q_GADGET
        QML_NAMED_ELEMENT(MusicTimeline)
        QML_FOREIGN(MusicTimeline)
        QML_EXTENDED(SVS::MusicTimelineExtended)
    };

}

#endif //MUSICTIMELINEFOREIGN_P_H
