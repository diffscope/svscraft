// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGCONTEXT_H
#define SVSCRAFT_DOCKINGCONTEXT_H

#include <QObject>
#include <QScopedPointer>
#include <qqmlintegration.h>

#include <SVSCraftQuick/DockingTypes.h>

Q_MOC_INCLUDE(<SVSCraftQuick / DockingPanel.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingGroup.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingHost.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingRequest.h>)

namespace SVS {
    class DockingPanel;
    class DockingGroup;
    class DockingHost;
    class DockingRequest;
    class DockingDragSource;
    class DockingContextPrivate;

    class SVSCRAFT_QUICK_EXPORT DockingContext : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DockingContext)
        QML_ELEMENT
        Q_PROPERTY(DockingPanel *activePanel READ activePanel NOTIFY changed)
        Q_PROPERTY(QVariantList hosts READ hostObjects NOTIFY changed)
        Q_PROPERTY(QVariantList panels READ panelObjects NOTIFY changed)
        Q_PROPERTY(int placedPanelCount READ placedPanelCount NOTIFY changed)
        Q_PROPERTY(bool dragging READ isDragging NOTIFY dragChanged)
        Q_PROPERTY(QString draggedPanelKey READ draggedPanelKey NOTIFY dragChanged)
    public:
        explicit DockingContext(QObject *parent = nullptr);
        ~DockingContext() override;
        DockingPanel *activePanel() const;
        QList<DockingHost *> hosts() const;
        QVariantList hostObjects() const;
        QVariantList panelObjects() const;
        int placedPanelCount() const;
        bool isDragging() const;
        QString draggedPanelKey() const;
        Q_INVOKABLE void startDrag(DockingPanel *panel);
        Q_INVOKABLE void cancelDrag();
        Q_INVOKABLE DockingHost *host(const QString &id) const;
        Q_INVOKABLE DockingPanel *panel(const QString &key) const;
        DockingGroup *groupForPanel(const QString &key) const;
        Q_INVOKABLE DockingResult registerPanel(DockingPanel *panel);
        Q_INVOKABLE DockingResult unregisterPanel(DockingPanel *panel);
        Q_INVOKABLE DockingResult registerHost(DockingHost *host);
        Q_INVOKABLE DockingResult unregisterHost(DockingHost *host, bool removeContents = false);
        Q_INVOKABLE DockingResult restoreLayouts(const QVariantMap &states, const QString &activePanelKey = {});
        DockingResult canPlace(DockingPanel *panel, const DockingPlacement &placement) const;
        DockingResult place(DockingPanel *panel, const DockingPlacement &placement);
        Q_INVOKABLE DockingResult canPlaceAt(DockingPanel *panel, const QVariantMap &placement) const;
        Q_INVOKABLE DockingResult placeAt(DockingPanel *panel, const QVariantMap &placement);
        Q_INVOKABLE DockingRequest *requestPlaceAt(DockingPanel *panel, const QVariantMap &placement);
        Q_INVOKABLE DockingResult show(DockingPanel *panel);
        Q_INVOKABLE DockingResult hide(DockingPanel *panel);
        Q_INVOKABLE DockingResult remove(DockingPanel *panel);
        Q_INVOKABLE DockingResult activate(DockingPanel *panel, bool requestFocus = true);
        Q_INVOKABLE void requestFloat(DockingPanel *panel);
    Q_SIGNALS:
        void changed();
        void dragChanged();
        void floatingRequested(SVS::DockingRequest *request);
        void placementRequested(SVS::DockingRequest *request);

    private:
        friend class DockingLayout;
        friend class DockingGroup;
        friend class DockingDragSource;
        void beginDrag(DockingPanel *panel, QObject *source);
        void endDrag(bool accepted);
        bool isUpdating() const;
        DockingResult removePanelFromLayout(DockingPanel *panel);
        void synchronize();
        QScopedPointer<DockingContextPrivate> d_ptr;
    };
}

#endif
