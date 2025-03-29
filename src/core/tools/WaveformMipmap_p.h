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

#ifndef SVSCRAFT_WAVEFORMMIPMAP_P_H
#define SVSCRAFT_WAVEFORMMIPMAP_P_H

#include <SVSCraftCore/WaveformMipmap.h>

#include <QVector>

namespace SVS {
    class WaveformMipmapData : public QSharedData {
    public:
        qsizetype size{};
        WaveformMipmap::Level level{};
        WaveformMipmap::SampleType sampleType{};
        bool useRms{};
        bool isValid{};

        struct alignas(8) HeaderBlock {
            char magic[22];
            qint8 version;
            bool useRms;
            qint64 size;
            qint32 level;
            qint32 sampleType;
        };

        QVector<qint8> max4096;
        QVector<qint8> min4096;
        QVector<qint8> rms4096;
        QVector<qint8> max256;
        QVector<qint8> min256;
        QVector<qint8> rms256;
        QVector<qint8> max16;
        QVector<qint8> min16;
        QVector<qint8> rms16;
        QVector<qint8> originalData;

    };
}

#endif //SVSCRAFT_WAVEFORMMIPMAP_P_H
