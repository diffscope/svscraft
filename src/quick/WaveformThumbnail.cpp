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

#include <algorithm>
#include <cmath>
#include <limits>
#include <numbers>

#include <QQuickWindow>
#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGNode>
#include <QSGTransformNode>
#include <QSGVertexColorMaterial>
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

    void WaveformThumbnail::itemChange(ItemChange change, const ItemChangeData &value) {
        QQuickItem::itemChange(change, value);
        if (change == ItemTransformHasChanged || change == ItemDevicePixelRatioHasChanged) {
            update();
        }
    }

    struct WaveformRasterMetrics {
        double devicePixelRatio = 1.0;
        double xScale = 1.0;
        double yScale = 1.0;
        double xPhase = 0.0;

        constexpr bool operator==(const WaveformRasterMetrics &other) const {
            return devicePixelRatio == other.devicePixelRatio
                && xScale == other.xScale
                && yScale == other.yScale
                && xPhase == other.xPhase;
        }
    };

    static WaveformRasterMetrics rasterMetricsForItem(const QQuickItem *item) {
        WaveformRasterMetrics metrics;
        if (item->window()) {
            metrics.devicePixelRatio = item->window()->effectiveDevicePixelRatio();
        }
        if (!std::isfinite(metrics.devicePixelRatio) || metrics.devicePixelRatio <= 0.0) {
            metrics.devicePixelRatio = 1.0;
        }
        const QPointF sceneOrigin = item->mapToScene(QPointF());
        const QPointF sceneX = item->mapToScene(QPointF(1.0, 0.0)) - sceneOrigin;
        const QPointF sceneY = item->mapToScene(QPointF(0.0, 1.0)) - sceneOrigin;
        metrics.xScale = std::hypot(sceneX.x(), sceneX.y());
        metrics.yScale = std::hypot(sceneY.x(), sceneY.y());
        if (!std::isfinite(metrics.xScale) || metrics.xScale <= 0.0) {
            metrics.xScale = 1.0;
        }
        if (!std::isfinite(metrics.yScale) || metrics.yScale <= 0.0) {
            metrics.yScale = 1.0;
        }
        const QPointF sceneXAxis = sceneX / metrics.xScale;
        metrics.xPhase = QPointF::dotProduct(sceneOrigin, sceneXAxis) * metrics.devicePixelRatio;
        if (!std::isfinite(metrics.xPhase)) {
            metrics.xPhase = 0.0;
        } else {
            metrics.xPhase -= std::floor(metrics.xPhase);
        }
        return metrics;
    }

    struct WaveformFilterKernel {
        double samplesPerDevicePixel = -1.0;
        int radius = 0;
        int tapCount = 0;
        QVector<double> weights;
    };

    class WaveformThumbnailSGNode : public QSGNode {
    public:
        double m_waveformOffset = -1;
        QList<WaveformThumbnailSection> m_waveformSections;
        double m_width = 0;
        double m_height = 0;
        QColor m_color;
        QColor m_rmsColor;
        WaveformRasterMetrics m_rasterMetrics;
        QVector<WaveformFilterKernel> m_filterKernels;
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
    static inline double interpolateAtPhase(const T *a, qsizetype size, qsizetype i, int k) {
        if (k == 0) {
            return i >= 0 && i < size ? 1.0 * a[i] / std::numeric_limits<T>::max() : 0.0;
        }
        double ret = 0;
        for (int j = -INTERPOLATE_WINDOW + 1; j <= INTERPOLATE_WINDOW; j++) {
            qsizetype idx = i + j;
            if (idx >= 0 && idx < size) {
                ret += a[idx] * sinc(j, k);
            }
        }
        return std::clamp(ret / std::numeric_limits<T>::max(), -1.0, 1.0);
    }

    template <typename T>
    static inline double interpolate(const T *a, qsizetype size, double x) {
        const qsizetype i = std::floor(x);
        const double phase = (x - i) * 16.0;
        const int firstPhase = static_cast<int>(std::floor(phase));
        const double ratio = phase - firstPhase;
        const double first = interpolateAtPhase(a, size, i, firstPhase);
        const double second = firstPhase == 15
            ? interpolateAtPhase(a, size, i + 1, 0)
            : interpolateAtPhase(a, size, i, firstPhase + 1);
        return first + ratio * (second - first);
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

    static double sincPi(double value) {
        if (qFuzzyIsNull(value)) {
            return 1.0;
        }
        const double argument = std::numbers::pi * value;
        return std::sin(argument) / argument;
    }

    class FilteredWaveformSampler {
    public:
        FilteredWaveformSampler(const WaveformMipmap &waveformMipmap,
                                double samplesPerDevicePixel,
                                WaveformFilterKernel *kernel)
            : m_waveformMipmap(waveformMipmap), m_kernel(*kernel) {
            if (m_kernel.samplesPerDevicePixel == samplesPerDevicePixel) {
                return;
            }
            const double cutoff = 1.0 / std::max(1.0, samplesPerDevicePixel);
            m_kernel.samplesPerDevicePixel = samplesPerDevicePixel;
            m_kernel.radius = std::clamp(static_cast<int>(std::ceil(INTERPOLATE_WINDOW / cutoff)),
                                         INTERPOLATE_WINDOW,
                                         INTERPOLATE_WINDOW * 4);
            m_kernel.tapCount = m_kernel.radius * 2;
            m_kernel.weights.resize((phaseCount + 1) * m_kernel.tapCount);
            for (int phase = 0; phase <= phaseCount; ++phase) {
                const double fraction = static_cast<double>(phase) / phaseCount;
                double weightSum = 0.0;
                for (int tap = 0; tap < m_kernel.tapCount; ++tap) {
                    const int offset = tap - m_kernel.radius + 1;
                    const double distance = offset - fraction;
                    const double weight = cutoff * sincPi(cutoff * distance) * sincPi(distance / m_kernel.radius);
                    m_kernel.weights[phase * m_kernel.tapCount + tap] = weight;
                    weightSum += weight;
                }
                if (!qFuzzyIsNull(weightSum)) {
                    for (int tap = 0; tap < m_kernel.tapCount; ++tap) {
                        m_kernel.weights[phase * m_kernel.tapCount + tap] /= weightSum;
                    }
                }
            }
        }

        double value(double index) const {
            if (m_waveformMipmap.size() <= 0) {
                return 0.0;
            }
            index = std::clamp(index, 0.0, static_cast<double>(m_waveformMipmap.size() - 1));
            const qsizetype baseIndex = std::floor(index);
            const double phase = (index - baseIndex) * phaseCount;
            const int firstPhase = static_cast<int>(std::floor(phase));
            const int secondPhase = std::min(firstPhase + 1, phaseCount);
            const double phaseRatio = phase - firstPhase;
            return m_waveformMipmap.sampleType() == WaveformMipmap::Int8
                ? valueForData(m_waveformMipmap.originalDataAsInt8(), baseIndex, firstPhase, secondPhase, phaseRatio)
                : valueForData(m_waveformMipmap.originalDataAsInt16(), baseIndex, firstPhase, secondPhase, phaseRatio);
        }

    private:
        template <typename T>
        double valueForData(const T *data,
                            qsizetype baseIndex,
                            int firstPhase,
                            int secondPhase,
                            double phaseRatio) const {
            double result = 0.0;
            double validWeightSum = 0.0;
            for (int tap = 0; tap < m_kernel.tapCount; ++tap) {
                const qsizetype sampleIndex = baseIndex + tap - m_kernel.radius + 1;
                if (sampleIndex < 0 || sampleIndex >= m_waveformMipmap.size()) {
                    continue;
                }
                const double firstWeight = m_kernel.weights.at(firstPhase * m_kernel.tapCount + tap);
                const double secondWeight = m_kernel.weights.at(secondPhase * m_kernel.tapCount + tap);
                const double weight = firstWeight + phaseRatio * (secondWeight - firstWeight);
                result += data[sampleIndex] * weight;
                validWeightSum += weight;
            }
            if (qFuzzyIsNull(validWeightSum)) {
                return 0.0;
            }
            return std::clamp(result / validWeightSum / std::numeric_limits<T>::max(), -1.0, 1.0);
        }

        static constexpr int phaseCount = 64;
        const WaveformMipmap &m_waveformMipmap;
        WaveformFilterKernel &m_kernel;
    };

    struct WaveformGeometryVertex {
        QPointF point;
        float coverage = 1.0f;
    };

    struct WaveformGeometry {
        QVector<WaveformGeometryVertex> vertices;
        QVector<quint32> indices;
    };

    struct WaveformAreaPoint {
        double x = 0.0;
        double firstY = 0.0;
        double secondY = 0.0;
    };

    static quint32 appendVertex(WaveformGeometry &geometry,
                                const QPointF &point,
                                float coverage) {
        const auto index = static_cast<quint32>(geometry.vertices.size());
        geometry.vertices.append({point, coverage});
        return index;
    }

    static void appendQuadIndices(WaveformGeometry &geometry,
                                  quint32 a,
                                  quint32 b,
                                  quint32 c,
                                  quint32 d) {
        geometry.indices.append(a);
        geometry.indices.append(b);
        geometry.indices.append(c);
        geometry.indices.append(c);
        geometry.indices.append(b);
        geometry.indices.append(d);
    }

    static void appendAntialiasedPolyline(WaveformGeometry &geometry,
                                          const QVector<QPointF> &points,
                                          double width,
                                          double antialiasWidth) {
        if (points.size() < 2) {
            return;
        }
        const double fringeHalfWidth = antialiasWidth * 0.5;
        const double innerHalfWidth = std::max(0.0, width * 0.5 - fringeHalfWidth);
        const double outerHalfWidth = width * 0.5 + fringeHalfWidth;
        const bool hasCore = innerHalfWidth > 0.0;
        const int verticesPerPoint = hasCore ? 4 : 3;
        const quint32 firstVertex = static_cast<quint32>(geometry.vertices.size());
        geometry.vertices.reserve(geometry.vertices.size() + points.size() * verticesPerPoint);
        geometry.indices.reserve(geometry.indices.size() + (points.size() - 1) * (hasCore ? 18 : 12));
        for (qsizetype i = 0; i < points.size(); ++i) {
            const QPointF direction = i == 0
                ? points.at(1) - points.at(0)
                : i == points.size() - 1
                    ? points.at(i) - points.at(i - 1)
                    : points.at(i + 1) - points.at(i - 1);
            const double length = std::hypot(direction.x(), direction.y());
            const QPointF unitNormal = qFuzzyIsNull(length)
                ? QPointF(0.0, 1.0)
                : QPointF(-direction.y() / length, direction.x() / length);
            appendVertex(geometry, points.at(i) + unitNormal * outerHalfWidth, 0.0f);
            if (hasCore) {
                appendVertex(geometry, points.at(i) + unitNormal * innerHalfWidth, 1.0f);
                appendVertex(geometry, points.at(i) - unitNormal * innerHalfWidth, 1.0f);
            } else {
                appendVertex(geometry, points.at(i), 1.0f);
            }
            appendVertex(geometry, points.at(i) - unitNormal * outerHalfWidth, 0.0f);
        }
        for (qsizetype i = 1; i < points.size(); ++i) {
            const quint32 previous = firstVertex + static_cast<quint32>((i - 1) * verticesPerPoint);
            const quint32 current = firstVertex + static_cast<quint32>(i * verticesPerPoint);
            appendQuadIndices(geometry, previous, previous + 1, current, current + 1);
            if (hasCore) {
                appendQuadIndices(geometry, previous + 1, previous + 2, current + 1, current + 2);
                appendQuadIndices(geometry, previous + 2, previous + 3, current + 2, current + 3);
            } else {
                appendQuadIndices(geometry, previous + 1, previous + 2, current + 1, current + 2);
            }
        }
    }

    static void appendAntialiasedArea(WaveformGeometry &geometry,
                                      const QVector<WaveformAreaPoint> &points,
                                      double antialiasWidth) {
        if (points.size() < 2) {
            return;
        }
        const bool hasArea = std::any_of(points.cbegin(), points.cend(), [](const WaveformAreaPoint &point) {
            return !qFuzzyCompare(point.firstY + 1.0, point.secondY + 1.0);
        });
        if (!hasArea) {
            return;
        }
        const double fringeHalfWidth = antialiasWidth * 0.5;
        const quint32 firstVertex = static_cast<quint32>(geometry.vertices.size());
        geometry.vertices.reserve(geometry.vertices.size() + points.size() * 4);
        geometry.indices.reserve(geometry.indices.size() + (points.size() - 1) * 18);
        for (const auto &point : points) {
            const double topY = std::min(point.firstY, point.secondY);
            const double bottomY = std::max(point.firstY, point.secondY);
            const double inset = std::min(fringeHalfWidth, (bottomY - topY) * 0.5);
            appendVertex(geometry, QPointF(point.x, topY - fringeHalfWidth), 0.0f);
            appendVertex(geometry, QPointF(point.x, topY + inset), 1.0f);
            appendVertex(geometry, QPointF(point.x, bottomY - inset), 1.0f);
            appendVertex(geometry, QPointF(point.x, bottomY + fringeHalfWidth), 0.0f);
        }
        for (qsizetype i = 1; i < points.size(); ++i) {
            const quint32 previous = firstVertex + static_cast<quint32>((i - 1) * 4);
            const quint32 current = firstVertex + static_cast<quint32>(i * 4);
            appendQuadIndices(geometry, previous, previous + 1, current, current + 1);
            appendQuadIndices(geometry, previous + 1, previous + 2, current + 1, current + 2);
            appendQuadIndices(geometry, previous + 2, previous + 3, current + 2, current + 3);
        }
    }

    static QSGGeometryNode *createGeometryNode() {
        auto node = new QSGGeometryNode;
        node->setFlag(QSGNode::OwnsGeometry);
        node->setFlag(QSGNode::OwnsMaterial);
        node->setFlag(QSGNode::OwnedByParent);
        auto geometry = new QSGGeometry(QSGGeometry::defaultAttributes_ColoredPoint2D(),
                                        0,
                                        0,
                                        QSGGeometry::UnsignedIntType);
        geometry->setDrawingMode(QSGGeometry::DrawTriangles);
        geometry->setVertexDataPattern(QSGGeometry::DynamicPattern);
        geometry->setIndexDataPattern(QSGGeometry::DynamicPattern);
        node->setGeometry(geometry);
        node->setMaterial(new QSGVertexColorMaterial);
        return node;
    }

    static void updateGeometryNode(QSGGeometryNode *node,
                                   const WaveformGeometry &source,
                                   const QColor &color) {
        auto geometry = node->geometry();
        geometry->allocate(static_cast<int>(source.vertices.size()),
                           static_cast<int>(source.indices.size()));
        auto vertices = geometry->vertexDataAsColoredPoint2D();
        for (int i = 0; i < source.vertices.size(); ++i) {
            const auto &point = source.vertices.at(i);
            const int alpha = qRound(color.alpha() * std::clamp(point.coverage, 0.0f, 1.0f));
            const int red = qRound(color.red() * alpha / 255.0);
            const int green = qRound(color.green() * alpha / 255.0);
            const int blue = qRound(color.blue() * alpha / 255.0);
            vertices[i].set(point.point.x(),
                            point.point.y(),
                            red,
                            green,
                            blue,
                            alpha);
        }
        std::copy(source.indices.cbegin(), source.indices.cend(), geometry->indexDataAsUInt());
        node->markDirty(QSGNode::DirtyGeometry);
    }

    static QVector<double> devicePixelBoundaries(double startX,
                                                 double endX,
                                                 const WaveformRasterMetrics &metrics) {
        QVector<double> result;
        const double startDeviceX = metrics.xPhase
            + startX * metrics.devicePixelRatio * metrics.xScale;
        const double endDeviceX = metrics.xPhase
            + endX * metrics.devicePixelRatio * metrics.xScale;
        const int interiorCount = static_cast<int>(std::ceil(std::abs(endDeviceX - startDeviceX)));
        result.reserve(interiorCount + 2);
        result.append(startX);
        if (endDeviceX > startDeviceX) {
            for (double deviceX = std::floor(startDeviceX) + 1.0; deviceX < endDeviceX; deviceX += 1.0) {
                result.append((deviceX - metrics.xPhase) / (metrics.devicePixelRatio * metrics.xScale));
            }
        } else {
            for (double deviceX = std::ceil(startDeviceX) - 1.0; deviceX > endDeviceX; deviceX -= 1.0) {
                result.append((deviceX - metrics.xPhase) / (metrics.devicePixelRatio * metrics.xScale));
            }
        }
        result.append(endX);
        return result;
    }

    static QSGTransformNode *createWaveformSectionNode() {
        auto sectionNode = new QSGTransformNode;
        sectionNode->setFlag(QSGNode::OwnedByParent);
        sectionNode->appendChildNode(createGeometryNode());
        sectionNode->appendChildNode(createGeometryNode());
        sectionNode->appendChildNode(createGeometryNode());
        sectionNode->appendChildNode(createGeometryNode());
        return sectionNode;
    }

    QSGNode *WaveformThumbnail::updatePaintNode(QSGNode *node_, UpdatePaintNodeData *update_paint_node_data) {
        Q_UNUSED(update_paint_node_data)
        Q_D(const WaveformThumbnail);
        if (!d->waveformMipmap.isValid()) {
            delete node_;
            return nullptr;
        }
        const WaveformRasterMetrics rasterMetrics = rasterMetricsForItem(this);
        auto node = static_cast<WaveformThumbnailSGNode *>(node_);
        if (!node) {
            node = new WaveformThumbnailSGNode();
        }

        while (node->childCount() < d->waveformSections.size()) {
            node->appendChildNode(createWaveformSectionNode());
        }
        while (node->childCount() > d->waveformSections.size()) {
            auto child = node->lastChild();
            node->removeChildNode(child);
            delete child;
        }
        node->m_filterKernels.resize(d->waveformSections.size());

        if (!d->waveformMipmapUpdatePending
            && d->waveformOffset == node->m_waveformOffset
            && d->waveformSections == node->m_waveformSections
            && width() == node->m_width
            && height() == node->m_height
            && d->color == node->m_color
            && d->rmsColor == node->m_rmsColor
            && rasterMetrics == node->m_rasterMetrics) {
            return node;
        }

        node->m_width = width();
        node->m_height = height();
        node->m_waveformOffset = d->waveformOffset;
        node->m_waveformSections = d->waveformSections;
        node->m_color = d->color;
        node->m_rmsColor = d->rmsColor;
        node->m_rasterMetrics = rasterMetrics;
        d->waveformMipmapUpdatePending = false;

        const double antialiasWidth = 1.0
            / (rasterMetrics.devicePixelRatio * std::sqrt(rasterMetrics.xScale * rasterMetrics.yScale));
        const double verticalAntialiasWidth = 1.0
            / (rasterMetrics.devicePixelRatio * rasterMetrics.yScale);
        double sectionOffset = d->waveformOffset;
        for (int sectionIndex = 0; sectionIndex < d->waveformSections.size(); ++sectionIndex) {
            const auto &section = d->waveformSections.at(sectionIndex);
            const auto sectionLength = qMax(0.0, section.length);
            const auto sectionStartX = section.start * width();
            const auto sectionEndX = section.end * width();
            const auto sectionWidth = sectionEndX - sectionStartX;
            const auto sectionNode = static_cast<QSGTransformNode *>(node->childAtIndex(sectionIndex));
            auto peakNode = static_cast<QSGGeometryNode *>(sectionNode->childAtIndex(0));
            auto peakLineNode = static_cast<QSGGeometryNode *>(sectionNode->childAtIndex(1));
            auto rmsNode = static_cast<QSGGeometryNode *>(sectionNode->childAtIndex(2));
            auto lineGraphNode = static_cast<QSGGeometryNode *>(sectionNode->childAtIndex(3));

            WaveformGeometry peakGeometry;
            WaveformGeometry peakLineGeometry;
            WaveformGeometry rmsGeometry;
            WaveformGeometry lineGraphGeometry;

            const auto logicalPixelCount = qMax(0, static_cast<int>(std::ceil(std::abs(sectionWidth))));
            const auto waveformLengthPerPixel = logicalPixelCount == 0
                ? 0.0
                : sectionLength / logicalPixelCount;
            const bool isMipmap = d->waveformMipmap.level() == WaveformMipmap::Downscale || waveformLengthPerPixel >= 4;

            if (sectionLength > 0.0 && sectionWidth != 0.0 && height() > 0.0 && isMipmap) {
                const QVector<double> boundaries = devicePixelBoundaries(sectionStartX, sectionEndX, rasterMetrics);
                struct WaveformSummary {
                    double peakFirstY = 0.0;
                    double peakSecondY = 0.0;
                    double rmsFirstY = 0.0;
                    double rmsSecondY = 0.0;
                };
                QVector<WaveformSummary> summaries;
                summaries.reserve(boundaries.size());
                for (qsizetype i = 0; i < boundaries.size(); ++i) {
                    const qsizetype intervalIndex = std::min(i, boundaries.size() - 2);
                    const double intervalStartX = boundaries.at(intervalIndex);
                    const double intervalEndX = boundaries.at(intervalIndex + 1);
                    const double intervalStartRatio = (intervalStartX - sectionStartX) / sectionWidth;
                    const double intervalEndRatio = (intervalEndX - sectionStartX) / sectionWidth;
                    const double intervalDataStart = sectionOffset + intervalStartRatio * sectionLength;
                    const double intervalDataEnd = sectionOffset + intervalEndRatio * sectionLength;
                    const double waveformBlockOffset = std::min(intervalDataStart, intervalDataEnd);
                    const double waveformBlockLength = qMax(2.0, std::abs(intervalDataEnd - intervalDataStart));
                    const double boundedBlockOffset = std::clamp(waveformBlockOffset,
                                                                 0.0,
                                                                 static_cast<double>(d->waveformMipmap.size()));
                    const double boundedBlockEnd = std::clamp(waveformBlockOffset + waveformBlockLength,
                                                              boundedBlockOffset,
                                                              static_cast<double>(d->waveformMipmap.size()));
                    const qsizetype roundedOffset = static_cast<qsizetype>(std::round(boundedBlockOffset));
                    qsizetype roundedEnd = std::max(roundedOffset,
                                                    static_cast<qsizetype>(std::round(boundedBlockEnd)));
                    if (roundedEnd == roundedOffset && roundedOffset < d->waveformMipmap.size()) {
                        ++roundedEnd;
                    }
                    const auto peak = d->waveformMipmap.peak(roundedOffset, roundedEnd - roundedOffset);
                    const auto normalizedPeak = qMakePair(
                        qMax(-1.0, 1.0 * peak.first / (d->waveformMipmap.sampleType() == WaveformMipmap::Int8 ? 127.0 : 32767.0)),
                        qMax(-1.0, 1.0 * peak.second / (d->waveformMipmap.sampleType() == WaveformMipmap::Int8 ? 127.0 : 32767.0))
                    );
                    WaveformSummary summary;
                    summary.peakFirstY = (1.0 - normalizedPeak.first) * height() * 0.5;
                    summary.peakSecondY = (1.0 - normalizedPeak.second) * height() * 0.5;
                    if (d->waveformMipmap.useRms() && waveformLengthPerPixel > 16) {
                        const auto rms = d->waveformMipmap.rms(roundedOffset, roundedEnd - roundedOffset);
                        summary.rmsFirstY = (1.0 - rms) * height() * 0.5;
                        summary.rmsSecondY = (1.0 + rms) * height() * 0.5;
                    }
                    summaries.append(summary);
                }
                QVector<WaveformAreaPoint> peakPoints;
                QVector<WaveformAreaPoint> rmsPoints;
                peakPoints.reserve(boundaries.size());
                if (d->waveformMipmap.useRms() && waveformLengthPerPixel > 16) {
                    rmsPoints.reserve(boundaries.size());
                }
                for (qsizetype i = 0; i < boundaries.size(); ++i) {
                    peakPoints.append({boundaries.at(i),
                                       summaries.at(i).peakFirstY,
                                       summaries.at(i).peakSecondY});
                    if (d->waveformMipmap.useRms() && waveformLengthPerPixel > 16) {
                        rmsPoints.append({boundaries.at(i),
                                          summaries.at(i).rmsFirstY,
                                          summaries.at(i).rmsSecondY});
                    }
                }
                appendAntialiasedArea(peakGeometry, peakPoints, verticalAntialiasWidth);
                appendAntialiasedArea(rmsGeometry, rmsPoints, verticalAntialiasWidth);
                appendAntialiasedPolyline(peakLineGeometry,
                                          {QPointF(sectionStartX, height() * 0.5),
                                           QPointF(sectionEndX, height() * 0.5)},
                                          antialiasWidth,
                                          antialiasWidth);
            } else if (sectionLength > 0.0 && sectionWidth != 0.0 && height() > 0.0) {
                QVector<QPointF> linePoints;
                const double physicalWidth = std::abs(sectionWidth)
                    * rasterMetrics.devicePixelRatio
                    * rasterMetrics.xScale;
                const double samplesPerDevicePixel = physicalWidth > 0.0
                    ? sectionLength / physicalWidth
                    : 0.0;
                if (samplesPerDevicePixel > 1.0) {
                    const QVector<double> boundaries = devicePixelBoundaries(sectionStartX, sectionEndX, rasterMetrics);
                    linePoints.reserve(boundaries.size() + 1);
                    linePoints.append(QPointF(sectionStartX,
                                              (1.0 - sampleValue(d->waveformMipmap, sectionOffset)) * height() * 0.5));
                    const FilteredWaveformSampler sampler(d->waveformMipmap,
                                                          samplesPerDevicePixel,
                                                          &node->m_filterKernels[sectionIndex]);
                    for (qsizetype i = 1; i < boundaries.size(); ++i) {
                        const double x = (boundaries.at(i - 1) + boundaries.at(i)) * 0.5;
                        const double index = sectionOffset + (x - sectionStartX) / sectionWidth * sectionLength;
                        linePoints.append(QPointF(x, (1.0 - sampler.value(index)) * height() * 0.5));
                    }
                    linePoints.append(QPointF(sectionEndX,
                                              (1.0 - sampleValue(d->waveformMipmap, sectionOffset + sectionLength)) * height() * 0.5));
                } else {
                    const int interpolateLevel = qBound(1.0,
                                                        std::pow(2, std::floor(2 - std::log2(waveformLengthPerPixel))),
                                                        1.0 * INTERPOLATE_MAX);
                    linePoints.append(QPointF(sectionStartX,
                                              (1.0 - sampleValue(d->waveformMipmap, sectionOffset)) * height() * 0.5));
                    for (qint64 i = std::ceil(sectionOffset * interpolateLevel);
                         i <= std::floor((sectionOffset + sectionLength) * interpolateLevel);
                         ++i) {
                        const double index = 1.0 * i / interpolateLevel;
                        const auto value = sampleValue(d->waveformMipmap, index);
                        const double pointX = sectionStartX + (index - sectionOffset) / sectionLength * sectionWidth;
                        linePoints.append(QPointF(pointX, (1.0 - value) * height() * 0.5));
                    }
                    linePoints.append(QPointF(sectionEndX,
                                              (1.0 - sampleValue(d->waveformMipmap, sectionOffset + sectionLength)) * height() * 0.5));
                }
                appendAntialiasedPolyline(lineGraphGeometry,
                                          linePoints,
                                          antialiasWidth,
                                          antialiasWidth);
            }

            updateGeometryNode(peakNode, peakGeometry, d->color);
            updateGeometryNode(peakLineNode, peakLineGeometry, d->color);
            updateGeometryNode(rmsNode, rmsGeometry, d->rmsColor);
            updateGeometryNode(lineGraphNode, lineGraphGeometry, d->color);
            sectionOffset += sectionLength;
        }

        return node;

    }
}

#include "moc_WaveformThumbnail.cpp"
