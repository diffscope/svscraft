// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGHOST_P_H
#define SVSCRAFT_DOCKINGHOST_P_H

#include <SVSCraftQuick/DockingHost.h>

#include <QPointer>
#include <QQuickWindow>

namespace SVS {
    class DockingView;

    class DockingHostPrivate {
        Q_DECLARE_PUBLIC(DockingHost)
    public:
        DockingHost *q_ptr = nullptr;
        QString id;
        DockingHost::Role role = DockingHost::Embedded;
        DockingContext *context = nullptr;
        QPointer<QQuickWindow> window;
        DockingLayout *layout = nullptr;
        QPointer<DockingView> view;
        QMetaObject::Connection windowConnection;
    };
}

#endif
