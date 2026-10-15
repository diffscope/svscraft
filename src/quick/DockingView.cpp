// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingView.h"
#include "DockingView_p.h"

#include <algorithm>
#include <cmath>
#include <functional>

#include <QJsonDocument>
#include <QJsonObject>

#include <SVSCraftQuick/DockingContext.h>
#include <SVSCraftQuick/DockingGroup.h>
#include <SVSCraftQuick/DockingLayout.h>
#include <SVSCraftQuick/DockingPanel.h>
#include <SVSCraftQuick/DockingSplit.h>
#include <SVSCraftQuick/private/DockingGroup_p.h>
#include <SVSCraftQuick/private/DockingHost_p.h>
#include <SVSCraftQuickImpl/private/DockingGeometry_p.h>

namespace SVS {
    DockingView::DockingView(QQuickItem *parent)
        : QQuickItem(parent), d_ptr(new DockingViewPrivate) {
        d_ptr->q_ptr = this;
    }

    DockingView::~DockingView() {
        Q_D(DockingView);
        if (d->host && d->host->d_ptr->view == this)
            d->host->d_ptr->view = nullptr;
        for (const auto &connection : d->connections)
            disconnect(connection);
    }

    DockingHost *DockingView::host() const {
        Q_D(const DockingView);
        return d->host;
    }

    void DockingView::setHost(DockingHost *h) {
        Q_D(DockingView);
        if (d->host == h)
            return;
        if (h && h->d_ptr->view && h->d_ptr->view != this)
            return;
        if (d->host && d->host->d_ptr->view == this)
            d->host->d_ptr->view = nullptr;
        for (const auto &c : d->connections)
            disconnect(c);
        d->connections.clear();
        d->host = h;
        d->previewKey.clear();
        d->previewRects.clear();
        if (h) {
            h->d_ptr->view = this;
            d->connections.append(
                connect(h->layout(), &DockingLayout::changed, this, &DockingView::polish)
            );
            d->connections.append(connect(h, &QObject::destroyed, this, [this] {
                Q_EMIT hostChanged();
                polish();
            }));
        }
        polish();
        Q_EMIT hostChanged();
    }

    QVariantList DockingView::groupFrames() const {
        Q_D(const DockingView);
        return d->groups;
    }

    QVariantList DockingView::splitHandles() const {
        Q_D(const DockingView);
        return d->handles;
    }

    qreal DockingView::padding() const {
        Q_D(const DockingView);
        return d->padding;
    }

    void DockingView::setPadding(qreal v) {
        Q_D(DockingView);
        if (!std::isfinite(v) || v < 0 || v == d->padding)
            return;
        d->padding = v;
        polish();
        Q_EMIT paddingChanged();
    }

    void DockingView::geometryChange(const QRectF &n, const QRectF &o) {
        QQuickItem::geometryChange(n, o);
        polish();
    }

    void DockingViewPrivate::arrange(DockingNode *n, const QRectF &r) {
        if (!n)
            return;
        rectangles.insert(n, r);
        if (auto g = qobject_cast<DockingGroup *>(n)) {
            DockingGroupPrivate::get(g)->layoutSize = r.size();
            groups.append(QVariant::fromValue(frame(g, -1, false, r)));
            return;
        }
        auto split = qobject_cast<DockingSplit *>(n);
        bool horizontal = split->orientation() == Qt::Horizontal;
        auto children = split->children();
        auto weights = split->weights();
        QVector<DockingExtent> extents;
        for (int i = 0; i < children.size(); ++i) {
            auto child = children[i];
            auto minimum = child->minimumSize();
            auto target = child->lockedSize();
            auto group = qobject_cast<DockingGroup *>(child);
            extents.append({horizontal ? minimum.width() : minimum.height(), horizontal ? target.width() : target.height(), weights[i].toDouble(), horizontal ? child->widthLocked() : child->heightLocked(), group && group->isCollapsed()});
        }
        auto sizes = dockingAllocate(
            std::max(qreal(0), (horizontal ? r.width() : r.height()) - children.size() + 1),
            extents
        );
        qreal position = horizontal ? r.x() : r.y();
        for (int i = 0; i < children.size(); ++i) {
            auto childRect = horizontal ? QRectF(position, r.y(), sizes[i], r.height())
                                        : QRectF(r.x(), position, r.width(), sizes[i]);
            arrange(children[i], childRect);
            position += sizes[i];
            if (i + 1 < children.size()) {
                auto rect = horizontal ? QRectF(position, r.y(), 1, r.height())
                                       : QRectF(r.x(), position, r.width(), 1);
                handles.append(QVariant::fromValue(frame(split, i, horizontal, rect)));
                ++position;
            }
        }
    }

