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

#include "WaveformMipmap.h"
#include "WaveformMipmap_p.h"

#include <algorithm>
#include <type_traits>
#include <limits>

#include <QDataStream>
#include <QtEndian>

namespace SVS {
    WaveformMipmap::WaveformMipmap() : d(new WaveformMipmapData) {
    }
    WaveformMipmap::WaveformMipmap(const WaveformMipmap &other) = default;
    WaveformMipmap &WaveformMipmap::operator=(const WaveformMipmap &other) = default;
    WaveformMipmap::WaveformMipmap(WaveformMipmap &&other) noexcept = default;
    WaveformMipmap &WaveformMipmap::operator=(WaveformMipmap &&other) noexcept = default;
    WaveformMipmap::WaveformMipmap(qsizetype size, Level level, SampleType sampleType,bool useRms) : d(new WaveformMipmapData) {
        d->level = level;
        d->sampleType = sampleType;
        d->useRms = useRms;
        d->isValid = true;
        resize(size);
    }
    WaveformMipmap::~WaveformMipmap() = default;
    bool WaveformMipmap::isValid() const {
        return d->isValid;
    }
    qsizetype WaveformMipmap::size() const {
        return d->size;
    }
    void WaveformMipmap::resize(qsizetype size) {
        d->size = size;
        int factor = sampleType() == Int16 ? 2 : 1;
        d->max4096.resize((size + 4095) / 4096 * factor);
        d->min4096.resize((size + 4095) / 4096 * factor);
        if (useRms()) {
            d->rms4096.resize((size + 4095) / 4096 * factor);
        }
        d->max256.resize((size + 255) / 256 * factor);
        d->min256.resize((size + 255) / 256 * factor);
        if (useRms()) {
            d->rms256.resize((size + 255) / 256 * factor);
        }
        d->max16.resize((size + 15) / 16 * factor);
        d->min16.resize((size + 15) / 16 * factor);
        if (useRms()) {
            d->rms16.resize((size + 15) / 16 * factor);
        }
        if (level() == Original) {
            d->originalData.resize(size * factor);
        }
    }
    WaveformMipmap::Level WaveformMipmap::level() const {
        return d->level;
    }
    WaveformMipmap::SampleType WaveformMipmap::sampleType() const {
        return d->sampleType;
    }
    bool WaveformMipmap::useRms() const {
        return d->useRms;
    }


