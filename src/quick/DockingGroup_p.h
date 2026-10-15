// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGGROUP_P_H
#define SVSCRAFT_DOCKINGGROUP_P_H

#include <SVSCraftQuick/DockingGroup.h>

#include <SVSCraftQuick/DockingTypes.h>

namespace SVS {
    class DockingGroupPrivate {
        Q_DECLARE_PUBLIC(DockingGroup)
    public:
        DockingGroup *q_ptr = nullptr;
        DockingNodeState state;
        QSizeF layoutSize;
        bool explicitLockedSize = false;
        bool hasExpandedSize = false;

        static constexpr qreal panelHeaderHeight = 32;

        qreal tabBarPadding() const {
            return state.compactLayout ? 0 : 4;
        }

        qreal tabButtonSize() const {
            return 24;
        }

        qreal tabBarThickness() const {
            return tabButtonSize() + 2 * tabBarPadding();
        }

        static DockingGroupPrivate *get(DockingGroup *group) { return group->d_func(); }

        QSizeF sizeHint(bool preferred, bool expanded = false) const;
    };
}

#endif
