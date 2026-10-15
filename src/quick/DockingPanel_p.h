// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGPANEL_P_H
#define SVSCRAFT_DOCKINGPANEL_P_H

#include <SVSCraftQuick/DockingPanel.h>

#include <QPointer>
#include <QQuickItem>

#include <SVSCraftCore/SVSCraftNamespace.h>

namespace SVS {
    class DockingPanelPrivate {
        Q_DECLARE_PUBLIC(DockingPanel)
    public:
        DockingPanel *q_ptr = nullptr;
        QString key;
        QString title;
        QString description;
        QUrl iconSource;
        QColor iconColor;
        int badgeNumber = 0;
        int badgeType = SVSCraft::CT_Accent;
        QSizeF minimumSize = {0, 0};
        QSizeF preferredSize = {-1, -1};
        QPointer<QQuickItem> contentItem;
        QPointer<QQuickItem> headerItem;
        DockingContext *context = nullptr;
        QList<QMetaObject::Connection> contentConnections;
        QMetaObject::Connection headerConnection;
    };
}

#endif
