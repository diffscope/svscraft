#ifndef MUSICNOTE_H
#define MUSICNOTE_H

#include <SVSCraftCore/svscraftcoreglobal.h>

#include <QString>
#include <QList>

namespace SVS {

    class SVSCRAFT_CORE_EXPORT MusicNote {
    public:
        enum SpecialValue {
            Wildcard = -1,
        };

        enum Accidental {
            Flat,
            Sharp,
        };

        constexpr MusicNote() : n(0) {
        }

        constexpr explicit MusicNote(qint8 pitch) : n(pitch) {
            Q_ASSERT(pitch >= -12);
        }

        MusicNote(qint8 key, qint8 octave) : n(qint8(key + octave * 12)) {
            Q_ASSERT(key >= 0 && key < 12 &&
                     (octave == -1 ||
                      (octave >= 0 && key + octave * 12 >= 0 && key + octave * 12 < 128)));
        }

        static MusicNote fromString(QStringView s, bool *ok = nullptr);
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

        QList<MusicNote> matchedNotes() const;

        constexpr bool isMatched(MusicNote note) const {
            return n < 0 && key() == note.key();
        }

    private:
        qint8 n;
    };

} // SVS

#endif // MUSICNOTE_H
