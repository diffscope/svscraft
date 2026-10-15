// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGLAYOUT_P_H
#define SVSCRAFT_DOCKINGLAYOUT_P_H

#include <SVSCraftQuick/DockingLayout.h>

#include <QHash>

namespace SVS {
    class DockingLayoutPrivate {
        Q_DECLARE_PUBLIC(DockingLayout)
    public:
        DockingLayout *q_ptr = nullptr;
        DockingHost *host = nullptr;
        DockingNode *root = nullptr;
        QHash<QString, DockingNode *> nodes;
        quint64 revision = 0;
        bool committing = false;
    };
}

#endif