    template <typename T, typename S>
    static inline S convertSample(T value) {
        static_assert(std::is_arithmetic_v<T> && std::is_integral_v<S> && std::is_signed_v<S>);
        if constexpr (std::is_integral_v<T>) {
            if constexpr (sizeof(T) < sizeof(S)) {
                return value << ((sizeof(S) - sizeof(T)) * 8);
            } else {
                return value >> ((sizeof(T) - sizeof(S)) * 8);
            }
        } else {
            return static_cast<S>(std::floor(value * (std::numeric_limits<S>::max() + .49999999999999)));
        }
    }
    template <typename T>
    static inline double normalizeSample(T value) {
        static_assert(std::is_arithmetic_v<T>);
        if constexpr (std::is_floating_point_v<T>) {
            return value;
        } else {
            return qMax(-1.0, 1.0 * value / std::numeric_limits<T>::max());
        }
    }
    template <typename T, typename S>
    static inline void loadMipmap16FromOriginal(WaveformMipmapData *d, const T *maxOriginal, const T *minOriginal, const T *rmsOriginal, qsizetype offset, qsizetype length, qsizetype destination, qsizetype restrictedBase, qsizetype restrictedEnd, S *max16, S *min16, S *rms16) {
        auto alignedOffset = offset / 16 * 16;
        auto alignedLength = (offset + length + 15) / 16 * 16 - alignedOffset;
        for (auto i = alignedOffset / 16; i < alignedLength / 16; i++) {
            auto mipmapIndex = i - alignedOffset / 16 + destination;
            auto blockOffset = qMax(restrictedBase, i * 16);
            auto blockLength = qMin(restrictedEnd, (i + 1) * 16) - blockOffset;
            min16[mipmapIndex] = convertSample<T, S>(*std::min_element(minOriginal + blockOffset, minOriginal + blockOffset + blockLength));
            max16[mipmapIndex] = convertSample<T, S>(*std::max_element(maxOriginal + blockOffset, maxOriginal + blockOffset + blockLength));
            if (d->useRms) {
                double rms = std::sqrt(std::accumulate(rmsOriginal + blockOffset, rmsOriginal + blockOffset + blockLength, 0.0, [](double s, auto v) {
                    return s + normalizeSample(v) * normalizeSample(v);
                }) / blockLength);
                rms16[mipmapIndex] = convertSample<double, S>(rms);
            }
        }
    }
    template <typename T, typename S>
    static inline void loadMipmap16FromOriginal(WaveformMipmapData *d, const T *original, qsizetype offset, qsizetype length, qsizetype destination, qsizetype restrictedBase, qsizetype restrictedEnd, S *max16, S *min16, S *rms16) {
        loadMipmap16FromOriginal(d, original, original, original, offset, length, destination, restrictedBase, restrictedEnd, max16, min16, rms16);
    }
    template <typename T>
    static inline void loadImpl(WaveformMipmapData *d, const T *data, qsizetype offset, qsizetype length, qsizetype destination) {
        auto offset16 = destination / 16 * 16;
        auto length16 = (destination + length + 15) / 16 - offset16;
        auto offset256 = offset16 / 16 * 16;
        auto length256 = (offset16 + length16 + 15) / 16 - offset256;
        auto offset4096 = offset256 / 16 * 16;
        if (d->level == WaveformMipmap::Original) {
            if (d->sampleType == WaveformMipmap::Int8) {
                std::transform(data + offset, data + offset + length, d->originalData.data() + destination, [](T value) {
                    return convertSample<T, qint8>(value);
                });
                loadMipmap16FromOriginal(d, d->originalData.data(), destination, length, offset16,
                    0, d->originalData.size(),
                    d->max16.data(), d->min16.data(), d->rms16.data());
            } else {
                std::transform(data + offset, data + offset + length, reinterpret_cast<qint16 *>(d->originalData.data()) + destination, [](T value) {
                    return convertSample<T, qint16>(value);
                });
                loadMipmap16FromOriginal(d, reinterpret_cast<qint16 *>(d->originalData.data()), destination, length, offset16,
                    0, d->originalData.size() / 2,
                    reinterpret_cast<qint16 *>(d->max16.data()), reinterpret_cast<qint16 *>(d->max16.data()), reinterpret_cast<qint16 *>(d->max16.data()));
            }
        } else if (d->level == WaveformMipmap::Downscale) {
            if (d->sampleType == WaveformMipmap::Int8) {
                loadMipmap16FromOriginal(d, data, offset, length, destination / 16 * 16,
                    offset, offset + length,
                    d->max16.data(), d->min16.data(), d->rms16.data());
            } else {
                loadMipmap16FromOriginal(d, data, offset, length, destination / 16 * 16,
                    offset, offset + length,
                    reinterpret_cast<qint16 *>(d->max16.data()), reinterpret_cast<qint16 *>(d->min16.data()), reinterpret_cast<qint16 *>(d->rms16.data()));
            }
        }
        if (d->sampleType == WaveformMipmap::Int8) {
            loadMipmap16FromOriginal(d, d->max16.data(), d->min16.data(), d->rms16.data(), offset16, length16, offset256,
                    0, d->max16.size(),
                    d->max256.data(), d->min256.data(), d->rms256.data());
            loadMipmap16FromOriginal(d, d->max256.data(), d->min256.data(), d->rms256.data(), offset256, length256, offset4096,
                0, d->max256.size(),
                d->max4096.data(), d->min4096.data(), d->rms4096.data());
        } else {
            loadMipmap16FromOriginal(d, reinterpret_cast<qint16 *>(d->max16.data()), reinterpret_cast<qint16 *>(d->min16.data()), reinterpret_cast<qint16 *>(d->rms16.data()), offset16, length16, offset256,
                    0, d->max16.size() / 2,
                    reinterpret_cast<qint16 *>(d->max256.data()), reinterpret_cast<qint16 *>(d->min256.data()), reinterpret_cast<qint16 *>(d->rms256.data()));
            loadMipmap16FromOriginal(d, reinterpret_cast<qint16 *>(d->max256.data()), reinterpret_cast<qint16 *>(d->min256.data()), reinterpret_cast<qint16 *>(d->rms256.data()), offset256, length256, offset4096,
                0, d->max256.size() / 2,
                reinterpret_cast<qint16 *>(d->max4096.data()), reinterpret_cast<qint16 *>(d->min4096.data()), reinterpret_cast<qint16 *>(d->rms4096.data()));
        }
    }


