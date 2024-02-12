#include "musicnote.h"

#include <QRegularExpression>

namespace SVS {

    static const char *keyNameStrFlat[12] = {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"};
    static const char *keyNameStrSharp[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    static int natureKeyPitch[7] = {9, 11, 0, 2, 4, 5, 7};

    MusicNote MusicNote::fromString(QStringView s, bool *ok) {
        QRegularExpression rx(R"(^\s*([A-G])([b#]*)\s*(\?|\d+)\s*$)");
        auto match = rx.match(s);
        if (!match.hasMatch()) {
            if (ok)
                *ok = false;
            return {};
        }
        auto capName = match.capturedView(1);
        auto capAccidental = match.capturedView(2);
        auto capOctave = match.capturedView(3);

        int keyName = natureKeyPitch[capName[0].toLatin1() - 'A'];
        keyName -= int(std::count(capAccidental.begin(), capAccidental.end(), 'b'));
        keyName += int(std::count(capAccidental.begin(), capAccidental.end(), '#'));

        if (capOctave[0] == '?') {
            if (ok)
                *ok = true;
            return {keyName >= 0 ? qint8(keyName % 12) : qint8(12 + keyName % 12), -1};
        }
        int pitch = keyName + 12 * capOctave.toInt();
        if (pitch < 0 || pitch >= 128) {
            if (ok)
                *ok = false;
            return {};
        }
        if (ok)
            *ok = true;
        return MusicNote(qint8(pitch));
    }

    QString MusicNote::toString(MusicNote::Accidental accidental) const {
        return keyName(accidental) + (n >= 0 ? QString::number(octave()) : "?");
    }

    QString MusicNote::keyName(MusicNote::Accidental accidental) const {
        switch (accidental) {
            case Flat:
                return keyNameStrFlat[key()];
            case Sharp:
                return keyNameStrSharp[key()];
        }
        return {};
    }

    QList<MusicNote> MusicNote::matchedNotes() const {
        if (n >= 0)
            return {};
        QList<MusicNote> list;
        for (int i = key(); i < 128; i += 12) {
            list.append(MusicNote(qint8(i)));
        }
        return list;
    }
} // SVS