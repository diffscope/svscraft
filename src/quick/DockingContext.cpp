// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingContext.h"
#include "DockingContext_p.h"

#include <algorithm>
#include <functional>
#include <utility>

#include <QLoggingCategory>
#include <QQuickItem>
#include <QQuickWindow>
#include <QSet>

#include <SVSCraftQuick/DockingGroup.h>
#include <SVSCraftQuick/DockingHost.h>
#include <SVSCraftQuick/DockingLayout.h>
#include <SVSCraftQuick/DockingPanel.h>
#include <SVSCraftQuick/DockingRequest.h>
#include <SVSCraftQuick/DockingView.h>
#include <SVSCraftQuick/private/DockingDragSource_p.h>
#include <SVSCraftQuick/private/DockingGroup_p.h>
#include <SVSCraftQuick/private/DockingHost_p.h>
#include <SVSCraftQuick/private/DockingLayout_p.h>
#include <SVSCraftQuick/private/DockingPanel_p.h>

namespace SVS {
    Q_STATIC_LOGGING_CATEGORY(lcDockingDrag, "svscraft.docking.drag")

    DockingContext::DockingContext(QObject *parent)
        : QObject(parent), d_ptr(new DockingContextPrivate) {
        d_ptr->q_ptr = this;
    }

    DockingContext::~DockingContext() {
        Q_D(DockingContext);
        d->destroying = true;
        for (auto h : d->hosts)
            h->layout()->clear();
        for (auto p : d->panels) {
            if (auto item = p->contentItem()) {
                item->setVisible(false);
                item->setParentItem(nullptr);
            }
            if (auto item = p->headerItem()) {
                item->setVisible(false);
                item->setParentItem(nullptr);
            }
            p->d_ptr->context = nullptr;
            Q_EMIT p->placementChanged();
        }
        for (auto h : d->hosts) {
            h->d_ptr->context = nullptr;
            Q_EMIT h->contextChanged();
            if (h->role() == DockingHost::Floating)
                Q_EMIT h->closeRequested();
        }
    }

    DockingPanel *DockingContext::activePanel() const {
        Q_D(const DockingContext);
        return d->activePanel;
    }

    bool DockingContext::isUpdating() const {
        Q_D(const DockingContext);
        return d->updating;
    }

    QList<DockingHost *> DockingContext::hosts() const {
        Q_D(const DockingContext);
        return d->hosts;
    }

    QVariantList DockingContext::hostObjects() const {
        QVariantList v;
        for (auto h : hosts())
            v.append(QVariant::fromValue(h));
        return v;
    }

    QVariantList DockingContext::panelObjects() const {
        Q_D(const DockingContext);
        QVariantList v;
        for (auto p : d->panels)
            v.append(QVariant::fromValue(p));
        return v;
    }

    int DockingContext::placedPanelCount() const {
        Q_D(const DockingContext);
        int n = 0;
        for (auto p : d->panels)
            if (p->group())
                ++n;
        return n;
    }

    bool DockingContext::isDragging() const {
        Q_D(const DockingContext);
        return !d->draggedPanelKey.isEmpty();
    }

    QString DockingContext::draggedPanelKey() const {
        Q_D(const DockingContext);
        return d->draggedPanelKey;
    }

    void DockingContext::startDrag(DockingPanel *panel) {
        if (!panel || panel->context() != this || isDragging() || isUpdating())
            return;
        DockingDragSource::start(panel);
    }

    void DockingContext::cancelDrag() {
        endDrag(false);
    }

    void DockingContext::beginDrag(DockingPanel *panel, QObject *source) {
        Q_D(DockingContext);
        if (!panel || panel->context() != this || isDragging() || d->updating || d->destroying)
            return;
        qCDebug(lcDockingDrag) << this << "Idle -> Pending";
        d->draggedPanelKey = panel->key();
        if (source)
            d->dragConnections.append(
                connect(source, &QObject::destroyed, this, [this] { endDrag(false); })
            );
        d->dragConnections.append(
            connect(panel, &QObject::destroyed, this, [this] { endDrag(false); })
        );
        qCDebug(lcDockingDrag) << this << "Pending -> Dragging";
        Q_EMIT dragChanged();
    }

