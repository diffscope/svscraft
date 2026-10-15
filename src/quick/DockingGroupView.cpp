// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingGroupView.h"
#include "DockingGroupView_p.h"

#include <algorithm>

#include <QPolygonF>

#include <SVSCraftQuick/DockingHost.h>
#include <SVSCraftQuick/DockingLayout.h>
#include <SVSCraftQuick/DockingPanel.h>
#include <SVSCraftQuick/private/DockingGroup_p.h>

namespace SVS {
    DockingGroupView::DockingGroupView(QQuickItem *parent)
        : QQuickItem(parent), d_ptr(new DockingGroupViewPrivate) {
        d_ptr->q_ptr = this;
        connect(this, &DockingGroupView::geometryChanged, this, [this] { d_ptr->regionsDirty = true; });
    }

    DockingGroupView::~DockingGroupView() = default;

    DockingGroup *DockingGroupView::group() const {
        Q_D(const DockingGroupView);
        return d->group;
    }

    void DockingGroupView::setGroup(DockingGroup *g) {
        Q_D(DockingGroupView);
        if (d->group == g)
            return;
        disconnect(d->connection);
        disconnect(d->destroyedConnection);
        d->group = g;
        if (g)
            d->connection =
                connect(g, &DockingGroup::changed, this, &DockingGroupView::geometryChanged);
        if (g)
            d->destroyedConnection = connect(g, &QObject::destroyed, this, [this] {
                Q_EMIT groupChanged();
                Q_EMIT geometryChanged();
            });
        Q_EMIT groupChanged();
        Q_EMIT geometryChanged();
    }

    bool DockingGroupView::revealBar() const {
        Q_D(const DockingGroupView);
        return d->reveal;
    }

    void DockingGroupView::setRevealBar(bool v) {
        Q_D(DockingGroupView);
        if (v == d->reveal)
            return;
        d->reveal = v;
        Q_EMIT geometryChanged();
    }

    bool DockingGroupView::mirrored() const {
        Q_D(const DockingGroupView);
        return d->mirrored;
    }

    void DockingGroupView::setMirrored(bool v) {
        Q_D(DockingGroupView);
        if (v == d->mirrored)
            return;
        d->mirrored = v;
        Q_EMIT geometryChanged();
    }

    QRectF DockingGroupView::barRect() const {
        auto g = group();
        if (!g || g->isBarHidden() && !revealBar())
            return {};
        switch (g->tabBarEdge()) {
            case Qt::TopEdge:
                return {0, 0, width(), barThickness()};
            case Qt::BottomEdge:
                return {0, height() - barThickness(), width(), barThickness()};
            case Qt::LeftEdge:
                return {0, 0, barThickness(), height()};
            case Qt::RightEdge:
                return {width() - barThickness(), 0, barThickness(), height()};
        }
        return {};
    }

    qreal DockingGroupView::barThickness() const {
        return tabButtonSize() + 2 * barPadding();
    }

    qreal DockingGroupView::barPadding() const {
        auto g = group();
        return g ? DockingGroupPrivate::get(g)->tabBarPadding() : 4;
    }

    qreal DockingGroupView::tabButtonSize() const {
        auto g = group();
        return g ? DockingGroupPrivate::get(g)->tabButtonSize() : 24;
    }

    qreal DockingGroupView::panelHeaderHeight() const {
        return DockingGroupPrivate::panelHeaderHeight;
    }

    QRectF DockingGroupViewPrivate::slot(int side, bool excludeRevealedBar) const {
        Q_Q(const DockingGroupView);
        if (!group || !group->selectedPanel(DockingGroup::Side(side)))
            return {};
        QRectF r(0, 0, q->width(), q->height());
        if (!group->isBarHidden() || (excludeRevealedBar && q->revealBar())) {
            switch (group->tabBarEdge()) {
                case Qt::TopEdge:
                    r.adjust(0, q->barThickness(), 0, 0);
                    break;
                case Qt::BottomEdge:
                    r.adjust(0, 0, 0, -q->barThickness());
                    break;
                case Qt::LeftEdge:
                    r.adjust(q->barThickness(), 0, 0, 0);
                    break;
                case Qt::RightEdge:
                    r.adjust(0, 0, -q->barThickness(), 0);
                    break;
            }
        }
        if (!group->leadingPanel() || !group->trailingPanel())
            return r;
        bool horizontal =
            group->tabBarEdge() == Qt::TopEdge || group->tabBarEdge() == Qt::BottomEdge;
        if (horizontal) {
            qreal first = std::max(qreal(0), r.width() - 1) * group->contentRatio();
            qreal x = side == 0 ? 0 : first + 1;
            qreal w = side == 0 ? first : r.width() - first - 1;
            if (mirrored)
                x = r.width() - x - w;
            r.setRect(r.x() + x, r.y(), w, r.height());
        } else {
            qreal first = std::max(qreal(0), r.height() - 1) * group->contentRatio();
            r.setRect(r.x(), r.y() + (side ? first + 1 : 0), r.width(), side ? r.height() - first - 1 : first);
        }
        return r;
    }

