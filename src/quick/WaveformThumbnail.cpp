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

#include "WaveformThumbnail_p.h"
#include "WaveformThumbnail_p_p.h"

#include <cmath>

#include <QSGNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>

namespace SVS {
    
    WaveformThumbnail::WaveformThumbnail(QQuickItem *parent) : QQuickItem(parent), d_ptr(new WaveformThumbnailPrivate) {
        Q_D(WaveformThumbnail);
        d->q_ptr = this;
        setFlag(ItemHasContents, true);
        connect(this, &QQuickItem::xChanged, &QQuickItem::update);
        connect(this, &QQuickItem::yChanged, &QQuickItem::update);
        connect(this, &WaveformThumbnail::colorChanged, &QQuickItem::update);
        connect(this, &WaveformThumbnail::rmsColorChanged, &QQuickItem::update);
        connect(this, &WaveformThumbnail::waveformOffsetChanged, &QQuickItem::update);
        connect(this, &WaveformThumbnail::waveformLengthChanged, &QQuickItem::update);
        connect(this, &WaveformThumbnail::waveformMipmapChanged, &QQuickItem::update);

    }
    WaveformThumbnail::~WaveformThumbnail() = default;
    QColor WaveformThumbnail::color() const {
        Q_D(const WaveformThumbnail);
        return d->color;
    }
    void WaveformThumbnail::setColor(const QColor &color) {
        Q_D(WaveformThumbnail);
        if (d->color != color) {
            d->color = color;
            emit colorChanged();
        }
    }
    QColor WaveformThumbnail::rmsColor() const {
        Q_D(const WaveformThumbnail);
        return d->rmsColor;
    }
    void WaveformThumbnail::setRmsColor(const QColor &rmsColor) {
        Q_D(WaveformThumbnail);
        if (d->rmsColor != rmsColor) {
            d->rmsColor = rmsColor;
            emit rmsColorChanged();
        }
    }
    double WaveformThumbnail::waveformOffset() const {
        Q_D(const WaveformThumbnail);
        return d->waveformOffset;
    }
    void WaveformThumbnail::setWaveformOffset(double waveformOffset) {
        Q_D(WaveformThumbnail);
        if (d->waveformOffset != waveformOffset) {
            d->waveformOffset = waveformOffset;
            emit waveformOffsetChanged();
        }
    }
    double WaveformThumbnail::waveformLength() const {
        Q_D(const WaveformThumbnail);
        return d->waveformLength;
    }
    void WaveformThumbnail::setWaveformLength(double waveformLength) {
        Q_D(WaveformThumbnail);
        if (d->waveformLength != waveformLength) {
            d->waveformLength = waveformLength;
            emit waveformLengthChanged();
        }
    }
    WaveformMipmap WaveformThumbnail::waveformMipmap() const {
        Q_D(const WaveformThumbnail);
        return d->waveformMipmap;
    }
    void WaveformThumbnail::setWaveformMipmap(const WaveformMipmap &waveformMipmap) {
        Q_D(WaveformThumbnail);
        d->waveformMipmap = waveformMipmap;
        d->waveformMipmapUpdatePending = true;
        emit waveformMipmapChanged();
    }

    class WaveformThumbnailSGNode : public QSGNode {
    public:
        double m_waveformOffset = -1;
        double m_waveformLength = -1;
        double m_width;
    };

    QSGNode *WaveformThumbnail::updatePaintNode(QSGNode *node_, UpdatePaintNodeData *update_paint_node_data) {
        Q_D(const WaveformThumbnail);
        if (!d->waveformMipmap.isValid()) {
            delete node_;
            return nullptr;
        }
        auto node = static_cast<WaveformThumbnailSGNode *>(node_);
        if (!node) {
            node = new WaveformThumbnailSGNode();
            node->m_width = width();

            auto mipmapNode = new QSGTransformNode;
            mipmapNode->setFlag(QSGNode::OwnedByParent);
            node->appendChildNode(mipmapNode);

            auto peakNode = new QSGGeometryNode;
            peakNode->setFlag(QSGNode::OwnsGeometry);
            peakNode->setFlag(QSGNode::OwnsMaterial);
            peakNode->setFlag(QSGNode::OwnedByParent);
            peakNode->setGeometry(new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), std::ceil(node->m_width) * 2));
            auto peakNodeMaterial = new QSGFlatColorMaterial;
            peakNode->setMaterial(peakNodeMaterial);
            mipmapNode->appendChildNode(peakNode);

