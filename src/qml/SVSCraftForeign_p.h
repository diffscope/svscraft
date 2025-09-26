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

#ifndef SVSCRAFT_SVSCRAFTFOREIGN_P_H
#define SVSCRAFT_SVSCRAFTFOREIGN_P_H

#include <qqmlintegration.h>
#include <QJSValue>
#include <QVariant>
#include <QWindow>

#include <SVSCraftCore/MusicTimeline.h>
#include <SVSCraftCore/MusicTime.h>
#include <SVSCraftCore/MusicTimeSignature.h>

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

    class EyedropperForeign : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(Eyedropper)
        QML_SINGLETON
    public:
        explicit EyedropperForeign(QObject *parent = nullptr);
        ~EyedropperForeign() override;
        Q_INVOKABLE static QColor pickColor(QWindow *window = nullptr, bool useNativeColorPickerIfAvailable = true);
    };

    class DesktopServicesForeign : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(DesktopServices)
        QML_SINGLETON
        Q_PROPERTY(QString fileManagerName READ fileManagerName CONSTANT)
    public:
        explicit DesktopServicesForeign(QObject *parent = nullptr);
        ~DesktopServicesForeign() override;
        Q_INVOKABLE static bool reveal(const QString &filename);
        static QString fileManagerName();
    };

}

#endif //SVSCRAFT_SVSCRAFTFOREIGN_P_H