    void DockingView::updatePolish() {
        Q_D(DockingView);
        const auto previousGroups = d->groups;
        const auto previousHandles = d->handles;
        d->groups.clear();
        d->handles.clear();
        d->rectangles.clear();
        d->usedFrames.clear();
        auto root = d->host ? d->host->layout()->root() : nullptr;
        auto hint = root ? root->preferredSize() : QSizeF(0, 0);
        setImplicitSize(hint.width() + 2 * padding(), hint.height() + 2 * padding());
        if (root)
            d->arrange(root, QRectF(padding(), padding(), std::max(qreal(0), width() - 2 * padding()), std::max(qreal(0), height() - 2 * padding())));
        for (const auto &key : d->frames.keys())
            if (!d->usedFrames.contains(key))
                d->frames.take(key)->deleteLater();
        if (d->groups != previousGroups || d->handles != previousHandles)
            Q_EMIT framesChanged();
    }

    DockingViewFrame *DockingViewPrivate::frame(DockingNode *node, int index, bool horizontal, const QRectF &rect) {
        auto key = node->nodeId() + ":" + QString::number(index) + ":" + QString::number(horizontal);
        usedFrames.insert(key);
        auto value = frames.value(key);
        if (value && value->node() != node) {
            value->deleteLater();
            value = nullptr;
        }
        if (!value) {
            value = new DockingViewFrame(node, index, horizontal, q_ptr);
            frames.insert(key, value);
        }
        value->setRect(rect);
        return value;
    }

    void DockingView::resizeSplit(DockingSplit *s, int index, qreal delta) {
        Q_D(DockingView);
        if (!s || !d->host || s->layout() != d->host->layout() || index < 0 ||
            index + 1 >= s->children().size() || !std::isfinite(delta))
            return;
        auto children = s->children();
        auto a = children[index], b = children[index + 1];
        if (auto g = qobject_cast<DockingGroup *>(a); g && g->isCollapsed())
            return;
        if (auto g = qobject_cast<DockingGroup *>(b); g && g->isCollapsed())
            return;
        bool horizontal = s->orientation() == Qt::Horizontal;
        qreal first = horizontal ? d->rectangles.value(a).width() : d->rectangles.value(a).height();
        qreal second =
            horizontal ? d->rectangles.value(b).width() : d->rectangles.value(b).height();
        qreal amin = horizontal ? a->minimumSize().width() : a->minimumSize().height();
        qreal bmin = horizontal ? b->minimumSize().width() : b->minimumSize().height();
        if (first + second <= 0 || first + second < amin + bmin)
            return;
        delta = std::clamp(delta, amin - first, second - bmin);
        first += delta;
        second -= delta;
        auto weights = s->weights();
        auto sum = weights[index].toDouble() + weights[index + 1].toDouble();
        weights[index] = sum * first / (first + second);
        weights[index + 1] = sum * second / (first + second);
        std::function<void(DockingNode *, qreal)> lock = [&](auto n, qreal value) {
            if (auto g = qobject_cast<DockingGroup *>(n)) {
                auto size = g->lockedSize();
                if (horizontal)
                    size.setWidth(value);
                else
                    size.setHeight(value);
                if (horizontal ? g->widthLocked() : g->heightLocked())
                    g->setLockedSize(size);
                auto expanded = g->expandedSize();
                if (horizontal)
                    expanded.setWidth(value);
                else
                    expanded.setHeight(value);
                g->setExpandedSize(expanded);
            } else if (auto branch = qobject_cast<DockingSplit *>(n)) {
                auto old =
                    horizontal ? branch->lockedSize().width() : branch->lockedSize().height();
                bool sameAxis = branch->orientation() == s->orientation();
                auto separators = branch->children().size() - 1;
                if (sameAxis)
                    old -= separators;
                for (auto child : branch->children()) {
                    auto target =
                        horizontal ? child->lockedSize().width() : child->lockedSize().height();
                    lock(child, sameAxis ? target * std::max(qreal(0), value - separators) / std::max(qreal(1), old) : value);
                }
            }
        };
        lock(a, first);
        lock(b, second);
        s->setWeights(weights);
    }

