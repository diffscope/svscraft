// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGSPLIT_P_H
#define SVSCRAFT_DOCKINGSPLIT_P_H

#include <SVSCraftQuick/DockingSplit.h>

namespace SVS {
    class DockingSplitPrivate {
        Q_DECLARE_PUBLIC(DockingSplit)
    public:
        DockingSplit *q_ptr = nullptr;
        Qt::Orientation orientation = Qt::Horizontal;
        QList<DockingNode *> children;
        QVector<qreal> weights;
        QSizeF aggregate(int hint) const;
    };
}

#endif
