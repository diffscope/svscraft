// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingLayout.h"
#include "DockingLayout_p.h"

#include <algorithm>
#include <cmath>
#include <functional>

#include <QSet>
#include <QUuid>

#include <SVSCraftQuick/DockingContext.h>
#include <SVSCraftQuick/DockingGroup.h>
#include <SVSCraftQuick/DockingHost.h>
#include <SVSCraftQuick/DockingPanel.h>
#include <SVSCraftQuick/DockingSplit.h>
#include <SVSCraftQuick/private/DockingGroup_p.h>
#include <SVSCraftQuick/private/DockingNode_p.h>
#include <SVSCraftQuick/private/DockingSplit_p.h>

namespace SVS {
    DockingLayout::DockingLayout(DockingHost *host) : QObject(host), d_ptr(new DockingLayoutPrivate) {
        d_ptr->q_ptr = this;
        d_ptr->host = host;
    }

    DockingLayout::~DockingLayout() = default;

    DockingHost *DockingLayout::host() const {
        Q_D(const DockingLayout);
        return d->host;
    }

    DockingNode *DockingLayout::root() const {
        Q_D(const DockingLayout);
        return d->root;
    }

    quint64 DockingLayout::revision() const {
        Q_D(const DockingLayout);
        return d->revision;
    }

    DockingNode *DockingLayout::node(const QString &id) const {
        Q_D(const DockingLayout);
        return d->nodes.value(id);
    }

    QList<DockingGroup *> DockingLayout::groups() const {
        QList<DockingGroup *> result;
        std::function<void(DockingNode *)> visit = [&](auto n) {
            if (auto g = qobject_cast<DockingGroup *>(n))
                result.append(g);
            else if (auto s = qobject_cast<DockingSplit *>(n))
                for (auto c : s->children())
                    visit(c);
        };
        visit(root());
        return result;
    }

    DockingGroup *DockingLayout::groupForPanel(const QString &key) const {
        for (auto g : groups())
            for (int side = 0; side < 2; ++side)
                if (g->d_ptr->state.panelIds[side].contains(key))
                    return g;
        return nullptr;
    }

    QVariantList DockingLayout::groupObjects() const {
        QVariantList result;
        for (auto group : groups())
            result.append(QVariant::fromValue(group));
        return result;
    }

    DockingLayoutState DockingLayout::saveState() const {
        std::function<DockingNodeState(DockingNode *)> visit = [&](auto n) {
            if (auto g = qobject_cast<DockingGroup *>(n))
                return g->d_ptr->state;
            auto split = qobject_cast<DockingSplit *>(n);
            DockingNodeState s;
            s.kind = "split";
            s.nodeId = n->nodeId();
            s.orientation = split->orientation();
            s.weights = split->d_ptr->weights;
            for (auto c : split->children())
                s.children.append(visit(c));
            return s;
        };
        return root() ? DockingLayoutState{true, visit(root())} : DockingLayoutState{};
    }

    DockingResult DockingLayout::validateState(const DockingLayoutState &state) {
        QSet<QString> nodes, panels;
        int count = 0;
        std::function<bool(const DockingNodeState &, int)> visit = [&](const auto &s, int depth) {
            if (++count > 4096 || depth > 64 || s.nodeId.isEmpty() || nodes.contains(s.nodeId))
                return false;
            nodes.insert(s.nodeId);
            if (s.kind == "split") {
                if ((s.orientation != Qt::Horizontal && s.orientation != Qt::Vertical) || s.children.size() < 2 || s.children.size() != s.weights.size())
                    return false;
                for (auto w : s.weights)
                    if (!std::isfinite(w) || w <= 0)
                        return false;
                for (const auto &child : s.children)
                    if (!visit(child, depth + 1))
                        return false;
                return true;
            }
            if (s.kind != "group" || !s.children.isEmpty() || s.panelIds[0].isEmpty() && s.panelIds[1].isEmpty())
                return false;
            if (s.tabBarEdge != Qt::LeftEdge && s.tabBarEdge != Qt::RightEdge && s.tabBarEdge != Qt::TopEdge && s.tabBarEdge != Qt::BottomEdge)
                return false;
            if (!std::isfinite(s.contentRatio) || s.contentRatio <= 0 || s.contentRatio >= 1)
                return false;
            for (auto size : {s.lockedSize, s.expandedSize})
                if (!std::isfinite(size.width()) || !std::isfinite(size.height()) || size.width() <= 0 || size.height() <= 0)
                    return false;
            for (int i = 0; i < 2; ++i) {
                if (!s.selected[i].isEmpty() && !s.panelIds[i].contains(s.selected[i]))
                    return false;
                if (!s.lastSelected[i].isEmpty() && !s.panelIds[i].contains(s.lastSelected[i]))
                    return false;
                for (const auto &key : s.panelIds[i]) {
                    if (key.isEmpty() || panels.contains(key) || panels.size() >= 4096)
                        return false;
                    panels.insert(key);
                }
            }
            return true;
        };
        return !state.hasRoot || visit(state.root, 0) ? DockingResult{} : DockingResult{DockingResult::Rejected, "invalidLayout"};
    }

