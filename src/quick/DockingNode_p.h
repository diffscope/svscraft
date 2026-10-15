// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGNODE_P_H
#define SVSCRAFT_DOCKINGNODE_P_H

#include <SVSCraftQuick/DockingNode.h>

namespace SVS {
    class DockingNodePrivate {
        Q_DECLARE_PUBLIC(DockingNode)
    public:
        DockingNode *q_ptr = nullptr;
        QString id;
        DockingNode::Kind kind;
        DockingLayout *layout = nullptr;
        DockingSplit *parent = nullptr;
    };
}

#endif
