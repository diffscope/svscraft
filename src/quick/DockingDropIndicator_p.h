// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGDROPINDICATOR_P_H
#define SVSCRAFT_DOCKINGDROPINDICATOR_P_H

#include <QColor>
#include <QPointF>
#include <QPolygonF>
#include <QQuickItem>
#include <QVariantList>
#include <qqmlintegration.h>

namespace SVS {
    class DockingDropIndicator : public QQuickItem {
        Q_OBJECT
        QML_NAMED_ELEMENT(DockingDropIndicatorTemplate)
        Q_PROPERTY(QVariantList polygon READ polygon WRITE setPolygon NOTIFY polygonChanged)
        Q_PROPERTY(QPointF origin READ origin WRITE setOrigin NOTIFY originChanged)
        Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
        Q_PROPERTY(qreal inset READ inset WRITE setInset NOTIFY insetChanged)
    public:
        explicit DockingDropIndicator(QQuickItem *parent = nullptr);
        QVariantList polygon() const;
        void setPolygon(const QVariantList &polygon);
        QPointF origin() const;
        void setOrigin(const QPointF &origin);
        QColor color() const;
        void setColor(const QColor &color);
        qreal inset() const;
        void setInset(qreal inset);
    Q_SIGNALS:
        void polygonChanged();
        void originChanged();
        void colorChanged();
        void insetChanged();

    protected:
        QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

    private:
        void updatePolygon();

        QPolygonF m_polygon;
        QPolygonF m_displayPolygon;
        QPointF m_origin;
        QColor m_color;
        qreal m_inset = 0;
    };
}

#endif
