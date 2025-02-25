#ifndef MUSICPITCH_H
#define MUSICPITCH_H

#include <QList>
#include <QMetaType>
#include <QString>

#include <SVSCraftCore/svscraftcoreglobal.h>

namespace SVS {

    class SVSCRAFT_CORE_EXPORT MusicPitch {
        Q_GADGET
        Q_PROPERTY(int key READ key CONSTANT)
        Q_PROPERTY(int octave READ octave CONSTANT)
        Q_PROPERTY(int pitch READ pitch CONSTANT)
        Q_PROPERTY(bool isWildcard READ isWildcard CONSTANT)
        Q_PROPERTY(QList<MusicPitch> matchedNotes READ matchedNotes CONSTANT)
    public:
        enum SpecialValue {
            Wildcard = -1,
        };
        Q_ENUM(SpecialValue)

        enum Accidental {
            Flat,
            Sharp,
        };
        Q_ENUM(Accidental)

        enum Key {
            C, D, E, F, G, A, B
        };
        Q_ENUM(Key)

        constexpr MusicPitch() : n(0) {
        }

        constexpr explicit MusicPitch(qint8 pitch) : n(pitch) {
            Q_ASSERT(pitch >= -12);
        }

        MusicPitch(qint8 key, qint8 octave) : n(qint8(key + octave * 12)) {
            Q_ASSERT(key >= 0 && key < 12 &&
                     (octave == -1 ||
                      (octave >= 0 && key + octave * 12 >= 0 && key + octave * 12 < 128)));
        }

        static MusicPitch fromString(QStringView s, bool *ok = nullptr);
        Q_INVOKABLE QString toString(Accidental accidental) const;

        constexpr qint8 key() const {
            return static_cast<qint8>((n + 12) % 12);
        }

        QString keyName(Accidental accidental) const;

        constexpr qint8 octave() const {
            return static_cast<qint8>((n + 12) / 12 - 1);
        }

        constexpr qint8 pitch() const {
            return n;
        }

        constexpr bool isWildcard() const {
            return n < 0;
        }

        QList<MusicPitch> matchedNotes() const;

        Q_INVOKABLE constexpr bool isMatched(MusicPitch note) const {
            return n < 0 && key() == note.key();
        }

        constexpr auto operator<=>(const MusicPitch &other) const = default;
        constexpr bool operator==(const MusicPitch &other) const = default;

        static QList<MusicPitch> range(MusicPitch a, MusicPitch b);

    private:
        qint8 n;
    };

}

#endif // MUSICPITCH_H
