// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKING_H
#define SVSCRAFT_DOCKING_H

#include <QObject>
#include <QScopedPointer>

#include <SVSCraftQuick/DockingTypes.h>

class QQuickWindow;

Q_MOC_INCLUDE(<SVSCraftQuick / DockingPanel.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingGroup.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingHost.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingContext.h>)
Q_MOC_INCLUDE(<QQuickWindow>)

namespace SVS {
    class DockingPanel;
    class DockingGroup;
    class DockingHost;
    class DockingContext;
    class DockingPrivate;

    class SVSCRAFT_QUICK_EXPORT Docking : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(Docking)
        QML_ELEMENT
        QML_UNCREATABLE("Docking is an attached property")
        QML_ATTACHED(Docking)
        Q_PROPERTY(DockingPanel *panel READ panel WRITE setPanel NOTIFY changed)
        Q_PROPERTY(DockingContext *context READ context NOTIFY changed)
        Q_PROPERTY(DockingGroup *group READ group NOTIFY changed)
        Q_PROPERTY(int side READ side NOTIFY changed)
        Q_PROPERTY(DockingHost *host READ host NOTIFY changed)
        Q_PROPERTY(QQuickWindow *window READ window NOTIFY windowChanged)
        Q_PROPERTY(bool visible READ visible NOTIFY visibleChanged)
        Q_PROPERTY(bool effectiveVisible READ effectiveVisible NOTIFY effectiveVisibleChanged)
        Q_PROPERTY(bool active READ active NOTIFY changed)
    public:
        enum PlacementKind {
            EmptyHost = DockingPlacement::EmptyHost,
            Tab = DockingPlacement::Tab,
            Center = DockingPlacement::Center,
            Opposite = DockingPlacement::Opposite,
            Edge = DockingPlacement::Edge
        };
        Q_ENUM(PlacementKind)

        enum Status {
            Succeeded = DockingResult::Succeeded,
            NoChange = DockingResult::NoChange,
            Rejected = DockingResult::Rejected
        };
        Q_ENUM(Status)
        explicit Docking(QObject *parent = nullptr);
        ~Docking() override;
        static Docking *qmlAttachedProperties(QObject *object);
        DockingPanel *panel() const;
        void setPanel(DockingPanel *panel);
        DockingContext *context() const;
        DockingGroup *group() const;
        int side() const;
        DockingHost *host() const;
        QQuickWindow *window() const;
        bool visible() const;
        bool effectiveVisible() const;
        bool active() const;
        Q_INVOKABLE DockingResult show();
        Q_INVOKABLE DockingResult hide();
        Q_INVOKABLE DockingResult remove();
        Q_INVOKABLE DockingResult activate();
        Q_INVOKABLE void floatPanel();
    Q_SIGNALS:
        void changed();
        void windowChanged();
        void visibleChanged();
        void effectiveVisibleChanged();
        void activated();

    private:
        void updateVisibility();
        QScopedPointer<DockingPrivate> d_ptr;
    };
}

#endif

