// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingNode.h"
#include "DockingNode_p.h"

#include <SVSCraftQuick/DockingLayout.h>

namespace SVS {
    DockingNode::DockingNode(Kind kind, const QString &id, DockingLayout *layout) : QObject(layout), d_ptr(new DockingNodePrivate) {
        Q_D(DockingNode);
        d->q_ptr = this;
        d->kind = kind;
        d->id = id;
        d->layout = layout;
    }

    DockingNode::~DockingNode() = default;

    QString DockingNode::nodeId() const {
        Q_D(const DockingNode);
        return d->id;
    }

    DockingNode::Kind DockingNode::kind() const {
        Q_D(const DockingNode);
        return d->kind;
    }

    DockingSplit *DockingNode::parentNode() const {
        Q_D(const DockingNode);
        return d->parent;
    }

    DockingLayout *DockingNode::layout() const {
        Q_D(const DockingNode);
        return d->layout;
    }
}

#include "moc_DockingNode.cpp"
