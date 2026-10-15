// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKING_P_H
#define SVSCRAFT_DOCKING_P_H

#include <SVSCraftQuick/Docking.h>

#include <QPointer>

#include <SVSCraftQuick/DockingPanel.h>

namespace SVS {
    class DockingPrivate {
        Q_DECLARE_PUBLIC(Docking)
    public:
        Docking *q_ptr = nullptr;
        QPointer<DockingPanel> panel;
        QList<QMetaObject::Connection> connections;
        QMetaObject::Connection windowConnection;
        bool lastVisible = false;
        bool lastEffectiveVisible = false;
    };
}

#endif
