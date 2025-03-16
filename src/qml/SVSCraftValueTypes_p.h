#ifndef SVSCRAFT_SVSCRAFTVALUETYPES_P_H
#define SVSCRAFT_SVSCRAFTVALUETYPES_P_H

#include <qqmlintegration.h>

#include <SVSCraftCore/LongTime.h>
#include <SVSCraftCore/MusicTime.h>
#include <SVSCraftCore/MusicTimeSignature.h>
#include <SVSCraftCore/MusicPitch.h>

namespace SVS {

    struct LongTimeForeign {
        Q_GADGET
        QML_VALUE_TYPE(longTime)
        QML_FOREIGN(LongTime)
        QML_EXTENDED(SVS::LongTimeForeign)
        QML_CONSTRUCTIBLE_VALUE
        Q_PROPERTY(bool negative READ negative CONSTANT)
        Q_PROPERTY(int minute READ minute CONSTANT)
        Q_PROPERTY(int second READ second CONSTANT)
        Q_PROPERTY(int millisecond READ millisecond CONSTANT)
        Q_PROPERTY(int totalMillisecond READ totalMillisecond CONSTANT)

        LongTime v;

    public:
        Q_INVOKABLE constexpr explicit LongTimeForeign(int millisecond) {
            v = LongTime(millisecond);
        }
        Q_INVOKABLE explicit LongTimeForeign(const QString &s) {
            v = LongTime::fromString(s);
        }
        Q_INVOKABLE explicit LongTimeForeign(const QVariantMap &time);

        constexpr int minute() const {
            return v.minute();
        }
        constexpr int second() const {
            return v.second();
        }
        constexpr int millisecond() const {
            return v.millisecond();
        }
        constexpr bool negative() const {
            return v.negative();
        }
        constexpr int totalMillisecond() const {
            return v.totalMillisecond();
        }

        Q_INVOKABLE QString toString(int minuteWidth = 1, int secondWidth = 2, int msecWidth = 3) const {
            return v.toString(minuteWidth, secondWidth, msecWidth);
        }

    };

    struct MusicTimeForeign {
        Q_GADGET
        QML_VALUE_TYPE(musicTime)
        QML_FOREIGN(MusicTime)
        QML_EXTENDED(SVS::MusicTimeForeign)
        QML_CONSTRUCTIBLE_VALUE
        Q_PROPERTY(int measure READ measure CONSTANT)
        Q_PROPERTY(int beat READ beat CONSTANT)
        Q_PROPERTY(int tick READ tick CONSTANT)
        Q_PROPERTY(bool isValid READ isValid CONSTANT)

        MusicTime v;

    public:
        Q_INVOKABLE explicit MusicTimeForeign(const QVariantMap &time);
        Q_INVOKABLE inline explicit MusicTimeForeign(const QString &s) {
            v = MusicTime::fromString(s);
        }

        constexpr inline int measure() const {
            return v.measure();
        }

        constexpr inline int beat() const {
            return v.beat();
        }

        constexpr inline int tick() const {
            return v.tick();
        }

        constexpr inline bool isValid() const {
            return v.isValid();
        }

        Q_INVOKABLE QString toString(int measureWidth = 1, int beatWidth = 1, int tickWidth = 3) const {
            return v.toString(measureWidth, beatWidth, tickWidth);
        }
    };

    struct PersistentMusicTimeForeign {
        Q_GADGET
        QML_VALUE_TYPE(persistentMusicTime)
        QML_FOREIGN(PersistentMusicTime)
    };

    struct MusicTimeSignatureForeign {
        Q_GADGET
        QML_VALUE_TYPE(musicTimeSignature)
        QML_FOREIGN(MusicTimeSignature)
        QML_EXTENDED(SVS::MusicTimeSignatureForeign)
        QML_CONSTRUCTIBLE_VALUE
        Q_PROPERTY(int numerator READ numerator CONSTANT)
        Q_PROPERTY(int denominator READ denominator CONSTANT)
        Q_PROPERTY(bool isValid READ isValid CONSTANT)

        MusicTimeSignature v;
    public:
        Q_INVOKABLE explicit MusicTimeSignatureForeign(const QVariantMap &signature);

        constexpr inline int numerator() const {
            return v.numerator();
        }
        constexpr inline int denominator() const {
            return v.denominator();
        }
        constexpr inline bool isValid() const {
            return v.isValid();
        }

        Q_INVOKABLE inline QString toString() const {
            return v.toString();
        }

        Q_INVOKABLE constexpr int ticksPerBar(int resolution) const {
            return v.ticksPerBar(resolution);
        }

        Q_INVOKABLE constexpr int ticksPerBeat(int resolution) const {
            return v.ticksPerBeat(resolution);
        }

    };

    struct MusicPitchForeign {
        Q_GADGET
        QML_VALUE_TYPE(musicPitch)
        QML_FOREIGN(MusicPitch)
        QML_EXTENDED(SVS::MusicPitchForeign)
        QML_CONSTRUCTIBLE_VALUE
        Q_PROPERTY(int key READ key CONSTANT)
        Q_PROPERTY(int octave READ octave CONSTANT)
        Q_PROPERTY(int pitch READ pitch CONSTANT)
        Q_PROPERTY(bool isWildcard READ isWildcard CONSTANT)
        Q_PROPERTY(QList<MusicPitch> matchedNotes READ matchedNotes CONSTANT)

        MusicPitch v;
    public:
        Q_INVOKABLE constexpr explicit MusicPitchForeign(int pitch) {
            v = MusicPitch(static_cast<qint8>(qBound(0, pitch, 127)));
        }
        Q_INVOKABLE explicit MusicPitchForeign(const QVariantMap &pitch);
        Q_INVOKABLE inline explicit MusicPitchForeign(const QString &s) {
            v = MusicPitch::fromString(s);
        }
        constexpr inline int key() const {
            return v.key();
        }
        constexpr inline int octave() const {
            return v.octave();
        }
        constexpr inline int pitch() const {
            return v.pitch();
        }
        constexpr inline bool isWildcard() const {
            return v.isWildcard();
        }
        inline QList<MusicPitch> matchedNotes() const {
            return v.matchedNotes();
        }
        Q_INVOKABLE constexpr inline bool isMatched(const MusicPitch &note) const {
            return v.isMatched(note);
        }
        Q_INVOKABLE inline QString toString(MusicPitch::Accidental accidental = MusicPitch::Flat) const {
            return v.toString(accidental);
        }

    };

    struct MusicPitchDerived : MusicPitch {
        Q_GADGET
    };

    namespace MusicPitchNamespaceForeign {
        Q_NAMESPACE
        QML_NAMED_ELEMENT(MusicPitch)
        QML_FOREIGN_NAMESPACE(MusicPitchDerived)
    }

}

#endif // SVSCRAFT_SVSCRAFTVALUETYPES_P_H
