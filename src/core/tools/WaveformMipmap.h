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

#ifndef SVSCRAFT_WAVEFORMMIPMAP_H
#define SVSCRAFT_WAVEFORMMIPMAP_H

#include <SVSCraftCore/SVSCraftCoreGlobal.h>

#include <QSharedDataPointer>
#include <QMetaObject>

namespace SVS {

    class WaveformMipmapData;

    class SVSCRAFT_CORE_EXPORT WaveformMipmap {
        Q_GADGET
        Q_PROPERTY(bool valid READ isValid CONSTANT)
    public:
        enum Level {
            Downscale,
            Original,
        };
        enum SampleType {
            Int8,
            Int16,
        };
        WaveformMipmap();
        WaveformMipmap(const WaveformMipmap &other);
        WaveformMipmap &operator=(const WaveformMipmap &other);
        WaveformMipmap(WaveformMipmap &&other) noexcept;
        WaveformMipmap &operator=(WaveformMipmap &&other) noexcept;
        explicit WaveformMipmap(qsizetype size, Level level, SampleType sampleType, bool useRms);
        ~WaveformMipmap();

        bool isValid() const;

        qsizetype size() const;
        void resize(qsizetype size);

        Level level() const;
        SampleType sampleType() const;
        bool useRms() const;

        void load(const qint8 *data, qsizetype offset, qsizetype length, qsizetype destination);
        void load(const qint16 *data, qsizetype offset, qsizetype length, qsizetype destination);
        void load(const qint32 *data, qsizetype offset, qsizetype length, qsizetype destination);
        void load(const qint64 *data, qsizetype offset, qsizetype length, qsizetype destination);
        void load(const float *data, qsizetype offset, qsizetype length, qsizetype destination);
        void load(const double *data, qsizetype offset, qsizetype length, qsizetype destination);

        QPair<int, int> peak(qsizetype offset, qsizetype length) const;
        double rms(qsizetype offset, qsizetype length) const;
        int value(qsizetype offset) const;
        const qint8 *originalDataAsInt8() const;
        const qint16 *originalDataAsInt16() const;

        SVSCRAFT_CORE_EXPORT friend QDataStream &operator<<(QDataStream &stream, const WaveformMipmap &waveformMipmap);
        SVSCRAFT_CORE_EXPORT friend QDataStream &operator>>(QDataStream &stream, WaveformMipmap &waveformMipmap);

    private:
        QSharedDataPointer<WaveformMipmapData> d;

    };

}

#endif //SVSCRAFT_WAVEFORMMIPMAP_H
