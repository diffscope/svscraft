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

#ifndef SVSCRAFT_MUSICTIMELINE_H
#define SVSCRAFT_MUSICTIMELINE_H

#include <QObject>

#include <SVSCraftCore/SVSCraftCoreGlobal.h>

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

#endif // SVSCRAFT_MUSICTIMELINE_H
