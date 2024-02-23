#ifndef MUSICNOTE_H
#define MUSICNOTE_H

#include <QList>
#include <QMetaType>
#include <QString>

#include <SVSCraftCore/svscraftcoreglobal.h>

namespace SVS {

    class SVSCRAFT_CORE_EXPORT MusicPitch {
    public:
        enum SpecialValue {
            Wildcard = -1,
        };

        enum Accidental {
            Flat,
            Sharp,
        };

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
        QString toString(Accidental accidental) const;

        constexpr qint8 key() const {
            return qint8((n + 12) % 12);
        }

        QString keyName(Accidental accidental) const;

        constexpr qint8 octave() const {
            return qint8((n + 12) / 12 - 1);
        }

        constexpr qint8 pitch() const {
            return n;
        }

        constexpr bool isWildcard() const {
            return n < 0;
        }

        QList<MusicPitch> matchedNotes() const;

        constexpr bool isMatched(MusicPitch note) const {
            return n < 0 && key() == note.key();
        }

        constexpr bool operator==(const MusicPitch &rhs) const {
            return n == rhs.n;
        }
        constexpr bool operator!=(const MusicPitch &rhs) const {
            return n != rhs.n;
        }
        constexpr bool operator<(const MusicPitch &rhs) const {
            return n < rhs.n;
        }
        constexpr bool operator>(const MusicPitch &rhs) const {
            return n > rhs.n;
        }
        constexpr bool operator<=(const MusicPitch &rhs) const {
            return n <= rhs.n;
        }
        constexpr bool operator>=(const MusicPitch &rhs) const {
            return n >= rhs.n;
        }

        static QList<MusicPitch> range(MusicPitch a, MusicPitch b);

    private:
        qint8 n;
    };

}

Q_DECLARE_METATYPE(SVS::MusicPitch)

#endif // MUSICNOTE_H