    DockingNode *DockingLayout::build(const DockingNodeState &s, DockingSplit *parent) {
        Q_D(DockingLayout);
        DockingNode *n;
        if (s.kind == "group") {
            auto g = new DockingGroup(s.nodeId, this);
            g->d_ptr->state = s;
            g->d_ptr->hasExpandedSize = true;
            n = g;
        } else {
            auto split = new DockingSplit(s.nodeId, this);
            split->d_ptr->orientation = s.orientation;
            split->d_ptr->weights = s.weights;
            for (const auto &child : s.children)
                split->d_ptr->children.append(build(child, split));
            n = split;
        }
        n->DockingNode::d_ptr->parent = parent;
        d->nodes.insert(s.nodeId, n);
        return n;
    }

    DockingResult DockingLayout::restoreState(const DockingLayoutState &state) {
        Q_D(DockingLayout);
        if (host()->context() && host()->context()->isUpdating())
            return {DockingResult::Rejected, "contextBusy"};
        auto result = validateState(state);
        if (!result.accepted())
            return result;
        auto context = host()->context();
        std::function<bool(const DockingNodeState &)> check = [&](const auto &n) {
            for (const auto &list : n.panelIds)
                for (const auto &key : list) {
                    if (!context || !context->panel(key))
                        return false;
                    auto existing = context->groupForPanel(key);
                    if (existing && existing->layout() != this)
                        return false;
                }
            for (const auto &c : n.children)
                if (!check(c))
                    return false;
            return true;
        };
        if (state.hasRoot && !check(state.root))
            return {DockingResult::Rejected, "unregisteredOrPlacedPanel"};
        auto oldNodes = d->nodes;
        d->nodes.clear();
        d->root = state.hasRoot ? build(state.root, nullptr) : nullptr;
        for (auto n : oldNodes)
            n->deleteLater();
        commit();
        return {};
    }

    QVariantMap DockingLayout::save() const {
        return saveState().toVariant();
    }

    DockingResult DockingLayout::restore(const QVariantMap &v) {
        return restoreState(DockingLayoutState::fromVariant(v));
    }

    DockingResult DockingLayout::clear() {
        return restoreState({});
    }

    DockingGroup *DockingLayout::createGroup(DockingPanel *panel, DockingGroup *anchor, Qt::Edge edge) {
        if (!host()->context() || !panel || root() && !anchor || anchor && anchor->layout() != this)
            return nullptr;
        DockingPlacement at;
        at.hostId = host()->hostId();
        at.kind = root() ? DockingPlacement::Edge : DockingPlacement::EmptyHost;
        at.groupId = anchor ? anchor->nodeId() : QString{};
        at.edge = edge;
        return host()->context()->place(panel, at).accepted() ? panel->group() : nullptr;
    }

    DockingGroup *DockingLayout::makeGroup() {
        Q_D(DockingLayout);
        auto group = new DockingGroup(QUuid::createUuid().toString(QUuid::WithoutBraces), this);
        d->nodes.insert(group->nodeId(), group);
        return group;
    }

    DockingResult DockingLayout::insertGroup(DockingGroup *group, DockingGroup *anchor, Qt::Edge edge) {
        Q_D(DockingLayout);
        if (!group || group->layout() != this || group->parentNode() || group == root() || group == anchor)
            return {DockingResult::Rejected, "invalidGroup"};
        if (!root()) {
            d->root = group;
            return {};
        }
        if (!anchor || anchor->layout() != this || !groups().contains(anchor))
            return {DockingResult::Rejected, "invalidAnchor"};
        if (edge != Qt::LeftEdge && edge != Qt::RightEdge && edge != Qt::TopEdge && edge != Qt::BottomEdge)
            return {DockingResult::Rejected, "invalidEdge"};
        auto axis = edge == Qt::LeftEdge || edge == Qt::RightEdge ? Qt::Horizontal : Qt::Vertical;
        bool before = edge == Qt::LeftEdge || edge == Qt::TopEdge;
        auto p = anchor->parentNode();
        if (p && p->orientation() == axis) {
            int index = p->d_ptr->children.indexOf(anchor);
            auto weight = p->d_ptr->weights[index] / 2;
            p->d_ptr->weights[index] = weight;
            p->d_ptr->children.insert(index + !before, group);
            p->d_ptr->weights.insert(index + !before, weight);
            group->DockingNode::d_ptr->parent = p;
        } else {
            auto split = new DockingSplit(QUuid::createUuid().toString(QUuid::WithoutBraces), this);
            d->nodes.insert(split->nodeId(), split);
            split->d_ptr->orientation = axis;
            split->d_ptr->children = before ? QList<DockingNode *>{group, anchor} : QList<DockingNode *>{anchor, group};
            split->d_ptr->weights = {1, 1};
            split->DockingNode::d_ptr->parent = p;
            if (p)
                p->d_ptr->children[p->d_ptr->children.indexOf(anchor)] = split;
            else
                d->root = split;
            anchor->DockingNode::d_ptr->parent = split;
            group->DockingNode::d_ptr->parent = split;
        }
        return {};
    }