    QRectF DockingView::groupGeometry(const QString &id) {
        updatePolish();
        Q_D(DockingView);
        return host() ? d->rectangles.value(host()->layout()->node(id)) : QRectF{};
    }

    QRectF DockingView::previewPlacement(DockingPanel *panel, const QVariantMap &placement) const {
        if (!host() || !host()->context() ||
            !host()->context()->canPlaceAt(panel, placement).accepted())
            return {};
        Q_D(const DockingView);
        auto original = host()->context();
        QVariantList revisions;
        for (auto h : original->hosts())
            revisions.append(QVariantMap{{"hostId", h->hostId()}, {"revision", h->layout()->revision()}});
        QVariantMap key{
            {"context", QVariant::fromValue(original)},
            {"panel", panel->key()},
            {"minimum", panel->minimumSize()},
            {"preferred", panel->effectivePreferredSize()},
            {"size", size()},
            {"padding", padding()},
            {"revisions", revisions}
        };
        if (d->previewKey != key) {
            d->previewKey = key;
            d->previewRects.clear();
        }
        const auto placementValue = DockingPlacement::fromVariant(placement).toVariant();
        const auto placementKey = QJsonDocument(QJsonObject::fromVariantMap(placementValue)).toJson(QJsonDocument::Compact);
        if (d->previewRects.contains(placementKey))
            return d->previewRects.value(placementKey);
        if (d->previewRects.size() >= 128)
            d->previewRects.clear();
        d->previewRects.insert(placementKey, {});

        QList<DockingHost *> affectedHosts{host()};
        auto sourceGroup = panel->group();
        auto sourceHost = sourceGroup ? sourceGroup->layout()->host() : nullptr;
        if (sourceHost && sourceHost != host())
            affectedHosts.append(sourceHost);
        QSet<QString> affectedPanels{panel->key()};
        for (auto h : affectedHosts)
            for (auto group : h->layout()->groups())
                for (auto side : {DockingGroup::Leading, DockingGroup::Trailing})
                    for (auto p : group->panels(side))
                        affectedPanels.insert(p->key());

        DockingContext candidate;
        for (const auto &panelKey : affectedPanels) {
            auto p = original->panel(panelKey);
            auto copy = new DockingPanel(&candidate);
            copy->setKey(p->key());
            copy->setMinimumSize(p->minimumSize());
            copy->setPreferredSize(p->effectivePreferredSize());
            auto content = new QQuickItem;
            content->setParent(&candidate);
            copy->setContentItem(content);
            candidate.registerPanel(copy);
        }
        for (auto h : affectedHosts) {
            auto copy = new DockingHost(&candidate);
            copy->setHostId(h->hostId());
            candidate.registerHost(copy);
            if (!copy->layout()->restoreState(h->layout()->saveState()).accepted())
                return {};
        }
        auto p = candidate.panel(panel->key());
        auto result = candidate.placeAt(p, placement);
        if (!result.accepted() || !p->group())
            return {};
        auto target = candidate.host(host()->hostId());
        auto minimum = target->layout()->root()->minimumSize();
        if (minimum.width() + 2 * padding() > width() ||
            minimum.height() + 2 * padding() > height())
            return {};
        DockingView view;
        view.setHost(target);
        view.setPadding(padding());
        view.setSize(size());
        const auto rect = view.groupGeometry(p->group()->nodeId());
        d->previewRects.insert(placementKey, rect);
        return rect;
    }
}

#include "moc_DockingView.cpp"
#include "moc_DockingView_p.cpp"
