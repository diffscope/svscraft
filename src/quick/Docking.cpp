/******************************************************************************
 * Copyright (c) 2025 OpenVPI                                                 *
 *                                                                            *
 * This file is part of SVSCraft                                              *
 *                                                                            *
 * SVSCraft is free software: you can redistribute it and/or modify it under  *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License, or (at your     *
 * option) any later version.                                                 *
 *                                                                            *
 * SVSCraft is distributed in the hope that it will be useful, but WITHOUT    *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public        *
 * License for more details.                                                  *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with SVSCraft. If not, see <https://www.gnu.org/licenses/>.          *
 ******************************************************************************/

#include "Docking_p.h"
#include "Docking_p_p.h"

#include <QQmlEngine>
#include <QQuickWindow>

#include <SVSCraftQuickImpl/private/DockingImpl_p.h>

namespace SVS {

    Docking *DockingAttachedType::qmlAttachedProperties(QObject *object) {
        auto o = new Docking(object);
        return o;
    }


    Docking::Docking(QObject *parent) : QObject(parent), d_ptr(new DockingPrivate) {
        Q_D(Docking);
        d->q_ptr = this;
        d->dockingImpl = qobject_cast<DockingImpl *>(qmlAttachedPropertiesObject<DockingImplAttachedType>(parent));
        connect(d->dockingImpl, &DockingImpl::dockingViewChanged, this, &Docking::dockingViewChanged);
        connect(d->dockingImpl, &DockingImpl::windowChanged, this, &Docking::windowChanged);
    }
    Docking::~Docking() = default;
    QObject *Docking::dockingView() const {
        Q_D(const Docking);
        return d->dockingImpl->dockingView();
    }
    QQuickWindow *Docking::window() const {
        Q_D(const Docking);
        return d->dockingImpl->window();
    }
}

#include "moc_Docking_p.cpp"