    DockingResult DockingLayout::removeGroup(DockingGroup *g) {
        if (host()->context() && host()->context()->isUpdating())
            return {DockingResult::Rejected, "contextBusy"};
        if (!g || g->layout() != this)
            return {DockingResult::Rejected, "invalidGroup"};
        g->d_ptr->state.panelIds[0].clear();
        g->d_ptr->state.panelIds[1].clear();
        commit();
        return {};
    }

    void DockingLayout::normalize() {
        Q_D(DockingLayout);
        std::function<DockingNode *(DockingNode *, DockingSplit *)> visit = [&](auto n, auto parent) -> DockingNode * {
            if (!n)
                return nullptr;
            n->DockingNode::d_ptr->parent = parent;
            if (auto g = qobject_cast<DockingGroup *>(n)) {
                auto &s = g->d_ptr->state;
                if (s.panelIds[0].isEmpty() && s.panelIds[1].isEmpty()) {
                    d->nodes.remove(n->nodeId());
                    n->deleteLater();
                    return nullptr;
                }
                for (int i = 0; i < 2; ++i) {
                    if (!s.panelIds[i].contains(s.selected[i]))
                        s.selected[i].clear();
                    if (!s.panelIds[i].contains(s.lastSelected[i]))
                        s.lastSelected[i].clear();
                }
                if (!g->isCollapsible() && g->isCollapsed())
                    for (int i = 0; i < 2; ++i)
                        if (!s.panelIds[i].isEmpty()) {
                            s.selected[i] = s.lastSelected[i].isEmpty() ? s.panelIds[i].first() : s.lastSelected[i];
                            break;
                        }
                return n;
            }
            auto split = qobject_cast<DockingSplit *>(n);
            for (int i = int(split->d_ptr->children.size()) - 1; i >= 0; --i) {
                auto child = visit(split->d_ptr->children[i], split);
                if (!child) {
                    split->d_ptr->children.removeAt(i);
                    split->d_ptr->weights.removeAt(i);
                } else
                    split->d_ptr->children[i] = child;
            }
            if (split->children().size() <= 1) {
                auto child = split->children().value(0);
                d->nodes.remove(n->nodeId());
                n->deleteLater();
                return visit(child, parent);
            }
            for (int i = int(split->d_ptr->children.size()) - 1; i >= 0; --i) {
                auto nested = qobject_cast<DockingSplit *>(split->d_ptr->children[i]);
                if (!nested || nested->orientation() != split->orientation())
                    continue;
                qreal total = 0;
                for (auto weight : nested->d_ptr->weights)
                    total += weight;
                auto outer = split->d_ptr->weights.takeAt(i);
                split->d_ptr->children.removeAt(i);
                for (int j = 0; j < nested->d_ptr->children.size(); ++j) {
                    auto child = nested->d_ptr->children[j];
                    child->DockingNode::d_ptr->parent = split;
                    split->d_ptr->children.insert(i + j, child);
                    split->d_ptr->weights.insert(i + j, outer * nested->d_ptr->weights[j] / total);
                }
                d->nodes.remove(nested->nodeId());
                nested->deleteLater();
            }
            return n;
        };
        d->root = visit(d->root, nullptr);
    }

    void DockingLayout::commit() {
        Q_D(DockingLayout);
        if (d->committing)
            return;
        d->committing = true;
        normalize();
        ++d->revision;
        for (auto n : d->nodes)
            Q_EMIT n->changed();
        d->committing = false;
        Q_EMIT changed();
        if (host()->context())
            host()->context()->synchronize();
    }
}

#include "moc_DockingLayout.cpp"