    void WaveformMipmap::load(const qint8 *data, qsizetype offset, qsizetype length, qsizetype destination) {
        loadImpl(d, data, offset, length, destination);
    }
    void WaveformMipmap::load(const qint16 *data, qsizetype offset, qsizetype length, qsizetype destination) {
        loadImpl(d, data, offset, length, destination);
    }
    void WaveformMipmap::load(const qint32 *data, qsizetype offset, qsizetype length, qsizetype destination) {
        loadImpl(d, data, offset, length, destination);
    }
    void WaveformMipmap::load(const qint64 *data, qsizetype offset, qsizetype length, qsizetype destination) {
        loadImpl(d, data, offset, length, destination);
    }
    void WaveformMipmap::load(const float *data, qsizetype offset, qsizetype length, qsizetype destination) {
        loadImpl(d, data, offset, length, destination);
    }
    void WaveformMipmap::load(const double *data, qsizetype offset, qsizetype length, qsizetype destination) {
        loadImpl(d, data, offset, length, destination);
    }

    static inline int getSampleFrom(const WaveformMipmapData *d, const qint8 *data, qsizetype offset) {
        if (d->sampleType == WaveformMipmap::Int8) {
            return data[offset];
        } else {
            return reinterpret_cast<const qint16 *>(data)[offset];
        }
    }
    QPair<int, int> WaveformMipmap::peak(qsizetype offset, qsizetype length) const {
        if (offset >= d->size)
            return {0, 0};
        if (offset + length > d->size)
            length = d->size - offset;
        QPair<int, int> ret(std::numeric_limits<int>::max(), std::numeric_limits<int>::min());
        auto initialIndex = offset;
        auto endIndex = offset + length;
        if (d->level == Downscale) {
            initialIndex = initialIndex / 16 * 16;
            endIndex = qMin(d->max16.size() * 16, (endIndex + 15) / 16 * 16);
        }
        for (auto i = initialIndex; i < endIndex;) {
            if (i % 4096 && endIndex - i > 4096) {
                ret.first = qMin(ret.first, getSampleFrom(d, d->min4096.data(), i / 4096));
                ret.second = qMax(ret.second, getSampleFrom(d, d->max4096.data(), i / 4096));
                i += 4096;
            } else if (i % 256 && endIndex - i > 256) {
                ret.first = qMin(ret.first, getSampleFrom(d, d->min256.data(), i / 256));
                ret.second = qMax(ret.second, getSampleFrom(d, d->max256.data(), i / 256));
                i += 256;
            } else if (d->level == Downscale || i % 16 && endIndex - i > 16) {
                ret.first = qMin(ret.first, getSampleFrom(d, d->min16.data(), i / 16));
                ret.second = qMax(ret.second, getSampleFrom(d, d->max16.data(), i / 16));
                i += 16;
            } else if (d->level == Original) {
                ret.first = qMin(ret.first, getSampleFrom(d, d->originalData.data(), i));
                ret.second = qMax(ret.second, getSampleFrom(d, d->originalData.data(), i));
                i += 1;
            }
        }
        if (ret.first > ret.second)
            ret = {0, 0};
        return ret;
    }
    static inline double getSampleNormalized(const WaveformMipmapData *d, const qint8 *data, qsizetype offset) {
        if (d->sampleType == WaveformMipmap::Int8) {
            return normalizeSample(data[offset]);
        } else {
            return normalizeSample(reinterpret_cast<const qint16 *>(data)[offset]);
        }
    }
    double WaveformMipmap::rms(qsizetype offset, qsizetype length) const {
        if (!useRms())
            return 0;
        if (offset >= d->size)
            return 0;
        if (offset + length > d->size)
            length = d->size - offset;
        double ret = 0;
        int n = 0;
        auto initialIndex = offset;
        auto endIndex = offset + length;
        if (d->level == Downscale) {
            initialIndex = initialIndex / 16 * 16;
            endIndex = qMin(d->max16.size() * 16, (endIndex + 15) / 16 * 16);
        }
        for (auto i = initialIndex; i < endIndex;) {
            if (i % 4096 && endIndex - i > 4096) {
                auto x = getSampleNormalized(d, d->rms4096.data(), i / 4096);
                ret += x * x * 4096;
                i += 4096;
                n += 4096;
            } else if (i % 256 && endIndex - i > 256) {
                auto x = getSampleNormalized(d, d->rms256.data(), i / 256);
                ret += x * x * 256;
                i += 256;
                n += 256;
            } else if (d->level == Downscale || i % 16 && endIndex - i > 16) {
                auto x = getSampleNormalized(d, d->rms16.data(), i / 16);
                ret += x * x * 16;
                i += 16;
                n += 16;
            } else if (d->level == Original) {
                auto x = getSampleNormalized(d, d->originalData.data(), i);
                ret += x * x;
                i += 1;
                n += 1;
            }
        }
        ret /= n;
        return std::sqrt(ret);
    }
    int WaveformMipmap::value(qsizetype offset) const {
        return getSampleFrom(d, d->originalData.data(), offset);
    }
    const qint8 *WaveformMipmap::originalDataAsInt8() const {
        if (d->sampleType != Int8 || d->level == Downscale)
            return nullptr;
        return d->originalData.constData();
    }
    const qint16 *WaveformMipmap::originalDataAsInt16() const {
        if (d->sampleType != Int16 || d->level == Downscale)
            return nullptr;
        return reinterpret_cast<const qint16 *>(d->originalData.constData());
    }


