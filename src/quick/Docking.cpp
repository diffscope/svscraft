// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "Docking.h"
#include "Docking_p.h"

#include <QQuickItem>
#include <QQuickWindow>

#include <SVSCraftQuick/DockingContext.h>
#include <SVSCraftQuick/DockingGroup.h>
#include <SVSCraftQuick/DockingHost.h>
#include <SVSCraftQuick/DockingLayout.h>

namespace SVS {
    Docking::Docking(QObject *parent) : QObject(parent), d_ptr(new DockingPrivate) {
        d_ptr->q_ptr = this;
        if (auto item = qobject_cast<QQuickItem *>(parent)) {
            auto updateWindow = [this, item] {
                disconnect(d_ptr->windowConnection);
                if (item->window())
                    d_ptr->windowConnection = connect(item->window(), &QWindow::visibilityChanged, this, &Docking::updateVisibility);
                Q_EMIT windowChanged();
                updateVisibility();
            };
            connect(item, &QQuickItem::windowChanged, this, updateWindow);
            updateWindow();
            connect(item, &QQuickItem::visibleChanged, this, &Docking::updateVisibility);
        }
    }

    Docking::~Docking() = default;

    Docking *Docking::qmlAttachedProperties(QObject *object) {
        return new Docking(object);
    }

    DockingPanel *Docking::panel() const {
        Q_D(const Docking);
        return d->panel;
    }

    void Docking::setPanel(DockingPanel *p) {
        Q_D(Docking);
        if (d->panel == p)
            return;
        for (const auto &c : d->connections)
            disconnect(c);
        d->connections.clear();
        d->panel = p;
        if (p) {
            d->connections.append(connect(p, &DockingPanel::placementChanged, this, [this] {
                Q_EMIT changed();
                updateVisibility();
            }));
            d->connections.append(connect(p, &DockingPanel::activated, this, &Docking::activated));
            d->connections.append(connect(p, &QObject::destroyed, this, [this] {
                Q_EMIT changed();
                updateVisibility();
            }));
        }
        Q_EMIT changed();
        updateVisibility();
    }

    DockingContext *Docking::context() const {
        return panel() ? panel()->context() : nullptr;
    }

    DockingGroup *Docking::group() const {
        return panel() ? panel()->group() : nullptr;
    }

    int Docking::side() const {
        return group() ? group()->sideOf(panel()) : -1;
    }

    DockingHost *Docking::host() const {
        return group() ? group()->layout()->host() : nullptr;
    }

    QQuickWindow *Docking::window() const {
        auto item = qobject_cast<QQuickItem *>(parent());
        return item ? item->window() : nullptr;
    }

    bool Docking::visible() const {
        return panel() && panel()->isShown();
    }

    bool Docking::effectiveVisible() const {
        auto item = qobject_cast<QQuickItem *>(parent());
        return visible() && item && item->isVisible() && window() && window()->isVisible() && window()->visibility() != QWindow::Minimized;
    }

    void Docking::updateVisibility() {
        Q_D(Docking);
        auto logical = visible();
        auto effective = effectiveVisible();
        bool logicalChanged = d->lastVisible != logical;
        bool effectiveChanged = d->lastEffectiveVisible != effective;
        d->lastVisible = logical;
        d->lastEffectiveVisible = effective;
        if (logicalChanged)
            Q_EMIT visibleChanged();
        if (effectiveChanged)
            Q_EMIT effectiveVisibleChanged();
    }

    bool Docking::active() const {
        return panel() && panel()->isActive();
    }

    DockingResult Docking::show() {
        return context() ? context()->show(panel()) : DockingResult{DockingResult::Rejected, "unregisteredPanel"};
    }

    DockingResult Docking::hide() {
        return context() ? context()->hide(panel()) : DockingResult{DockingResult::Rejected, "unregisteredPanel"};
    }

    DockingResult Docking::remove() {
        return context() ? context()->remove(panel()) : DockingResult{DockingResult::Rejected, "unregisteredPanel"};
    }

    DockingResult Docking::activate() {
        return context() ? context()->activate(panel()) : DockingResult{DockingResult::Rejected, "unregisteredPanel"};
    }

    void Docking::floatPanel() {
        if (context())
            context()->requestFloat(panel());
    }
}

#include "moc_Docking.cpp"
