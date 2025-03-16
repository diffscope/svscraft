#include "MusicPitch.h"

#include <set>

#include <QRegularExpression>

namespace SVS {

    static const char *keyNameStrFlat[12] = {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"};
    static const char *keyNameStrSharp[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    static int natureKeyPitch[7] = {9, 11, 0, 2, 4, 5, 7};

    MusicPitch MusicPitch::fromString(QStringView s, bool *ok) {
        QRegularExpression rx(R"(^\s*([A-G])([b#]*)\s*(\?|\d*)\s*$)");
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

        if (capOctave.isEmpty()) {
            if (ok)
                *ok = false;
            return MusicPitch(static_cast<qint8>(keyName));
        }
        if (capOctave[0] == '?') {
            if (ok)
                *ok = true;
            return {keyName >= 0 ? qint8(keyName % 12) : qint8(12 + keyName % 12), -1};
        }
        int pitch = keyName + 12 * capOctave.toInt();
        if (pitch < 0 || pitch >= 128) {
            if (ok)
                *ok = false;
            return MusicPitch(static_cast<qint8>(qBound(0, pitch, 127)));
        }
        if (ok)
            *ok = true;
        return MusicPitch(qint8(pitch));
    }

    QString MusicPitch::toString(MusicPitch::Accidental accidental) const {
        return keyName(accidental) + (n >= 0 ? QString::number(octave()) : "?");
    }

    QString MusicPitch::keyName(MusicPitch::Accidental accidental) const {
        switch (accidental) {
            case Flat:
                return keyNameStrFlat[key()];
            case Sharp:
                return keyNameStrSharp[key()];
        }
        return {};
    }

    QList<MusicPitch> MusicPitch::matchedNotes() const {
        if (n >= 0)
            return {};
        QList<MusicPitch> list;
        for (int i = key(); i < 128; i += 12) {
            list.append(MusicPitch(qint8(i)));
        }
        return list;
    }

    QList<MusicPitch> MusicPitch::range(MusicPitch a, MusicPitch b) {
        std::set<MusicPitch> pitches;
        if (a.isWildcard() && b.isWildcard()) {
            if (a.key() <= b.key()) {
                for (auto startPitch : a.matchedNotes()) {
                    for (int i = startPitch.pitch(); i < 128; i++) {
                        auto pitch = MusicPitch(qint8(i));
                        pitches.insert(pitch);
                        if (b.isMatched(pitch))
                            break;
                    }
                }
            } else {
                for (auto startPitch : b.matchedNotes()) {
                    for (int i = startPitch.pitch(); i >= 0; i--) {
                        auto pitch = MusicPitch(qint8(i));
                        pitches.insert(pitch);
                        if (a.isMatched(pitch))
                            break;
                    }
                }
            }
        } else if (b.isWildcard()) {
            for (int i = a.pitch(); i < 128; i++) {
                auto pitch = MusicPitch(qint8(i));
                pitches.insert(pitch);
                if (b.isMatched(pitch))
                    break;
            }
        } else if (a.isWildcard()) {
            for (int i = b.pitch(); i >= 0; i--) {
                auto pitch = MusicPitch(qint8(i));
                pitches.insert(pitch);
                if (a.isMatched(pitch))
                    break;
            }
        } else {
            if (a > b)
                qSwap(a, b);
            for (int i = a.pitch(); i <= b.pitch(); i++) {
                pitches.insert(MusicPitch(qint8(i)));
            }
        }
        return QList<MusicPitch>(pitches.cbegin(), pitches.cend());
    }

}