            auto rmsNode = new QSGGeometryNode;
            rmsNode->setFlag(QSGNode::OwnsGeometry);
            rmsNode->setFlag(QSGNode::OwnsMaterial);
            rmsNode->setFlag(QSGNode::OwnedByParent);
            rmsNode->setGeometry(new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), std::ceil(node->m_width) * 2));
            auto rmsNodeMaterial = new QSGFlatColorMaterial;
            rmsNode->setMaterial(rmsNodeMaterial);
            mipmapNode->appendChildNode(rmsNode);
        }
        auto mipmapNode = static_cast<QSGTransformNode *>(node->childAtIndex(0));
        auto peakNode = static_cast<QSGGeometryNode *>(mipmapNode->childAtIndex(0));
        auto peakNodeGeometry = peakNode->geometry();
        auto peakNodeMaterial = static_cast<QSGFlatColorMaterial *>(peakNode->material());
        auto rmsNode = static_cast<QSGGeometryNode *>(mipmapNode->childAtIndex(1));
        auto rmsNodeGeometry = rmsNode->geometry();
        auto rmsNodeMaterial = static_cast<QSGFlatColorMaterial *>(rmsNode->material());

        if (d->color != peakNodeMaterial->color()) {
            peakNodeMaterial->setColor(d->color);
            peakNode->markDirty(QSGNode::DirtyMaterial);
        }

        if (d->rmsColor != rmsNodeMaterial->color()) {
            rmsNodeMaterial->setColor(d->rmsColor);
            rmsNode->markDirty(QSGNode::DirtyMaterial);
        }
        QMatrix4x4 matrix;
        matrix.scale(1, height());
        mipmapNode->setMatrix(matrix);

        if (!d->waveformMipmapUpdatePending && d->waveformOffset == node->m_waveformOffset && d->waveformLength == node->m_waveformLength && width() == node->m_width)
            return node;

        if (width() != node->m_width) {
            node->m_width = width();
            peakNodeGeometry->allocate(std::ceil(node->m_width) * 2);
            rmsNodeGeometry->allocate(std::ceil(node->m_width) * 2);
        }

        node->m_waveformOffset = d->waveformOffset;
        node->m_waveformLength = d->waveformLength;
        d->waveformMipmapUpdatePending = false;

        auto waveformLengthPerPixel = d->waveformLength / width();

        for (int i = 0; i < std::ceil(node->m_width); i++) {
            auto waveformBlockOffset = node->m_waveformOffset + i * waveformLengthPerPixel;
            auto waveformBlockLength = waveformLengthPerPixel;
            auto peak = waveformBlockLength < 0.5 ? qMakePair(0, 0) : d->waveformMipmap.peak(std::round(waveformBlockOffset), std::round(waveformBlockLength));
            auto normalizedPeak = qMakePair(
                qMax(-1.0, 1.0 * peak.first / (d->waveformMipmap.sampleType() == WaveformMipmap::Int8 ? 127.0 : 32767.0)),
                qMax(-1.0, 1.0 * peak.second / (d->waveformMipmap.sampleType() == WaveformMipmap::Int8 ? 127.0 : 32767.0))
            );
            peakNodeGeometry->vertexDataAsPoint2D()[i * 2].set(i, (1.0 - normalizedPeak.first) * 0.5);
            peakNodeGeometry->vertexDataAsPoint2D()[i * 2 + 1].set(i, (1.0 - normalizedPeak.second) * 0.5);

            if (d->waveformMipmap.useRms()) {
                auto rms = waveformBlockLength < 0.5 ? 0 : d->waveformMipmap.rms(std::round(waveformBlockOffset), std::round(waveformBlockLength));
                rmsNodeGeometry->vertexDataAsPoint2D()[i * 2].set(i, (1.0 - rms) * 0.5);
                rmsNodeGeometry->vertexDataAsPoint2D()[i * 2 + 1].set(i, (1.0 + rms) * 0.5);
            }

        }
        peakNode->markDirty(QSGNode::DirtyGeometry);
        if (d->waveformMipmap.useRms()) {
            rmsNode->markDirty(QSGNode::DirtyGeometry);
        }

        return node;

    }
}

#include "moc_WaveformThumbnail_p.cpp"