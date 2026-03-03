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

#ifndef SVSCRAFT_SVSQMLNAMESPACE_P_H
#define SVSCRAFT_SVSQMLNAMESPACE_P_H

#include <QObject>
#include <qqmlintegration.h>

#include <SVSCraftCore/SVSCraftNamespace.h>

namespace SVS {

    class MusicPitch;
    class MusicTime;
    class MusicTimeOffset;
    class MusicMode;

    class SVSQmlNamespace : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(SVS)
        QML_EXTENDED_NAMESPACE(SVS::SVSCraft)
        QML_SINGLETON
    public:

        Q_INVOKABLE static double decibelToLinearValue(double decibel, double factor = -15);
        Q_INVOKABLE static double linearValueToDecibel(double linearValue, double factor = -15);
        Q_INVOKABLE static double decibelsToGain(double decibels, double minusInfinityDb = -96);
        Q_INVOKABLE static double gainToDecibels(double gain, double minusInfinityDb = -96);
        Q_INVOKABLE static MusicPitch musicPitch(int key);
        Q_INVOKABLE static MusicPitch musicPitch(int key, int octave);
        Q_INVOKABLE static MusicPitch musicPitch(const QString &s);
        Q_INVOKABLE static MusicTime musicTime(int measure, int beat, int tick);
        Q_INVOKABLE static MusicTime musicTime(const QString &s);
        Q_INVOKABLE static MusicTimeOffset musicTimeOffset(int quarterNote, int tick);
        Q_INVOKABLE static MusicTimeOffset musicTimeOffset(int totalTick);
        Q_INVOKABLE static MusicTimeOffset musicTimeOffset(const QString &s);
        Q_INVOKABLE static QVariant getBuiltInMusicModeInfoList();
        Q_INVOKABLE static MusicMode musicMode(int mask);
    };

}

#endif // SVSCRAFT_SVSQMLNAMESPACE_P_H