    void DockingContext::endDrag(bool accepted) {
        Q_D(DockingContext);
        if (!isDragging())
            return;
        qCDebug(lcDockingDrag) << this
                               << (accepted ? "Dragging -> Committing" : "Dragging -> Aborting");
        d->draggedPanelKey.clear();
        for (const auto &connection : d->dragConnections)
            disconnect(connection);
        d->dragConnections.clear();
        qCDebug(lcDockingDrag) << this << (accepted ? "Committing -> Idle" : "Aborting -> Idle");
        Q_EMIT dragChanged();
    }

    DockingHost *DockingContext::host(const QString &id) const {
        for (auto h : hosts())
            if (h->hostId() == id)
                return h;
        return nullptr;
    }

    DockingPanel *DockingContext::panel(const QString &key) const {
        Q_D(const DockingContext);
        return d->panels.value(key);
    }

    DockingGroup *DockingContext::groupForPanel(const QString &key) const {
        for (auto h : hosts())
            if (auto g = h->layout()->groupForPanel(key))
                return g;
        return nullptr;
    }

    DockingResult DockingContext::registerPanel(DockingPanel *p) {
        Q_D(DockingContext);
        if (!p || p->key().isEmpty() || p->context() && p->context() != this ||
            panel(p->key()) && panel(p->key()) != p)
            return {DockingResult::Rejected, "duplicateOrInvalidPanel"};
        if (p->context() == this)
            return {DockingResult::NoChange, {}};
        d->panels.insert(p->key(), p);
        p->d_ptr->context = this;
        connect(p, &DockingPanel::sizeHintsChanged, this, [this, p] {
            if (auto g = p->group())
                g->layout()->commit();
        });
        synchronize();
        return {};
    }

    DockingResult DockingContext::unregisterPanel(DockingPanel *p) {
        Q_D(DockingContext);
        if (!p || p->context() != this)
            return {DockingResult::Rejected, "sessionMismatch"};
        removePanelFromLayout(p);
        d->panels.remove(p->key());
        disconnect(p, nullptr, this, nullptr);
        p->d_ptr->context = nullptr;
        Q_EMIT p->placementChanged();
        synchronize();
        return {};
    }

    DockingResult DockingContext::registerHost(DockingHost *h) {
        Q_D(DockingContext);
        if (!h || h->context() && h->context() != this ||
            host(h->hostId()) && host(h->hostId()) != h)
            return {DockingResult::Rejected, "duplicateOrInvalidHost"};
        if (d->hosts.contains(h))
            return {DockingResult::NoChange, {}};
        d->hosts.append(h);
        h->d_ptr->context = this;
        connect(h, &QObject::destroyed, this, [this, h] {
            d_ptr->hosts.removeAll(h);
            synchronize();
        });
        Q_EMIT h->contextChanged();
        synchronize();
        return {};
    }

    DockingResult DockingContext::unregisterHost(DockingHost *h, bool removeContents) {
        Q_D(DockingContext);
        if (d->updating)
            return {DockingResult::Rejected, "contextBusy"};
        if (!d->hosts.contains(h))
            return {DockingResult::Rejected, "invalidHost"};
        if (!h->isEmpty() && !removeContents)
            return {DockingResult::Rejected, "nonemptyHost"};
        h->layout()->clear();
        d->hosts.removeAll(h);
        h->d_ptr->context = nullptr;
        Q_EMIT h->contextChanged();
        synchronize();
        return {};
    }

