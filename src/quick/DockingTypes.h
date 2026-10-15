// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGTYPES_H
#define SVSCRAFT_DOCKINGTYPES_H

#include <QRectF>
#include <QSizeF>
#include <QStringList>
#include <QVariantMap>
#include <QVector>
#include <qqmlintegration.h>

#include <SVSCraftQuick/SVSCraftQuickGlobal.h>

namespace SVS {

    class SVSCRAFT_QUICK_EXPORT DockingResult {
        Q_GADGET
        QML_VALUE_TYPE(dockingResult)
        Q_PROPERTY(Status status MEMBER status)
        Q_PROPERTY(QString reason MEMBER reason)
        Q_PROPERTY(bool accepted READ accepted)
    public:
        enum Status {
            Succeeded,
            NoChange,
            Rejected
        };
        Q_ENUM(Status)
        Status status = Succeeded;
        QString reason;

        bool accepted() const { return status != Rejected; }
    };

    class SVSCRAFT_QUICK_EXPORT DockingPlacement {
        Q_GADGET
        QML_VALUE_TYPE(dockingPlacement)
        Q_PROPERTY(QString hostId MEMBER hostId)
        Q_PROPERTY(QString groupId MEMBER groupId)
        Q_PROPERTY(Kind kind MEMBER kind)
        Q_PROPERTY(int side MEMBER side)
        Q_PROPERTY(QString anchor MEMBER anchor)
        Q_PROPERTY(bool after MEMBER after)
        Q_PROPERTY(Qt::Edge edge MEMBER edge)
    public:
        enum Kind {
            EmptyHost,
            Tab,
            Center,
            Opposite,
            Edge
        };
        Q_ENUM(Kind)
        QString hostId;
        QString groupId;
        Kind kind = EmptyHost;
        int side = 0;
        QString anchor;
        bool after = true;
        Qt::Edge edge = Qt::LeftEdge;
        QVariantMap toVariant() const;
        static DockingPlacement fromVariant(const QVariantMap &value);
    };

    struct SVSCRAFT_QUICK_EXPORT DockingNodeState {
        QString kind = QStringLiteral("group");
        QString nodeId;
        Qt::Orientation orientation = Qt::Horizontal;
        QVector<DockingNodeState> children;
        QVector<qreal> weights;
        Qt::Edge tabBarEdge = Qt::TopEdge;
        bool autoHide = false;
        bool widthLocked = false;
        bool heightLocked = false;
        QSizeF expandedSize = {400, 400};
        QSizeF lockedSize = {400, 400};
        qreal contentRatio = 0.5;
        QStringList panelIds[2];
        QString selected[2];
        QString lastSelected[2];
        QVariantMap toVariant() const;
        static DockingNodeState fromVariant(const QVariantMap &value);
    };

    struct SVSCRAFT_QUICK_EXPORT DockingLayoutState {
        bool hasRoot = false;
        DockingNodeState root;
        QVariantMap toVariant() const;
        static DockingLayoutState fromVariant(const QVariantMap &value);
    };

}

Q_DECLARE_METATYPE(SVS::DockingResult)
Q_DECLARE_METATYPE(SVS::DockingPlacement)
Q_DECLARE_METATYPE(SVS::DockingLayoutState)

#endif
