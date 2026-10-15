// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingRequest.h"
#include "DockingRequest_p.h"

#include <SVSCraftQuick/DockingGroup.h>
#include <SVSCraftQuick/DockingHost.h>
#include <SVSCraftQuick/DockingLayout.h>

namespace SVS {
    DockingRequest::DockingRequest(DockingContext *context, DockingPanel *panel, const DockingPlacement &at, bool floating) : QObject(context), d_ptr(new DockingRequestPrivate) {
        d_ptr->q_ptr = this;
        d_ptr->context = context;
        d_ptr->panel = panel;
        d_ptr->placement = at;
        d_ptr->floating = floating;
        if (panel && panel->group()) {
            d_ptr->sourceHost = panel->group()->layout()->host();
            d_ptr->sourceRevision = d_ptr->sourceHost->layout()->revision();
        }
        if (!floating) {
            d_ptr->targetHost = context->host(at.hostId);
            if (d_ptr->targetHost)
                d_ptr->targetRevision = d_ptr->targetHost->layout()->revision();
        }
    }

    DockingRequest::~DockingRequest() = default;

    DockingPanel *DockingRequest::panel() const {
        Q_D(const DockingRequest);
        return d->panel;
    }

    bool DockingRequest::isFinished() const {
        Q_D(const DockingRequest);
        return d->finished;
    }

    DockingResult DockingRequest::result() const {
        Q_D(const DockingRequest);
        return d->result;
    }

    DockingResult DockingRequest::accept(DockingHost *host) {
        Q_D(DockingRequest);
        if (d->finished)
            return {DockingResult::Rejected, "requestFinished"};
        if (!d->context || !d->panel || d->sourceRevision && !d->sourceHost) {
            reject("requestExpired");
            return d->result;
        }
        if (!d->sourceRevision && d->panel->group()) {
            reject("layoutChanged");
            return d->result;
        }
        if (d->sourceHost && d->sourceHost->layout()->revision() != d->sourceRevision || !d->floating && (!d->targetHost || d->targetHost->layout()->revision() != d->targetRevision)) {
            reject("layoutChanged");
            return d->result;
        }
        auto placement = d->placement;
        if (d->floating) {
            if (!host || host->context() != d->context || host->role() != DockingHost::Floating || !host->isEmpty()) {
                reject("invalidFloatingHost");
                return d->result;
            }
            placement.hostId = host->hostId();
            placement.kind = DockingPlacement::EmptyHost;
        }
        d->result = d->context->place(d->panel, placement);
        d->finished = true;
        Q_EMIT finishedChanged();
        deleteLater();
        return d->result;
    }

    void DockingRequest::reject(const QString &reason) {
        Q_D(DockingRequest);
        if (d->finished)
            return;
        d->result = {DockingResult::Rejected, reason};
        d->finished = true;
        Q_EMIT finishedChanged();
        deleteLater();
    }
}

#include "moc_DockingRequest.cpp"