    QRectF DockingGroupView::leadingRect() const {
        Q_D(const DockingGroupView);
        return d->slot(0);
    }

    QRectF DockingGroupView::trailingRect() const {
        Q_D(const DockingGroupView);
        return d->slot(1);
    }

    QVariantList DockingGroupView::dropRegions() const {
        Q_D(const DockingGroupView);
        if (!d->regionsDirty)
            return d->regions;
        d->regionsDirty = false;
        d->regions.clear();
        auto g = group();
        if (!g)
            return {};
        const bool horizontal = g->tabBarEdge() == Qt::TopEdge || g->tabBarEdge() == Qt::BottomEdge;
        for (int side = 0; side < 2; ++side) {
            const auto rect = d->slot(side, true);
            if (rect.isEmpty())
                continue;
            const auto band = std::min(qreal(32), std::min(rect.width(), rect.height()) / 4);
            const auto center = rect.adjusted(band, band, -band, -band);
            DockingPlacement placement;
            placement.hostId = g->layout()->host()->hostId();
            placement.groupId = g->nodeId();
            placement.side = side;

            auto append = [&](const DockingPlacement &target, const QPolygonF &polygon, int label) {
                QVariantList points;
                for (int i = 0; i < 4; ++i)
                    points.append(polygon[i]);
                auto value = target.toVariant();
                value.insert("polygon", points);
                value.insert("rect", polygon.boundingRect());
                value.insert("panelRect", rect);
                value.insert("label", label);
                d->regions.append(value);
            };

            placement.kind = DockingPlacement::Edge;
            placement.edge = Qt::TopEdge;
            append(placement, {rect.topLeft(), rect.topRight(), center.topRight(), center.topLeft()}, 3);
            placement.edge = Qt::RightEdge;
            append(placement, {rect.topRight(), rect.bottomRight(), center.bottomRight(), center.topRight()}, 3);
            placement.edge = Qt::BottomEdge;
            append(placement, {rect.bottomRight(), rect.bottomLeft(), center.bottomLeft(), center.bottomRight()}, 3);
            placement.edge = Qt::LeftEdge;
            append(placement, {rect.bottomLeft(), rect.topLeft(), center.topLeft(), center.bottomLeft()}, 3);

            bool low = side == 0;
            if (horizontal && mirrored())
                low = !low;
            const qreal cut = low ? 2.0 / 3 : 1.0 / 3;
            for (int opposite = 0; opposite < 2; ++opposite) {
                const qreal total = horizontal ? center.width() : center.height();
                const qreal start = (bool(opposite) == low) ? total * cut : 0;
                const qreal extent = opposite ? total / 3 : total * 2 / 3;
                const auto area =
                    horizontal ? QRectF(center.x() + start, center.y(), extent, center.height())
                               : QRectF(center.x(), center.y() + start, center.width(), extent);
                placement.kind = opposite ? DockingPlacement::Opposite : DockingPlacement::Center;
                placement.side = opposite ? 1 - side : side;
                placement.anchor = opposite ? QString{} : g->selectedPanel(DockingGroup::Side(side))->key();
                append(placement, QPolygonF(area), opposite ? 2 : 1);
            }
        }
        return d->regions;
    }

    QVariantMap DockingGroupView::hitTest(qreal x, qreal y) const {
        const QPointF point(x, y);
        for (const auto &region : dropRegions()) {
            auto value = region.toMap();
            const auto rect = value.value("rect").toRectF();
            if (!rect.contains(point))
                continue;
            QPolygonF polygon;
            for (const auto &position : value.value("polygon").toList())
                polygon.append(position.toPointF());
            if (!polygon.containsPoint(point, Qt::OddEvenFill))
                continue;
            value.insert("previewRect", rect);
            return value;
        }
        return {};
    }

    void DockingGroupView::resizeContents(qreal ratio) {
        auto g = group();
        if (!g || !g->leadingPanel() || !g->trailingPanel())
            return;
        bool horizontal = g->tabBarEdge() == Qt::TopEdge || g->tabBarEdge() == Qt::BottomEdge;
        auto a = g->leadingPanel()->minimumSize(), b = g->trailingPanel()->minimumSize();
        qreal extent = horizontal ? leadingRect().width() + trailingRect().width()
                                  : leadingRect().height() + trailingRect().height();
        qreal amin = horizontal ? a.width() : a.height() + panelHeaderHeight(),
              bmin = horizontal ? b.width() : b.height() + panelHeaderHeight();
        if (extent <= 0 || amin + bmin > extent)
            return;
        auto lower = std::max(qreal(0.001), amin / extent);
        auto upper = std::min(qreal(0.999), 1 - bmin / extent);
        if (lower <= upper)
            g->setContentRatio(std::clamp(ratio, lower, upper));
    }

    void DockingGroupView::geometryChange(const QRectF &n, const QRectF &o) {
        QQuickItem::geometryChange(n, o);
        Q_EMIT geometryChanged();
    }
}

#include "moc_DockingGroupView.cpp"