    DockingResult DockingContext::restoreLayouts(const QVariantMap &states, const QString &activePanelKey) {
        Q_D(DockingContext);
        if (d->updating || d->destroying)
            return {DockingResult::Rejected, "contextBusy"};

        QHash<DockingHost *, DockingLayoutState> candidates;
        for (auto it = states.cbegin(); it != states.cend(); ++it) {
            auto target = host(it.key());
            if (!target || !it.value().canConvert<QVariantMap>())
                return {DockingResult::Rejected, "invalidHost"};
            auto state = DockingLayoutState::fromVariant(it.value().toMap());
            if (!DockingLayout::validateState(state).accepted())
                return {DockingResult::Rejected, "invalidLayout"};
            candidates.insert(target, state);
        }

        QSet<QString> placed, shown;
        std::function<bool(const DockingNodeState &)> inspect = [&](const auto &node) {
            for (int side = 0; side < 2; ++side) {
                for (const auto &key : node.panelIds[side]) {
                    if (!panel(key) || placed.contains(key))
                        return false;
                    placed.insert(key);
                }
                if (!node.selected[side].isEmpty())
                    shown.insert(node.selected[side]);
            }
            for (const auto &child : node.children)
                if (!inspect(child))
                    return false;
            return true;
        };
        for (auto target : hosts()) {
            auto state = candidates.contains(target) ? candidates.value(target)
                                                     : target->layout()->saveState();
            if (state.hasRoot && !inspect(state.root))
                return {DockingResult::Rejected, "unregisteredOrPlacedPanel"};
        }
        if (!activePanelKey.isEmpty() && !shown.contains(activePanelKey))
            return {DockingResult::Rejected, "panelNotShown"};

        auto previousActive = d->activePanel;
        d->updating = true;
        for (auto target : hosts()) {
            if (!candidates.contains(target))
                continue;
            auto layout = target->layout();
            auto oldNodes = layout->d_ptr->nodes;
            auto state = candidates.value(target);
            layout->d_ptr->nodes.clear();
            layout->d_ptr->root = state.hasRoot ? layout->build(state.root, nullptr) : nullptr;
            for (auto node : oldNodes)
                node->deleteLater();
        }

        for (auto target : hosts())
            if (candidates.contains(target))
                target->layout()->commit();
        if (!activePanelKey.isEmpty())
            d->activePanel = panel(activePanelKey);
        d->updating = false;
        synchronize();

        if (!activePanelKey.isEmpty() && d->activePanel && d->activePanel != previousActive)
            Q_EMIT d->activePanel->activated();
        return {};
    }

    DockingResult DockingContext::canPlace(DockingPanel *p, const DockingPlacement &at) const {
        if (!p || p->context() != this)
            return {DockingResult::Rejected, "sessionMismatch"};
        auto target = host(at.hostId);
        if (!target)
            return {DockingResult::Rejected, "invalidHost"};
        if (at.kind == DockingPlacement::EmptyHost)
            return target->isEmpty() ? DockingResult{}
                                     : DockingResult{DockingResult::Rejected, "nonemptyHost"};
        auto g = qobject_cast<DockingGroup *>(target->layout()->node(at.groupId));
        if (!g || !target->layout()->groups().contains(g))
            return {DockingResult::Rejected, "invalidGroup"};
        if (at.kind == DockingPlacement::Edge) {
            if (at.edge != Qt::LeftEdge && at.edge != Qt::RightEdge && at.edge != Qt::TopEdge &&
                at.edge != Qt::BottomEdge)
                return {DockingResult::Rejected, "invalidEdge"};
        } else if (at.kind < DockingPlacement::Tab || at.kind > DockingPlacement::Opposite || at.side < 0 || at.side > 1)
            return {DockingResult::Rejected, "invalidPlacement"};
        if (at.kind != DockingPlacement::Edge && !at.anchor.isEmpty() &&
            !g->d_ptr->state.panelIds[at.side].contains(at.anchor))
            return {DockingResult::Rejected, "invalidAnchor"};
        return {};
    }

