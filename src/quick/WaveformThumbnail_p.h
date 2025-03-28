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

namespace SVS {

    class WaveformMipmap;

    class WaveformThumbnailPrivate;

    class WaveformThumbnail : public QQuickItem {
        Q_OBJECT
        QML_ELEMENT
        Q_DECLARE_PRIVATE(WaveformThumbnail)
        Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
        Q_PROPERTY(QColor rmsColor READ rmsColor WRITE setRmsColor NOTIFY rmsColorChanged)
        Q_PROPERTY(double waveformOffset READ waveformOffset WRITE setWaveformOffset NOTIFY waveformOffsetChanged)
        Q_PROPERTY(double waveformLength READ waveformLength WRITE setWaveformLength NOTIFY waveformLengthChanged)
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

        double waveformLength() const;
        void setWaveformLength(double waveformLength);

        WaveformMipmap waveformMipmap() const;
        void setWaveformMipmap(const WaveformMipmap &waveformMipmap);

    signals:
        void colorChanged();
        void rmsColorChanged();
        void waveformOffsetChanged();
        void waveformLengthChanged();
        void waveformMipmapChanged();

    protected:
        QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

    private:
        QScopedPointer<WaveformThumbnailPrivate> d_ptr;
    };

}

#endif //SVSCRAFT_WAVEFORMTHUMBNAIL_P_H
