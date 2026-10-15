// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGGEOMETRY_P_H
#define SVSCRAFT_DOCKINGGEOMETRY_P_H

#include <QVector>

#include <SVSCraftQuickImpl/SVSCraftQuickImplGlobal.h>

namespace SVS {
    struct DockingExtent {
        qreal minimum = 0;
        qreal target = 0;
        qreal weight = 1;
        bool locked = false;
        bool fixed = false;
    };

    SVSCRAFT_QUICK_IMPL_EXPORT QVector<qreal>
    dockingAllocate(qreal available, const QVector<DockingExtent> &extents);
}

#endif
