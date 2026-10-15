// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGSPLIT_H
#define SVSCRAFT_DOCKINGSPLIT_H

#include <SVSCraftQuick/DockingNode.h>
#include <SVSCraftQuick/DockingTypes.h>

namespace SVS {
    class DockingSplitPrivate;

    class SVSCRAFT_QUICK_EXPORT DockingSplit : public DockingNode {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DockingSplit)
        QML_ELEMENT
        QML_UNCREATABLE("Splits belong to a DockingLayout")
        Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation NOTIFY changed)
        Q_PROPERTY(QVariantList childNodes READ childNodes NOTIFY changed)
        Q_PROPERTY(QVariantList weights READ weights WRITE setWeights NOTIFY changed)
    public:
        ~DockingSplit() override;
        Qt::Orientation orientation() const;
        void setOrientation(Qt::Orientation orientation);
        QList<DockingNode *> children() const;
        QVariantList childNodes() const;
        QVariantList weights() const;
        void setWeights(const QVariantList &weights);
        QSizeF minimumSize() const override;
        QSizeF preferredSize() const override;
        QSizeF lockedSize() const override;
        bool widthLocked() const override;
        bool heightLocked() const override;

    private:
        friend class DockingLayout;
        explicit DockingSplit(const QString &id, DockingLayout *layout);
        QScopedPointer<DockingSplitPrivate> d_ptr;
    };
}

#endif
