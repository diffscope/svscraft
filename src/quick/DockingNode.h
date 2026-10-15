// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGNODE_H
#define SVSCRAFT_DOCKINGNODE_H

#include <QObject>
#include <QScopedPointer>
#include <QSizeF>
#include <qqmlintegration.h>

#include <SVSCraftQuick/SVSCraftQuickGlobal.h>

Q_MOC_INCLUDE(<SVSCraftQuick / DockingLayout.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingSplit.h>)

namespace SVS {
    class DockingLayout;
    class DockingNodePrivate;
    class DockingSplit;

    class SVSCRAFT_QUICK_EXPORT DockingNode : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DockingNode)
        QML_ELEMENT
        QML_UNCREATABLE("Nodes belong to a DockingLayout")
        Q_PROPERTY(QString nodeId READ nodeId CONSTANT)
        Q_PROPERTY(Kind kind READ kind CONSTANT)
        Q_PROPERTY(DockingSplit *parentNode READ parentNode NOTIFY changed)
        Q_PROPERTY(DockingLayout *layout READ layout CONSTANT)
        Q_PROPERTY(QSizeF minimumSize READ minimumSize NOTIFY changed)
        Q_PROPERTY(QSizeF preferredSize READ preferredSize NOTIFY changed)
        Q_PROPERTY(bool widthLocked READ widthLocked NOTIFY changed)
        Q_PROPERTY(bool heightLocked READ heightLocked NOTIFY changed)
        Q_PROPERTY(QSizeF lockedSize READ lockedSize NOTIFY changed)
    public:
        enum Kind {
            Group,
            Split
        };
        Q_ENUM(Kind)
        ~DockingNode() override;
        QString nodeId() const;
        Kind kind() const;
        DockingSplit *parentNode() const;
        DockingLayout *layout() const;
        virtual QSizeF minimumSize() const = 0;
        virtual QSizeF preferredSize() const = 0;
        virtual bool widthLocked() const = 0;
        virtual bool heightLocked() const = 0;
        virtual QSizeF lockedSize() const = 0;
    Q_SIGNALS:
        void changed();

    protected:
        explicit DockingNode(Kind kind, const QString &id, DockingLayout *layout);

    private:
        friend class DockingLayout;
        friend class DockingSplit;
        QScopedPointer<DockingNodePrivate> d_ptr;
    };
}

#endif
