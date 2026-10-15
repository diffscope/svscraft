// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGREQUEST_H
#define SVSCRAFT_DOCKINGREQUEST_H

#include <QObject>
#include <QScopedPointer>

#include <SVSCraftQuick/DockingTypes.h>

Q_MOC_INCLUDE(<SVSCraftQuick / DockingPanel.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingHost.h>)

namespace SVS {
    class DockingContext;
    class DockingPanel;
    class DockingHost;
    class DockingRequestPrivate;

    class SVSCRAFT_QUICK_EXPORT DockingRequest : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DockingRequest)
        QML_ELEMENT
        QML_UNCREATABLE("Requests belong to a DockingContext")
        Q_PROPERTY(DockingPanel *panel READ panel CONSTANT)
        Q_PROPERTY(bool finished READ isFinished NOTIFY finishedChanged)
        Q_PROPERTY(DockingResult result READ result NOTIFY finishedChanged)
    public:
        ~DockingRequest() override;
        DockingPanel *panel() const;
        bool isFinished() const;
        DockingResult result() const;
        Q_INVOKABLE DockingResult accept(DockingHost *floatingHost = nullptr);
        Q_INVOKABLE void reject(const QString &reason);
    Q_SIGNALS:
        void finishedChanged();

    private:
        friend class DockingContext;
        DockingRequest(DockingContext *context, DockingPanel *panel, const DockingPlacement &placement, bool floating);
        QScopedPointer<DockingRequestPrivate> d_ptr;
    };
}

#endif

