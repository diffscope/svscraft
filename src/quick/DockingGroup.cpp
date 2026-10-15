// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingGroup.h"
#include "DockingGroup_p.h"

#include <algorithm>
#include <cmath>

#include <SVSCraftQuick/DockingContext.h>
#include <SVSCraftQuick/DockingHost.h>
#include <SVSCraftQuick/DockingLayout.h>
#include <SVSCraftQuick/DockingPanel.h>
#include <SVSCraftQuick/DockingSplit.h>

namespace SVS {
    DockingGroup::DockingGroup(const QString &id, DockingLayout *layout) : DockingNode(Group, id, layout), d_ptr(new DockingGroupPrivate) {
        d_ptr->q_ptr = this;
        d_ptr->state.nodeId = id;
    }

    DockingGroup::~DockingGroup() = default;

    Qt::Edge DockingGroup::tabBarEdge() const {
        Q_D(const DockingGroup);
        return d->state.tabBarEdge;
    }

    void DockingGroup::setTabBarEdge(Qt::Edge v) {
        Q_D(DockingGroup);
        if (d->state.tabBarEdge == v || v != Qt::LeftEdge && v != Qt::RightEdge && v != Qt::TopEdge && v != Qt::BottomEdge)
            return;
        d->state.tabBarEdge = v;
        layout()->commit();
    }

    bool DockingGroup::autoHide() const {
        Q_D(const DockingGroup);
        return d->state.autoHide;
    }

    void DockingGroup::setAutoHide(bool v) {
        Q_D(DockingGroup);
        if (d->state.autoHide == v)
            return;
        d->state.autoHide = v;
        layout()->commit();
    }

    bool DockingGroup::widthLocked() const {
        Q_D(const DockingGroup);
        return d->state.widthLocked;
    }

    void DockingGroup::setWidthLocked(bool v) {
        Q_D(DockingGroup);
        if (d->state.widthLocked == v)
            return;
        if (v && !d->explicitLockedSize) {
            qreal value = !isCollapsed() && d->layoutSize.width() > 0 ? d->layoutSize.width() : d->hasExpandedSize ? expandedSize().width()
                                                                                                                   : d->sizeHint(true, true).width();
            if (value <= 0)
                return;
            d->state.lockedSize.setWidth(value);
        }
        d->state.widthLocked = v;
        layout()->commit();
    }

    bool DockingGroup::heightLocked() const {
        Q_D(const DockingGroup);
        return d->state.heightLocked;
    }

    void DockingGroup::setHeightLocked(bool v) {
        Q_D(DockingGroup);
        if (d->state.heightLocked == v)
            return;
        if (v && !d->explicitLockedSize) {
            qreal value = !isCollapsed() && d->layoutSize.height() > 0 ? d->layoutSize.height() : d->hasExpandedSize ? expandedSize().height()
                                                                                                                     : d->sizeHint(true, true).height();
            if (value <= 0)
                return;
            d->state.lockedSize.setHeight(value);
        }
        d->state.heightLocked = v;
        layout()->commit();
    }

    QSizeF DockingGroup::lockedSize() const {
        Q_D(const DockingGroup);
        return d->state.lockedSize;
    }

    void DockingGroup::setLockedSize(const QSizeF &v) {
        Q_D(DockingGroup);
        if (!std::isfinite(v.width()) || !std::isfinite(v.height()) || v.width() <= 0 || v.height() <= 0)
            return;
        d->explicitLockedSize = true;
        if (d->state.lockedSize == v)
            return;
        d->state.lockedSize = v;
        layout()->commit();
    }

    QSizeF DockingGroup::expandedSize() const {
        Q_D(const DockingGroup);
        return d->state.expandedSize;
    }

    void DockingGroup::setExpandedSize(const QSizeF &v) {
        Q_D(DockingGroup);
        if (!std::isfinite(v.width()) || !std::isfinite(v.height()) || v.width() <= 0 || v.height() <= 0)
            return;
        d->hasExpandedSize = true;
        if (d->state.expandedSize == v)
            return;
        d->state.expandedSize = v;
        layout()->commit();
    }

    qreal DockingGroup::contentRatio() const {
        Q_D(const DockingGroup);
        return d->state.contentRatio;
    }

    void DockingGroup::setContentRatio(qreal v) {
        Q_D(DockingGroup);
        if (!std::isfinite(v) || v <= 0 || v >= 1 || d->state.contentRatio == v)
            return;
        d->state.contentRatio = v;
        layout()->commit();
    }

    bool DockingGroup::isCollapsible() const {
        if (!parentNode())
            return false;
        auto axis = tabBarEdge() == Qt::TopEdge || tabBarEdge() == Qt::BottomEdge ? Qt::Vertical : Qt::Horizontal;
        return parentNode()->orientation() == axis;
    }

    bool DockingGroup::isCollapsed() const {
        Q_D(const DockingGroup);
        return d->state.selected[0].isEmpty() && d->state.selected[1].isEmpty();
    }

    bool DockingGroup::isBarHidden() const {
        Q_D(const DockingGroup);
        if (!autoHide())
            return false;
        bool any = false;
        for (int i = 0; i < 2; ++i)
            if (!d->state.panelIds[i].isEmpty()) {
                any = true;
                if (d->state.selected[i].isEmpty())
                    return false;
            }
        return any;
    }

    QList<DockingPanel *> DockingGroup::panels(Side side) const {
        Q_D(const DockingGroup);
        QList<DockingPanel *> result;
        auto context = layout()->host()->context();
        if (side != Leading && side != Trailing)
            return result;
        for (const auto &id : d->state.panelIds[side])
            if (context)
                if (auto p = context->panel(id))
                    result.append(p);
        return result;
    }

