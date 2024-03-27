#ifndef MUSICTIMELINE_H
#define MUSICTIMELINE_H

#include <QObject>
#include <QSharedPointer>
#include <QVariant>

#include <SVSCraftCore/musictime.h>

namespace SVS {

    class MusicTimeline;

    class SVSCRAFT_CORE_EXPORT MusicTimeSignature {
    public:
        MusicTimeSignature();
        MusicTimeSignature(int numerator, int denominator);

        Q_DECL_CONSTEXPR inline int numerator() const;
        Q_DECL_CONSTEXPR inline int denominator() const;
        Q_DECL_RELAXED_CONSTEXPR inline void setNumerator(int numerator);
        Q_DECL_RELAXED_CONSTEXPR inline void setDenominator(int denominator);

        Q_DECL_CONSTEXPR bool isValid() const;

        Q_DECL_CONSTEXPR int ticksPerBar(int resolution) const;
        Q_DECL_CONSTEXPR int ticksPerBeat(int resolution) const;

        QString toString() const;

        Q_DECL_CONSTEXPR inline bool operator==(const MusicTimeSignature &t) const;
        Q_DECL_CONSTEXPR inline bool operator!=(const MusicTimeSignature &t) const;

        friend SVSCRAFT_CORE_EXPORT QDebug operator<<(QDebug debug, const MusicTimeSignature &t);

    private:
        int num;
        int den;
    };

    Q_DECL_CONSTEXPR int MusicTimeSignature::numerator() const {
        return num;
    }

    Q_DECL_CONSTEXPR int MusicTimeSignature::denominator() const {
        return den;
    }

    Q_DECL_RELAXED_CONSTEXPR void MusicTimeSignature::setNumerator(int numerator) {
        num = numerator;
    }

    Q_DECL_RELAXED_CONSTEXPR void MusicTimeSignature::setDenominator(int denominator) {
        den = denominator;
    }

    Q_DECL_CONSTEXPR bool MusicTimeSignature::operator==(const MusicTimeSignature &t) const {
        return num == t.num && den == t.den;
    }

    Q_DECL_CONSTEXPR bool MusicTimeSignature::operator!=(const MusicTimeSignature &t) const {
        return num != t.num || den != t.den;
    }

    SVSCRAFT_CORE_EXPORT QDataStream &operator<<(QDataStream &out, const MusicTimeSignature &ts);
    SVSCRAFT_CORE_EXPORT QDataStream &operator>>(QDataStream &in, MusicTimeSignature &ts);

    class MusicTimelinePrivate;

    class SVSCRAFT_CORE_EXPORT MusicTimeline : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(MusicTimeline)
    public:
        explicit MusicTimeline(QObject *parent = nullptr);
        ~MusicTimeline();

    public:
        inline void setTimeSignature(int bar, const MusicTimeSignature &timeSignature);
        inline void removeTimeSignature(int bar);
        void setMultipleTimeSignatures(const QList<QPair<int, MusicTimeSignature>> &timeSignatureList);
        void removeMultipleTimeSignatures(const QList<int> &bars);
        QList<QPair<int, MusicTimeSignature>> timeSignatures() const;
        QList<int> barsWithTimeSignature() const;
        MusicTimeSignature timeSignatureAt(int bar) const;
        int nearestTimeSignatureTo(int bar) const;

        inline void setTempo(int tick, double tempo);
        inline void removeTempo(int tick);
        void setMultipleTempos(const QList<QPair<int, double>> &tempos);
        void removeMultipleTempos(const QList<int> &ticks);
        QList<QPair<int, double>> tempos() const;
        QList<int> ticksWithTempo() const;
        double tempoAt(int tick) const;
        int nearestTempoTo(int tick) const;

    public:
        inline PersistentMusicTime create(const MusicTime &time) const;
        PersistentMusicTime create(int measure, int beat, int tick) const;
        PersistentMusicTime create(QStringView str, bool *ok = nullptr) const;
        PersistentMusicTime create(double msec) const;

    signals:
        void timeSignatureChanged();
        void tempoChanged();
        void changed();

    protected:
        MusicTimeline(MusicTimelinePrivate &d, QObject *parent = nullptr);

        QScopedPointer<MusicTimelinePrivate> d_ptr;
    };

    inline void MusicTimeline::setTimeSignature(int bar, const SVS::MusicTimeSignature &timeSignature) {
        setMultipleTimeSignatures({
            {bar, timeSignature}
        });
    }

    inline void MusicTimeline::removeTimeSignature(int bar) {
        removeMultipleTimeSignatures({bar});
    }

    inline void MusicTimeline::setTempo(int tick, double tempo) {
        setMultipleTempos({
            {tick, tempo}
        });
    }

    inline void MusicTimeline::removeTempo(int tick) {
        removeMultipleTempos({tick});
    }

    inline PersistentMusicTime MusicTimeline::create(const MusicTime &time) const {
        return create(time.measure(), time.beat(), time.tick());
    }

}

Q_DECLARE_METATYPE(SVS::MusicTimeSignature)

#endif // MUSICTIMELINE_H