    static inline bool writeData(QDataStream &stream, const QVector<qint8> &data, WaveformMipmap::SampleType sampleType) {
        if (QSysInfo::ByteOrder == QSysInfo::LittleEndian || sampleType == WaveformMipmap::Int8) {
            if (stream.writeRawData(reinterpret_cast<const char *>(data.constData()), data.size()) != data.size())
                return false;
        } else {
            for (qsizetype i = 0; i < data.size() / 2; i++) {
                if (stream.writeRawData(reinterpret_cast<const char *>(data.constData() + (i * 2 + 1)), 1) != 1)
                    return false;
                if (stream.writeRawData(reinterpret_cast<const char *>(data.constData() + (i * 2)), 1) != 1)
                    return false;
            }
        }
        return true;
    }
#define WRITE_DATA(...) \
    if (!writeData(__VA_ARGS__)) { \
        stream.abortTransaction(); \
        return stream; \
    }
    static inline bool readData(QDataStream &stream, QVector<qint8> &data, WaveformMipmap::SampleType sampleType) {
        if (QSysInfo::ByteOrder == QSysInfo::LittleEndian || sampleType == WaveformMipmap::Int8) {
            if (stream.readRawData(reinterpret_cast<char *>(data.data()), data.size()) != data.size())
                return false;
        } else {
            for (qsizetype i = 0; i < data.size() / 2; i++) {
                if (stream.readRawData(reinterpret_cast<char *>(data.data() + (i * 2 + 1)), 1) != 1)
                    return false;
                if (stream.readRawData(reinterpret_cast<char *>(data.data() + (i * 2)), 1) != 1)
                    return false;
            }
        }
        return true;
    }
#define READ_DATA(...) \
    if (!readData(__VA_ARGS__)) { \
        stream.abortTransaction(); \
        waveformMipmap = {}; \
        return stream; \
    }

#define MAGIC {'S', 'V', 'S', ':', ':', 'W', 'a', 'v', 'e', 'f', 'o', 'r', 'm', 'M', 'i', 'p', 'm', 'a', 'p', 'C', 'R', 'S'}
    QDataStream &operator<<(QDataStream &stream, const WaveformMipmap &waveformMipmap) {
        if (!waveformMipmap.isValid())
            return stream;
        stream.startTransaction();
        WaveformMipmapData::HeaderBlock headerBlock = {
            MAGIC,
            1,
            waveformMipmap.useRms(),
            qToLittleEndian(static_cast<qint64>(waveformMipmap.size())),
            qToLittleEndian(static_cast<qint32>(waveformMipmap.level())),
            qToLittleEndian(static_cast<qint32>(waveformMipmap.sampleType())),
        };
        if (stream.writeRawData(reinterpret_cast<const char *>(&headerBlock), sizeof(WaveformMipmapData::HeaderBlock)) != sizeof(WaveformMipmapData::HeaderBlock)) {
            stream.abortTransaction();
            return stream;
        }
        WRITE_DATA(stream, waveformMipmap.d->max4096, waveformMipmap.sampleType());
        WRITE_DATA(stream, waveformMipmap.d->min4096, waveformMipmap.sampleType());
        if (waveformMipmap.useRms()) {
            WRITE_DATA(stream, waveformMipmap.d->rms4096, waveformMipmap.sampleType());
        }
        WRITE_DATA(stream, waveformMipmap.d->max256, waveformMipmap.sampleType());
        WRITE_DATA(stream, waveformMipmap.d->min256, waveformMipmap.sampleType());
        if (waveformMipmap.useRms()) {
            WRITE_DATA(stream, waveformMipmap.d->rms256, waveformMipmap.sampleType());
        }
        WRITE_DATA(stream, waveformMipmap.d->max16, waveformMipmap.sampleType());
        WRITE_DATA(stream, waveformMipmap.d->min16, waveformMipmap.sampleType());
        if (waveformMipmap.useRms()) {
            WRITE_DATA(stream, waveformMipmap.d->rms16, waveformMipmap.sampleType());
        }
        if (waveformMipmap.level() == WaveformMipmap::Original) {
            WRITE_DATA(stream, waveformMipmap.d->originalData, waveformMipmap.sampleType());
        }
        stream.commitTransaction();
        return stream;
    }
    QDataStream &operator>>(QDataStream &stream, WaveformMipmap &waveformMipmap) {
        stream.startTransaction();
        WaveformMipmapData::HeaderBlock headerBlock;
        if (stream.readRawData(reinterpret_cast<char *>(&headerBlock), sizeof(WaveformMipmapData::HeaderBlock)) != sizeof(WaveformMipmapData::HeaderBlock)) {
            stream.abortTransaction();
            waveformMipmap = {};
            return stream;
        }
        static constexpr char MAGIC_[] = MAGIC;
        if (std::memcmp(headerBlock.magic, MAGIC_, 22) != 0) {
            stream.abortTransaction();
            waveformMipmap = {};
            return stream;
        }
        if (headerBlock.version != 1) {
            stream.abortTransaction();
            waveformMipmap = {};
            return stream;
        }
        WaveformMipmap o(
            qFromLittleEndian(headerBlock.size),
            static_cast<WaveformMipmap::Level>(qFromLittleEndian(headerBlock.level)),
            static_cast<WaveformMipmap::SampleType>(qFromLittleEndian(headerBlock.sampleType)),
            headerBlock.useRms
        );
        READ_DATA(stream, o.d->max4096, o.sampleType());
        READ_DATA(stream, o.d->min4096, o.sampleType());
        if (o.useRms()) {
            READ_DATA(stream, o.d->rms4096, o.sampleType());
        }
        READ_DATA(stream, o.d->max256, o.sampleType());
        READ_DATA(stream, o.d->min256, o.sampleType());
        if (o.useRms()) {
            READ_DATA(stream, o.d->rms256, o.sampleType());
        }
        READ_DATA(stream, o.d->max16, o.sampleType());
        READ_DATA(stream, o.d->min16, o.sampleType());
        if (o.useRms()) {
            READ_DATA(stream, o.d->rms16, o.sampleType());
        }
        if (o.level() == WaveformMipmap::Original) {
            READ_DATA(stream, o.d->originalData, o.sampleType());
        }
        stream.commitTransaction();
        waveformMipmap = o;
        return stream;
    }

}
