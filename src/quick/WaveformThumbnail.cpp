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

#include "WaveformThumbnail.h"
#include "WaveformThumbnail_p.h"

#include <cmath>
#include <limits>

#include <QSGNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QVariant>

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
        connect(this, &WaveformThumbnail::waveformSectionsChanged, &QQuickItem::update);
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
    QVariant WaveformThumbnail::waveformSectionsVariant() const {
        Q_D(const WaveformThumbnail);
        QVariantList list;
        list.reserve(d->waveformSections.size());
        for (const auto &section : d->waveformSections) {
            list.append(QVariantMap{{"start", section.start}, {"end", section.end}, {"length", section.length}});
        }
        return list;
    }
    static inline bool variantToDouble(const QVariant &variant, double *value) {
        bool ok = false;
        const auto number = variant.toDouble(&ok);
        if (ok) {
            *value = number;
        }
        return ok;
    }
    static inline bool variantToWaveformSection(const QVariant &variant, WaveformThumbnailSection *section) {
        if (variant.canConvert<WaveformThumbnailSection>()) {
            *section = variant.value<WaveformThumbnailSection>();
            return true;
        }
        const auto map = variant.toMap();
        if (!map.isEmpty()) {
            return variantToDouble(map.value("start"), &section->start) &&
                variantToDouble(map.value("end"), &section->end) &&
                variantToDouble(map.value("length"), &section->length);
        }
        const auto list = variant.toList();
        if (list.size() >= 3) {
            return variantToDouble(list.at(0), &section->start) &&
                variantToDouble(list.at(1), &section->end) &&
                variantToDouble(list.at(2), &section->length);
        }
        return false;
    }
    void WaveformThumbnail::setWaveformSectionsVariant(const QVariant &waveformSectionsVariant) {
        QList<WaveformThumbnailSection> waveformSections;
        const auto list = waveformSectionsVariant.toList();
        waveformSections.reserve(list.size());
        for (const auto &item : list) {
            WaveformThumbnailSection section;
            if (variantToWaveformSection(item, &section)) {
                waveformSections.append(section);
            }
        }
        setWaveformSections(waveformSections);
    }
    QList<WaveformThumbnailSection> WaveformThumbnail::waveformSections() const {
        Q_D(const WaveformThumbnail);
        return d->waveformSections;
    }
    void WaveformThumbnail::setWaveformSections(const QList<WaveformThumbnailSection> &waveformSections) {
        Q_D(WaveformThumbnail);
        if (d->waveformSections != waveformSections) {
            d->waveformSections = waveformSections;
            emit waveformSectionsChanged();
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
        QList<WaveformThumbnailSection> m_waveformSections;
        double m_width = 0;
    };

    static constexpr int INTERPOLATE_WINDOW = 16;
    static constexpr int INTERPOLATE_MAX = 16;
    static constexpr double SINC[] = {
#       include "sinc.inc"
    };
    static constexpr double sinc(int j, int k) {
        return SINC[k * (INTERPOLATE_WINDOW * 2) + (j + INTERPOLATE_WINDOW - 1)];
    }

    template <typename T>
    static inline double interpolate(T *a, qsizetype size, double x) {
        qsizetype i = std::floor(x);
        int k = static_cast<int>((x - i) * 16);
        if (k == 0) {
            return 1.0 * a[i] / std::numeric_limits<T>::max();
        }
        double ret = 0;
        for (int j = -INTERPOLATE_WINDOW + 1; j <= INTERPOLATE_WINDOW; j++) {
            qsizetype idx = i + j;
            if (idx >= 0 && idx < size) {
                ret += a[idx] * sinc(j, k);
            }
        }
        ret = qMax(-1.0, ret / std::numeric_limits<T>::max());
        return ret;
    }

    static inline double sampleValue(const WaveformMipmap &waveformMipmap, double index) {
        if (waveformMipmap.size() <= 0) {
            return 0.0;
        }
        index = qBound(0.0, index, 1.0 * (waveformMipmap.size() - 1));
        return waveformMipmap.sampleType() == WaveformMipmap::Int8
            ? interpolate(waveformMipmap.originalDataAsInt8(), waveformMipmap.size(), index)
            : interpolate(waveformMipmap.originalDataAsInt16(), waveformMipmap.size(), index);
    }

    static QSGTransformNode *createWaveformSectionNode(const QColor &color, const QColor &rmsColor) {
        auto sectionNode = new QSGTransformNode;
        sectionNode->setFlag(QSGNode::OwnedByParent);

        auto peakNode = new QSGGeometryNode;
        peakNode->setFlag(QSGNode::OwnsGeometry);
        peakNode->setFlag(QSGNode::OwnsMaterial);
        peakNode->setFlag(QSGNode::OwnedByParent);
        auto peakNodeGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        peakNodeGeometry->setDrawingMode(QSGGeometry::DrawTriangleStrip);
        peakNode->setGeometry(peakNodeGeometry);
        auto peakNodeMaterial = new QSGFlatColorMaterial;
        peakNodeMaterial->setColor(color);
        peakNode->setMaterial(peakNodeMaterial);
        sectionNode->appendChildNode(peakNode);

        auto peakLineNode = new QSGGeometryNode;
        peakLineNode->setFlag(QSGNode::OwnsGeometry);
        peakLineNode->setFlag(QSGNode::OwnedByParent);
        auto peakLineNodeGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        peakLineNodeGeometry->setDrawingMode(QSGGeometry::DrawLines);
        peakLineNode->setGeometry(peakLineNodeGeometry);
        peakLineNode->setMaterial(peakNodeMaterial);
        sectionNode->appendChildNode(peakLineNode);

        auto rmsNode = new QSGGeometryNode;
        rmsNode->setFlag(QSGNode::OwnsGeometry);
        rmsNode->setFlag(QSGNode::OwnsMaterial);
        rmsNode->setFlag(QSGNode::OwnedByParent);
        auto rmsNodeGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        rmsNodeGeometry->setDrawingMode(QSGGeometry::DrawTriangleStrip);
        rmsNode->setGeometry(rmsNodeGeometry);
        auto rmsNodeMaterial = new QSGFlatColorMaterial;
        rmsNodeMaterial->setColor(rmsColor);
        rmsNode->setMaterial(rmsNodeMaterial);
        sectionNode->appendChildNode(rmsNode);

        auto lineGraphNode = new QSGGeometryNode;
        lineGraphNode->setFlag(QSGNode::OwnsGeometry);
        lineGraphNode->setFlag(QSGNode::OwnedByParent);
        auto lineGraphNodeGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        lineGraphNodeGeometry->setDrawingMode(QSGGeometry::DrawLineStrip);
        lineGraphNode->setGeometry(lineGraphNodeGeometry);
        lineGraphNode->setMaterial(peakNodeMaterial);
        sectionNode->appendChildNode(lineGraphNode);

        return sectionNode;
    }

    QSGNode *WaveformThumbnail::updatePaintNode(QSGNode *node_, UpdatePaintNodeData *update_paint_node_data) {
        Q_D(const WaveformThumbnail);
        if (!d->waveformMipmap.isValid()) {
            delete node_;
            return nullptr;
        }
        auto node = static_cast<WaveformThumbnailSGNode *>(node_);
        if (!node) {
            node = new WaveformThumbnailSGNode();
        }

        QMatrix4x4 matrix;
        matrix.scale(1, height());

        while (node->childCount() < d->waveformSections.size()) {
            node->appendChildNode(createWaveformSectionNode(d->color, d->rmsColor));
        }
        while (node->childCount() > d->waveformSections.size()) {
            auto child = node->lastChild();
            node->removeChildNode(child);
            delete child;
        }

        for (int i = 0; i < node->childCount(); ++i) {
            auto sectionNode = static_cast<QSGTransformNode *>(node->childAtIndex(i));
            sectionNode->setMatrix(matrix);
            auto peakNode = static_cast<QSGGeometryNode *>(sectionNode->childAtIndex(0));
            auto peakNodeMaterial = static_cast<QSGFlatColorMaterial *>(peakNode->material());
            auto lineGraphNode = static_cast<QSGGeometryNode *>(sectionNode->childAtIndex(3));
            if (d->color != peakNodeMaterial->color()) {
                peakNodeMaterial->setColor(d->color);
                peakNode->markDirty(QSGNode::DirtyMaterial);
                lineGraphNode->markDirty(QSGNode::DirtyMaterial);
            }

            auto rmsNode = static_cast<QSGGeometryNode *>(sectionNode->childAtIndex(2));
            auto rmsNodeMaterial = static_cast<QSGFlatColorMaterial *>(rmsNode->material());
            if (d->rmsColor != rmsNodeMaterial->color()) {
                rmsNodeMaterial->setColor(d->rmsColor);
                rmsNode->markDirty(QSGNode::DirtyMaterial);
            }
        }

        if (!d->waveformMipmapUpdatePending && d->waveformOffset == node->m_waveformOffset && d->waveformSections == node->m_waveformSections && width() == node->m_width)
            return node;

        node->m_width = width();
        node->m_waveformOffset = d->waveformOffset;
        node->m_waveformSections = d->waveformSections;
        d->waveformMipmapUpdatePending = false;

        double sectionOffset = d->waveformOffset;
        for (int sectionIndex = 0; sectionIndex < d->waveformSections.size(); ++sectionIndex) {
            const auto &section = d->waveformSections.at(sectionIndex);
            const auto sectionLength = qMax(0.0, section.length);
            const auto sectionStartX = section.start * width();
            const auto sectionEndX = section.end * width();
            const auto sectionWidth = sectionEndX - sectionStartX;
            const auto sectionPixelCount = qMax(0, static_cast<int>(std::ceil(std::abs(sectionWidth))));
            const auto sectionNode = static_cast<QSGTransformNode *>(node->childAtIndex(sectionIndex));
            auto peakNode = static_cast<QSGGeometryNode *>(sectionNode->childAtIndex(0));
            auto peakNodeGeometry = peakNode->geometry();
            auto peakLineNode = static_cast<QSGGeometryNode *>(sectionNode->childAtIndex(1));
            auto peakLineNodeGeometry = peakLineNode->geometry();
            auto rmsNode = static_cast<QSGGeometryNode *>(sectionNode->childAtIndex(2));
            auto rmsNodeGeometry = rmsNode->geometry();
            auto lineGraphNode = static_cast<QSGGeometryNode *>(sectionNode->childAtIndex(3));
            auto lineGraphNodeGeometry = lineGraphNode->geometry();

            const auto waveformLengthPerPixel = sectionPixelCount == 0 ? 0.0 : sectionLength / sectionPixelCount;
            const bool isMipmap = d->waveformMipmap.level() == WaveformMipmap::Downscale || waveformLengthPerPixel >= 4;

            if (sectionLength == 0.0 || sectionWidth == 0.0) {
                peakNodeGeometry->allocate(0);
                peakLineNodeGeometry->allocate(0);
                rmsNodeGeometry->allocate(0);
                lineGraphNodeGeometry->allocate(0);
            } else if (isMipmap) {
                peakNodeGeometry->allocate((sectionPixelCount + 1) * 2);
                peakLineNodeGeometry->allocate(2);
                rmsNodeGeometry->allocate((sectionPixelCount + 1) * 2);
                lineGraphNodeGeometry->allocate(0);
            } else {
                const int interpolateLevel = qBound(1.0, std::pow(2, std::floor(2 - std::log2(waveformLengthPerPixel))), 1.0 * INTERPOLATE_MAX);
                const int pointCount = qMax(0.0, std::floor((sectionOffset + sectionLength) * interpolateLevel) - std::ceil(sectionOffset * interpolateLevel) + 1.0);
                peakNodeGeometry->allocate(0);
                peakLineNodeGeometry->allocate(0);
                rmsNodeGeometry->allocate(0);
                lineGraphNodeGeometry->allocate(pointCount + 2);
            }

            if (isMipmap && sectionLength > 0.0 && sectionWidth != 0.0) {
                for (int i = 0; i <= sectionPixelCount; i++) {
                    const auto x = sectionStartX + sectionWidth * i / sectionPixelCount;
                    const auto waveformBlockOffset = sectionOffset + qMin(i, sectionPixelCount - 1) * waveformLengthPerPixel;
                    const auto waveformBlockLength = qMax(2.0, waveformLengthPerPixel);
                    const auto peak = d->waveformMipmap.peak(std::round(waveformBlockOffset), std::round(waveformBlockOffset + waveformBlockLength) - std::round(waveformBlockOffset));
                    const auto normalizedPeak = qMakePair(
                        qMax(-1.0, 1.0 * peak.first / (d->waveformMipmap.sampleType() == WaveformMipmap::Int8 ? 127.0 : 32767.0)),
                        qMax(-1.0, 1.0 * peak.second / (d->waveformMipmap.sampleType() == WaveformMipmap::Int8 ? 127.0 : 32767.0))
                    );
                    peakNodeGeometry->vertexDataAsPoint2D()[i * 2].set(x, (1.0 - normalizedPeak.first) * 0.5);
                    peakNodeGeometry->vertexDataAsPoint2D()[i * 2 + 1].set(x, (1.0 - normalizedPeak.second) * 0.5);
                    if (d->waveformMipmap.useRms() && waveformLengthPerPixel > 16) {
                        const auto rms = waveformBlockLength < 0.5 ? 0 : d->waveformMipmap.rms(std::round(waveformBlockOffset), std::round(waveformBlockLength));
                        rmsNodeGeometry->vertexDataAsPoint2D()[i * 2].set(x, (1.0 - rms) * 0.5);
                        rmsNodeGeometry->vertexDataAsPoint2D()[i * 2 + 1].set(x, (1.0 + rms) * 0.5);
                    } else {
                        rmsNodeGeometry->vertexDataAsPoint2D()[i * 2].set(0, 0);
                        rmsNodeGeometry->vertexDataAsPoint2D()[i * 2 + 1].set(0, 0);
                    }
                }
                peakLineNodeGeometry->vertexDataAsPoint2D()[0].set(sectionStartX, 0.5);
                peakLineNodeGeometry->vertexDataAsPoint2D()[1].set(sectionEndX, 0.5);
            } else if (sectionLength > 0.0 && sectionWidth != 0.0) {
                const int interpolateLevel = qBound(1.0, std::pow(2, std::floor(2 - std::log2(waveformLengthPerPixel))), 1.0 * INTERPOLATE_MAX);
                lineGraphNodeGeometry->vertexDataAsPoint2D()[0].set(sectionStartX, (1.0 - sampleValue(d->waveformMipmap, sectionOffset)) * 0.5);
                qint64 j = 1;
                for (qint64 i = std::ceil(sectionOffset * interpolateLevel); i <= std::floor((sectionOffset + sectionLength) * interpolateLevel); i++, j++) {
                    const double index = 1.0 * i / interpolateLevel;
                    const auto value = sampleValue(d->waveformMipmap, index);
                    const double pointX = sectionStartX + (index - sectionOffset) / sectionLength * sectionWidth;
                    const double pointY = (1.0 - value) * 0.5;
                    lineGraphNodeGeometry->vertexDataAsPoint2D()[j].set(pointX, pointY);
                }
                lineGraphNodeGeometry->vertexDataAsPoint2D()[j].set(sectionEndX, (1.0 - sampleValue(d->waveformMipmap, sectionOffset + sectionLength)) * 0.5);
            }

            peakNode->markDirty(QSGNode::DirtyGeometry);
            peakLineNode->markDirty(QSGNode::DirtyGeometry);
            rmsNode->markDirty(QSGNode::DirtyGeometry);
            lineGraphNode->markDirty(QSGNode::DirtyGeometry);
            sectionOffset += sectionLength;
        }

        return node;

    }
}

#include "moc_WaveformThumbnail.cpp"