    QVariantList DockingGroup::leadingPanels() const {
        QVariantList v;
        for (auto p : panels(Leading))
            v.append(QVariant::fromValue(p));
        return v;
    }

    QVariantList DockingGroup::trailingPanels() const {
        QVariantList v;
        for (auto p : panels(Trailing))
            v.append(QVariant::fromValue(p));
        return v;
    }

    DockingPanel *DockingGroup::selectedPanel(Side side) const {
        Q_D(const DockingGroup);
        auto c = layout()->host()->context();
        return c && (side == Leading || side == Trailing) ? c->panel(d->state.selected[side]) : nullptr;
    }

    DockingPanel *DockingGroup::leadingPanel() const {
        return selectedPanel(Leading);
    }

    DockingPanel *DockingGroup::trailingPanel() const {
        return selectedPanel(Trailing);
    }

    int DockingGroup::sideOf(DockingPanel *p) const {
        Q_D(const DockingGroup);
        if (!p || p->context() != layout()->host()->context())
            return -1;
        for (int i = 0; i < 2; ++i)
            if (d->state.panelIds[i].contains(p->key()))
                return i;
        return -1;
    }

    DockingResult DockingGroup::select(DockingPanel *p) {
        Q_D(DockingGroup);
        if (auto context = layout()->host()->context(); context && context->isUpdating())
            return {DockingResult::Rejected, "contextBusy"};
        int side = sideOf(p);
        if (side < 0)
            return {DockingResult::Rejected, "panelNotInGroup"};
        if (d->state.selected[side] == p->key())
            return {DockingResult::NoChange, {}};
        d->state.selected[side] = d->state.lastSelected[side] = p->key();
        layout()->commit();
        return {};
    }

    bool DockingGroup::canHide(DockingPanel *p) const {
        int side = sideOf(p);
        return side >= 0 && (selectedPanel(Side(side)) != p || isCollapsible() || selectedPanel(Side(1 - side)) || panels(Side(side)).size() > 1);
    }

    DockingResult DockingGroup::hide(DockingPanel *p) {
        Q_D(DockingGroup);
        if (auto context = layout()->host()->context(); context && context->isUpdating())
            return {DockingResult::Rejected, "contextBusy"};
        int side = sideOf(p);
        if (side < 0)
            return {DockingResult::Rejected, "panelNotInGroup"};
        if (d->state.selected[side] != p->key())
            return {DockingResult::NoChange, {}};
        if (!canHide(p))
            return {DockingResult::Rejected, "cannotCollapseGroup"};
        if (!isCollapsible() && d->state.selected[1 - side].isEmpty()) {
            const auto &ids = d->state.panelIds[side];
            auto index = ids.indexOf(p->key());
            d->state.selected[side] = d->state.lastSelected[side] = ids[index + 1 < ids.size() ? index + 1 : index - 1];
        } else
            d->state.selected[side].clear();
        layout()->commit();
        return {};
    }

    QSizeF DockingGroupPrivate::sizeHint(bool preferred, bool expanded) const {
        Q_Q(const DockingGroup);
        QSizeF sizes[2]{{0, 0}, {0, 0}};
        bool open[2]{false, false};
        for (int i = 0; i < 2; ++i) {
            auto p = q->selectedPanel(DockingGroup::Side(i));
            if (!p && expanded && q->isCollapsed()) {
                auto context = q->layout()->host()->context();
                if (context)
                    p = context->panel(state.lastSelected[i].isEmpty() ? state.panelIds[i].value(0) : state.lastSelected[i]);
            }
            if (!p)
                continue;
            sizes[i] = preferred ? p->effectivePreferredSize() : p->minimumSize();
            sizes[i].rheight() += 32;
            open[i] = true;
        }
        bool horizontal = state.tabBarEdge == Qt::TopEdge || state.tabBarEdge == Qt::BottomEdge;
        QSizeF size = open[0] ? sizes[0] : sizes[1];
        if (open[0] && open[1]) {
            if (horizontal)
                size = {std::max(sizes[0].width() / state.contentRatio, sizes[1].width() / (1 - state.contentRatio)) + 1, std::max(sizes[0].height(), sizes[1].height())};
            else
                size = {std::max(sizes[0].width(), sizes[1].width()), std::max(sizes[0].height() / state.contentRatio, sizes[1].height() / (1 - state.contentRatio)) + 1};
        }
        if (!q->isBarHidden()) {
            if (horizontal)
                size.rheight() += 32;
            else
                size.rwidth() += 32;
        }
        return size;
    }

    QSizeF DockingGroup::minimumSize() const {
        Q_D(const DockingGroup);
        return d->sizeHint(false);
    }

    DockingResult DockingGroup::insertPanel(DockingPanel *panel, Side side, const QString &anchor, bool after) {
        auto context = layout()->host()->context();
        return context ? context->place(panel, {layout()->host()->hostId(), nodeId(), DockingPlacement::Tab, int(side), anchor, after}) : DockingResult{DockingResult::Rejected, "unregisteredHost"};
    }

    DockingResult DockingGroup::removePanel(DockingPanel *panel) {
        if (sideOf(panel) < 0)
            return {DockingResult::Rejected, "panelNotInGroup"};
        return layout()->host()->context()->remove(panel);
    }

    QSizeF DockingGroup::preferredSize() const {
        Q_D(const DockingGroup);
        return d->sizeHint(true);
    }
}

#include "moc_DockingGroup.cpp"
