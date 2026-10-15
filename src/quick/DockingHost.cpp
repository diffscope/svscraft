// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingHost.h"
#include "DockingHost_p.h"

#include <QUuid>

#include <SVSCraftQuick/DockingContext.h>
#include <SVSCraftQuick/DockingLayout.h>

namespace SVS {
    DockingHost::DockingHost(QObject *parent) : QObject(parent), d_ptr(new DockingHostPrivate) {
        Q_D(DockingHost);
        d->q_ptr = this;
        d->id = QUuid::createUuid().toString(QUuid::WithoutBraces);
        d->layout = new DockingLayout(this);
        connect(d->layout, &DockingLayout::changed, this, &DockingHost::emptyChanged);
    }

    DockingHost::~DockingHost() {
        if (d_ptr->context)
            d_ptr->context->unregisterHost(this, true);
    }

    QString DockingHost::hostId() const {
        Q_D(const DockingHost);
        return d->id;
    }

    void DockingHost::setHostId(const QString &v) {
        Q_D(DockingHost);
        if (d->context || v.isEmpty() || d->id == v)
            return;
        d->id = v;
        Q_EMIT hostIdChanged();
    }

    DockingHost::Role DockingHost::role() const {
        Q_D(const DockingHost);
        return d->role;
    }

    void DockingHost::setRole(Role v) {
        Q_D(DockingHost);
        if (v == d->role || (v != Embedded && v != Floating))
            return;
        d->role = v;
        Q_EMIT roleChanged();
    }

    DockingContext *DockingHost::context() const {
        Q_D(const DockingHost);
        return d->context;
    }

    void DockingHost::setContext(DockingContext *v) {
        Q_D(DockingHost);
        if (d->context == v || !isEmpty())
            return;
        if (d->context)
            d->context->unregisterHost(this);
        if (v)
            v->registerHost(this);
    }

    QQuickWindow *DockingHost::window() const {
        Q_D(const DockingHost);
        return d->window;
    }

    void DockingHost::setWindow(QQuickWindow *v) {
        Q_D(DockingHost);
        if (d->window == v)
            return;
        disconnect(d->windowConnection);
        d->window = v;
        if (v)
            d->windowConnection = connect(v, &QObject::destroyed, this, [this] {
                Q_EMIT windowChanged();
                QMetaObject::invokeMethod(this, [this] {
                if (!window() && role() == Floating && context()) {
                    layout()->clear();
                    context()->unregisterHost(this, true);
                } }, Qt::QueuedConnection);
            });
        Q_EMIT windowChanged();
    }

    DockingLayout *DockingHost::layout() const {
        Q_D(const DockingHost);
        return d->layout;
    }

    bool DockingHost::isEmpty() const {
        return !layout()->root();
    }

    DockingResult DockingHost::close() {
        auto result = layout()->clear();
        if (result.accepted())
            Q_EMIT closeRequested();
        return result;
    }
}

#include "moc_DockingHost.cpp"
