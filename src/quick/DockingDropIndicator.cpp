// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingDropIndicator_p.h"

#include <algorithm>
#include <cmath>

#include <QLineF>
#include <QPainter>
#include <QQuickWindow>
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>
#include <QSGRendererInterface>

#include <SVSCraftQuick/SoftwarePainterNode.h>

namespace SVS {
    class DockingDropSoftwareNode : public SoftwarePainterNode {
    public:
        explicit DockingDropSoftwareNode(QQuickItem *item) : SoftwarePainterNode(item) {}

        void setData(const QPolygonF &polygon, const QColor &color) {
            m_polygon = polygon;
            m_color = color;
            setBoundingRect(polygon.boundingRect());
            markDirty(QSGNode::DirtyMaterial);
        }

    protected:
        void paint(QPainter *painter) override {
            painter->setRenderHint(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            painter->setBrush(m_color);
            painter->drawPolygon(m_polygon);
        }

    private:
        QPolygonF m_polygon;
        QColor m_color;
    };

    DockingDropIndicator::DockingDropIndicator(QQuickItem *parent) : QQuickItem(parent) {
        setFlag(ItemHasContents);
    }

    QVariantList DockingDropIndicator::polygon() const {
        QVariantList points;
        for (const auto &point : m_polygon)
            points.append(point);
        return points;
    }

    void DockingDropIndicator::setPolygon(const QVariantList &points) {
        QPolygonF polygon;
        for (const auto &point : points)
            polygon.append(point.toPointF());
        if (m_polygon == polygon)
            return;
        m_polygon = polygon;
        updatePolygon();
        Q_EMIT polygonChanged();
    }

    QPointF DockingDropIndicator::origin() const {
        return m_origin;
    }

    void DockingDropIndicator::setOrigin(const QPointF &origin) {
        if (m_origin == origin)
            return;
        m_origin = origin;
        update();
        Q_EMIT originChanged();
    }

    QColor DockingDropIndicator::color() const {
        return m_color;
    }

    void DockingDropIndicator::setColor(const QColor &color) {
        if (m_color == color)
            return;
        m_color = color;
        update();
        Q_EMIT colorChanged();
    }

    qreal DockingDropIndicator::inset() const {
        return m_inset;
    }

    void DockingDropIndicator::setInset(qreal inset) {
        if (!std::isfinite(inset) || inset < 0 || m_inset == inset)
            return;
        m_inset = inset;
        updatePolygon();
        Q_EMIT insetChanged();
    }

    void DockingDropIndicator::updatePolygon() {
        m_displayPolygon.clear();
        if (m_polygon.size() == 4) {
            QPointF center;
            for (const auto &point : m_polygon)
                center += point / 4;

            QPointF normals[4];
            qreal inset = m_inset;
            bool valid = true;
            for (int i = 0; i < 4; ++i) {
                const auto edge = m_polygon[(i + 1) % 4] - m_polygon[i];
                const auto length = std::hypot(edge.x(), edge.y());
                if (qFuzzyIsNull(length)) {
                    valid = false;
                    break;
                }
                QPointF normal(-edge.y() / length, edge.x() / length);
                auto distance = QPointF::dotProduct(center - m_polygon[i], normal);
                if (distance < 0) {
                    normal = -normal;
                    distance = -distance;
                }
                normals[i] = normal;
                inset = std::min(inset, distance / 2);
            }

            if (valid) {
                QLineF edges[4];
                for (int i = 0; i < 4; ++i)
                    edges[i] = QLineF(m_polygon[i] + normals[i] * inset, m_polygon[(i + 1) % 4] + normals[i] * inset);
                for (int i = 0; i < 4; ++i) {
                    QPointF point;
                    if (edges[(i + 3) % 4].intersects(edges[i], &point) == QLineF::NoIntersection) {
                        m_displayPolygon.clear();
                        break;
                    }
                    m_displayPolygon.append(point);
                }
            }
        }
        update();
    }

    QSGNode *DockingDropIndicator::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) {
        if (!window() || m_displayPolygon.size() != 4 || !m_color.isValid() || m_color.alpha() == 0) {
            delete oldNode;
            return nullptr;
        }
        if (window()->rendererInterface()->graphicsApi() == QSGRendererInterface::Software) {
            auto node = static_cast<DockingDropSoftwareNode *>(oldNode);
            if (!node)
                node = new DockingDropSoftwareNode(this);
            node->setData(m_displayPolygon.translated(-m_origin), m_color);
            return node;
        }
        auto node = static_cast<QSGGeometryNode *>(oldNode);
        if (!node) {
            node = new QSGGeometryNode;
            auto geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 6);
            geometry->setDrawingMode(QSGGeometry::DrawTriangles);
            node->setGeometry(geometry);
            node->setFlag(QSGNode::OwnsGeometry);
            node->setMaterial(new QSGFlatColorMaterial);
            node->setFlag(QSGNode::OwnsMaterial);
        }
        const int indices[]{0, 1, 2, 0, 2, 3};
        auto vertices = node->geometry()->vertexDataAsPoint2D();
        for (int i = 0; i < 6; ++i) {
            auto point = m_displayPolygon[indices[i]] - m_origin;
            vertices[i].set(float(point.x()), float(point.y()));
        }
        static_cast<QSGFlatColorMaterial *>(node->material())->setColor(m_color);
        node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
        return node;
    }
}

#include "moc_DockingDropIndicator_p.cpp"
