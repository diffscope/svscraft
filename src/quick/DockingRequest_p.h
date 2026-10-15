// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGREQUEST_P_H
#define SVSCRAFT_DOCKINGREQUEST_P_H

#include <SVSCraftQuick/DockingRequest.h>

#include <QPointer>

#include <SVSCraftQuick/DockingContext.h>
#include <SVSCraftQuick/DockingHost.h>
#include <SVSCraftQuick/DockingPanel.h>

namespace SVS {
    class DockingRequestPrivate {
        Q_DECLARE_PUBLIC(DockingRequest)
    public:
        DockingRequest *q_ptr = nullptr;
        QPointer<DockingContext> context;
        QPointer<DockingPanel> panel;
        DockingPlacement placement;
        bool floating = false;
        bool finished = false;
        QPointer<DockingHost> sourceHost;
        QPointer<DockingHost> targetHost;
        quint64 sourceRevision = 0;
        quint64 targetRevision = 0;
        DockingResult result{DockingResult::NoChange, "pending"};
    };
}

#endif