    DockingResult DockingContext::place(DockingPanel *p, const DockingPlacement &at) {
        Q_D(DockingContext);
        if (d->updating || d->destroying)
            return {DockingResult::Rejected, "contextBusy"};
        QPointer<DockingPanel> guard(p);
        auto result = canPlace(p, at);
        if (!result.accepted())
            return result;
        if (!p->contentItem())
            return {DockingResult::Rejected, "contentNotReady"};

        auto h = host(at.hostId);
        auto old = p->group();
        auto g = qobject_cast<DockingGroup *>(h->layout()->node(at.groupId));
        if (at.kind == DockingPlacement::Edge && old == g &&
            g->leadingPanels().size() + g->trailingPanels().size() == 1) {
            show(p);
            return {DockingResult::NoChange, {}};
        }
        if (at.kind == DockingPlacement::Edge && h->d_ptr->view &&
            h->d_ptr->view->previewPlacement(p, at.toVariant()).isEmpty())
            return {DockingResult::Rejected, "insufficientSize"};

        d->updating = true;
        if (at.kind == DockingPlacement::EmptyHost || at.kind == DockingPlacement::Edge) {
            auto anchor = g;
            g = h->layout()->makeGroup();
            if (at.kind == DockingPlacement::Edge)
                g->d_ptr->state.tabBarEdge = anchor->tabBarEdge();
            h->layout()->insertGroup(g, anchor, at.edge);
        }

        auto side = at.kind == DockingPlacement::Edge || at.kind == DockingPlacement::EmptyHost
                        ? 0
                        : at.side;
        auto &list = g->d_ptr->state.panelIds[side];
        int index =
            at.anchor.isEmpty() ? int(list.size()) : int(list.indexOf(at.anchor)) + at.after;
        if (old) {
            int os = old->sideOf(p);
            auto &oldList = old->d_ptr->state.panelIds[os];
            int oi = int(oldList.indexOf(p->key()));
            if (old == g && os == side && oi < index)
                --index;
            oldList.removeAll(p->key());
            if (old->d_ptr->state.selected[os] == p->key())
                old->d_ptr->state.selected[os].clear();
            if (old->d_ptr->state.lastSelected[os] == p->key())
                old->d_ptr->state.lastSelected[os].clear();
        }

        list.insert(std::clamp(index, 0, int(list.size())), p->key());
        g->d_ptr->state.selected[side] = g->d_ptr->state.lastSelected[side] = p->key();
        d->activePanel = p;
        if (old && old->layout() != g->layout())
            old->layout()->commit();
        g->layout()->commit();
        d->updating = false;
        synchronize();
        if (guard)
            activate(guard);
        return {};
    }

    DockingResult DockingContext::canPlaceAt(DockingPanel *p, const QVariantMap &v) const {
        return canPlace(p, DockingPlacement::fromVariant(v));
    }

    DockingResult DockingContext::placeAt(DockingPanel *p, const QVariantMap &v) {
        return place(p, DockingPlacement::fromVariant(v));
    }

    DockingRequest *DockingContext::requestPlaceAt(DockingPanel *p, const QVariantMap &v) {
        auto at = DockingPlacement::fromVariant(v);
        auto request = new DockingRequest(this, p, at, false);
        auto result = canPlace(p, at);
        if (!result.accepted())
            request->reject(result.reason);
        else if (p->contentItem())
            request->accept();
        else
            Q_EMIT placementRequested(request);
        return request;
    }

    DockingResult DockingContext::show(DockingPanel *p) {
        if (isUpdating())
            return {DockingResult::Rejected, "contextBusy"};
        QPointer<DockingPanel> guard(p);
        if (!p || p->context() != this)
            return {DockingResult::Rejected, "sessionMismatch"};
        if (!p->group()) {
            requestFloat(p);
            return {DockingResult::NoChange, {}};
        }
        auto r = p->group()->select(p);
        if (r.accepted() && guard)
            activate(guard);
        return r;
    }

    DockingResult DockingContext::hide(DockingPanel *p) {
        if (isUpdating())
            return {DockingResult::Rejected, "contextBusy"};
        if (!p || p->context() != this)
            return {DockingResult::Rejected, "sessionMismatch"};
        return p->group() ? p->group()->hide(p) : DockingResult{DockingResult::NoChange, {}};
    }

    DockingResult DockingContext::remove(DockingPanel *p) {
        if (isUpdating())
            return {DockingResult::Rejected, "contextBusy"};
        return removePanelFromLayout(p);
    }

    DockingResult DockingContext::removePanelFromLayout(DockingPanel *p) {
        if (!p || p->context() != this)
            return {DockingResult::Rejected, "sessionMismatch"};
        auto g = p->group();
        if (!g)
            return {DockingResult::NoChange, {}};
        for (int i = 0; i < 2; ++i) {
            g->d_ptr->state.panelIds[i].removeAll(p->key());
            if (g->d_ptr->state.selected[i] == p->key())
                g->d_ptr->state.selected[i].clear();
        }
        g->layout()->commit();
        return {};
    }

