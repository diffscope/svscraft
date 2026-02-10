/******************************************************************************
 * Copyright (c) 2025 OpenVPI                                                 *
 *                                                                            *
 * This file is part of SVSCraft                                              *
 *                                                                            *
 * SVSCraft is free software: you can redistribute it and/or modify it under  *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License, or (at your     *
 * option) any later version.                                                 *
 *                                                                            *
 * SVSCraft is distributed in the hope that it will be useful, but WITHOUT    *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public        *
 * License for more details.                                                  *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with SVSCraft. If not, see <https://www.gnu.org/licenses/>.          *
 ******************************************************************************/

#include "MusicPitch.h"

#include <set>

#include <QRegularExpression>

namespace SVS {

    static const char *keyNameStrFlat[12] = {"C", "D\u266d", "D", "E\u266d", "E", "F", "G\u266d", "G", "A\u266d", "A", "B\u266d", "B"};
    static const char *keyNameStrSharp[12] = {"C", "C\u266f", "D", "D\u266f", "E", "F", "F\u266f", "G", "G\u266f", "A", "A\u266f", "B"};
    static int natureKeyPitch[7] = {9, 11, 0, 2, 4, 5, 7};

    MusicPitch MusicPitch::fromString(QStringView s, bool *ok) {
        QRegularExpression rx("^\\s*([A-G])([b#\u266d\u266f]*)\\s*(\\?|\uff1f|\\d*)\\s*$");
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
        keyName -= int(std::ranges::count(capAccidental, 'b') + std::ranges::count(capAccidental, QChar(L'\u266d')));
        keyName += int(std::ranges::count(capAccidental, '#') + std::ranges::count(capAccidental, QChar(L'\u266f')));

        if (capOctave.isEmpty()) {
            if (ok)
                *ok = false;
            return MusicPitch(static_cast<qint8>(keyName));
        }
        if (capOctave[0] == '?' || capOctave[0] == QChar(L'\uff1f')) {
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
