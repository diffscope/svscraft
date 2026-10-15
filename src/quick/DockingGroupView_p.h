// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGGROUPVIEW_P_H
#define SVSCRAFT_DOCKINGGROUPVIEW_P_H

#include <SVSCraftQuick/DockingGroupView.h>

#include <QPointer>

#include <SVSCraftQuick/DockingGroup.h>

namespace SVS {
    class DockingGroupViewPrivate {
        Q_DECLARE_PUBLIC(DockingGroupView)
    public:
        DockingGroupView *q_ptr = nullptr;
        QPointer<DockingGroup> group;
        bool reveal = false;
        bool mirrored = false;
        mutable bool regionsDirty = true;
        mutable QVariantList regions;
        QMetaObject::Connection connection;
        QMetaObject::Connection destroyedConnection;
        QRectF slot(int side, bool excludeRevealedBar = false) const;
    };
}

#endif
