// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGLAYOUT_H
#define SVSCRAFT_DOCKINGLAYOUT_H

#include <QObject>
#include <QScopedPointer>
#include <qqmlintegration.h>

#include <SVSCraftQuick/DockingTypes.h>

Q_MOC_INCLUDE(<SVSCraftQuick / DockingHost.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingNode.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingGroup.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingPanel.h>)

namespace SVS {
    class DockingGroup;
    class DockingHost;
    class DockingNode;
    class DockingPanel;
    class DockingSplit;
    class DockingLayoutPrivate;

    class SVSCRAFT_QUICK_EXPORT DockingLayout : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DockingLayout)
        QML_ELEMENT
        QML_UNCREATABLE("Layouts belong to a DockingHost")
        Q_PROPERTY(DockingNode *root READ root NOTIFY changed)
        Q_PROPERTY(quint64 revision READ revision NOTIFY changed)
        Q_PROPERTY(DockingHost *host READ host CONSTANT)
        Q_PROPERTY(QVariantList groups READ groupObjects NOTIFY changed)
    public:
        ~DockingLayout() override;
        DockingHost *host() const;
        DockingNode *root() const;
        quint64 revision() const;
        Q_INVOKABLE DockingNode *node(const QString &id) const;
        QList<DockingGroup *> groups() const;
        QVariantList groupObjects() const;
        DockingGroup *groupForPanel(const QString &key) const;
        DockingLayoutState saveState() const;
        DockingResult restoreState(const DockingLayoutState &state);
        static DockingResult validateState(const DockingLayoutState &state);
        Q_INVOKABLE QVariantMap save() const;
        Q_INVOKABLE DockingResult restore(const QVariantMap &state);
        Q_INVOKABLE DockingResult clear();
        Q_INVOKABLE DockingGroup *createGroup(DockingPanel *panel, DockingGroup *anchor = nullptr, Qt::Edge edge = Qt::LeftEdge);
        Q_INVOKABLE DockingResult removeGroup(DockingGroup *group);
    Q_SIGNALS:
        void changed();

    private:
        friend class DockingContext;
        friend class DockingHost;
        friend class DockingGroup;
        friend class DockingSplit;
        explicit DockingLayout(DockingHost *host);
        void commit();
        void normalize();
        DockingGroup *makeGroup();
        DockingResult insertGroup(DockingGroup *group, DockingGroup *anchor, Qt::Edge edge);
        DockingNode *build(const DockingNodeState &state, DockingSplit *parent);
        QScopedPointer<DockingLayoutPrivate> d_ptr;
    };
}

#endif