    DockingResult DockingContext::activate(DockingPanel *p, bool requestFocus) {
        Q_D(DockingContext);
        QPointer<DockingPanel> guard(p);
        if (d->updating)
            return {DockingResult::Rejected, "contextBusy"};
        if (!p || p->context() != this || !p->isShown())
            return {DockingResult::Rejected, "panelNotShown"};
        bool changed = d->activePanel != p;
        d->activePanel = p;
        if (changed)
            synchronize();
        if (!guard || !guard->isShown())
            return {DockingResult::NoChange, "panelNotShown"};
        if (requestFocus) {
            disconnect(d->focusConnection);
            QPointer<QQuickWindow> targetWindow = p->group()->layout()->host()->window();
            if (auto w = targetWindow.data()) {
                if (w->windowStates().testFlag(Qt::WindowMinimized))
                    w->setWindowStates(w->windowStates() & ~Qt::WindowMinimized);
                w->setVisible(true);
                w->raise();
                w->requestActivate();
            }
            if (guard && guard->contentItem()) {
                auto content = guard->contentItem();
                auto focusContent = [this, guard] {
                    if (!guard || activePanel() != guard || !guard->isShown() ||
                        !guard->contentItem())
                        return;
                    auto item = guard->contentItem();
                    auto focused = item->window() ? item->window()->activeFocusItem() : nullptr;
                    if (!focused || focused != item && !item->isAncestorOf(focused))
                        item->forceActiveFocus();
                };
                if (!targetWindow || content->window() == targetWindow)
                    focusContent();
                else
                    d->focusConnection =
                        connect(content, &QQuickItem::windowChanged, this, [this, guard, targetWindow, focusContent](QQuickWindow *window) {
                            if (!guard || activePanel() != guard || !guard->isShown() ||
                                !targetWindow) {
                                disconnect(d_ptr->focusConnection);
                                return;
                            }
                            if (window != targetWindow)
                                return;
                            disconnect(d_ptr->focusConnection);
                            focusContent();
                        });
            }
        }
        if (guard)
            Q_EMIT guard->activated();
        return {};
    }

    void DockingContext::requestFloat(DockingPanel *p) {
        Q_D(DockingContext);
        if (!d->updating && !d->destroying && p && p->context() == this)
            Q_EMIT floatingRequested(new DockingRequest(this, p, {}, true));
    }

    void DockingContext::synchronize() {
        Q_D(DockingContext);
        if (d->updating || d->destroying)
            return;
        if (d->synchronizing) {
            d->synchronizePending = true;
            return;
        }
        d->synchronizing = true;
        auto previousActive = d->activePanel;
        if (d->activePanel && !d->activePanel->isShown())
            d->activePanel = nullptr;
        if (!d->activePanel)
            for (auto h : hosts()) {
                for (auto g : h->layout()->groups()) {
                    if (auto p = g->leadingPanel()) {
                        d->activePanel = p;
                        break;
                    }
                    if (auto p = g->trailingPanel()) {
                        d->activePanel = p;
                        break;
                    }
                }
                if (d->activePanel)
                    break;
            }
        QList<QPointer<DockingPanel>> panels;
        for (auto p : d->panels)
            panels.append(p);
        for (auto p : panels) {
            if (!p || p->context() != this)
                continue;
            if (!p->isShown()) {
                QPointer<QQuickItem> content = p->contentItem();
                if (content)
                    content->setVisible(false);
                if (content)
                    content->setParentItem(nullptr);
            }
            if (p)
                Q_EMIT p->placementChanged();
        }
        Q_EMIT changed();
        if (d->activePanel && d->activePanel != previousActive)
            Q_EMIT d->activePanel->activated();
        for (auto h : hosts())
            if (h->role() == DockingHost::Floating && h->isEmpty())
                QMetaObject::invokeMethod(
                    h,
                    [this, h] {
                        if (h->context() == this && h->isEmpty())
                            Q_EMIT h->closeRequested();
                    },
                    Qt::QueuedConnection
                );
        d->synchronizing = false;
        if (std::exchange(d->synchronizePending, false))
            QMetaObject::invokeMethod(this, [this] { synchronize(); }, Qt::QueuedConnection);
    }
}

#include "moc_DockingContext.cpp"
