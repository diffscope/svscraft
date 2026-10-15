// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGCONTEXT_P_H
#define SVSCRAFT_DOCKINGCONTEXT_P_H

#include <SVSCraftQuick/DockingContext.h>

#include <QHash>
#include <QPointer>

namespace SVS {
    class DockingContextPrivate {
        Q_DECLARE_PUBLIC(DockingContext)
    public:
        DockingContext *q_ptr = nullptr;
        QList<DockingHost *> hosts;
        QHash<QString, DockingPanel *> panels;
        QPointer<DockingPanel> activePanel;
        bool updating = false;
        bool destroying = false;
        bool synchronizing = false;
        bool synchronizePending = false;
        QString draggedPanelKey;
        QList<QMetaObject::Connection> dragConnections;
        QMetaObject::Connection focusConnection;
    };
}

#endif
