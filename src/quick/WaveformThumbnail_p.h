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

#ifndef SVSCRAFT_WAVEFORMTHUMBNAIL_P_H
#define SVSCRAFT_WAVEFORMTHUMBNAIL_P_H

#include <QQuickItem>
#include <QList>
#include <QVariant>

#include <qqmlintegration.h>

namespace SVS {

    class WaveformMipmap;

    struct WaveformThumbnailSection {
        Q_GADGET
        QML_VALUE_TYPE(waveformThumbnailSection)
        Q_PROPERTY(double start MEMBER start)
        Q_PROPERTY(double end MEMBER end)
        Q_PROPERTY(double length MEMBER length)

    public:
        double start{};
        double end{};
        double length{};

        constexpr bool operator==(const WaveformThumbnailSection &other) const {
            return start == other.start && end == other.end && length == other.length;
        }
    };

    class WaveformThumbnailPrivate;

    class WaveformThumbnail : public QQuickItem {
        Q_OBJECT
        QML_ELEMENT
        Q_DECLARE_PRIVATE(WaveformThumbnail)
        Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
        Q_PROPERTY(QColor rmsColor READ rmsColor WRITE setRmsColor NOTIFY rmsColorChanged)
        Q_PROPERTY(double waveformOffset READ waveformOffset WRITE setWaveformOffset NOTIFY waveformOffsetChanged)
        Q_PROPERTY(QVariant waveformSections READ waveformSectionsVariant WRITE setWaveformSectionsVariant NOTIFY waveformSectionsChanged)
        Q_PROPERTY(SVS::WaveformMipmap waveformMipmap READ waveformMipmap WRITE setWaveformMipmap NOTIFY waveformMipmapChanged)

    public:
        explicit WaveformThumbnail(QQuickItem *parent = nullptr);
        ~WaveformThumbnail() override;

        QColor color() const;
        void setColor(const QColor &color);

        QColor rmsColor() const;
        void setRmsColor(const QColor &rmsColor);

        double waveformOffset() const;
        void setWaveformOffset(double waveformOffset);

        QVariant waveformSectionsVariant() const;
        void setWaveformSectionsVariant(const QVariant &waveformSections);
        QList<WaveformThumbnailSection> waveformSections() const;
        void setWaveformSections(const QList<WaveformThumbnailSection> &waveformSections);

        WaveformMipmap waveformMipmap() const;
        void setWaveformMipmap(const WaveformMipmap &waveformMipmap);

    signals:
        void colorChanged();
        void rmsColorChanged();
        void waveformOffsetChanged();
        void waveformSectionsChanged();
        void waveformMipmapChanged();

    protected:
        QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

    private:
        QScopedPointer<WaveformThumbnailPrivate> d_ptr;
    };

}

Q_DECLARE_METATYPE(SVS::WaveformThumbnailSection)

#endif //SVSCRAFT_WAVEFORMTHUMBNAIL_P_H
