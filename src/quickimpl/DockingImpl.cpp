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

#include "DockingImpl_p.h"

#include <QQmlEngine>
#include <QQuickWindow>

namespace SVS {

    DockingImpl *DockingImplAttachedType::qmlAttachedProperties(QObject *object) {
        return new DockingImpl(object);
    }


    DockingImpl::DockingImpl(QObject *parent)
        : QObject(parent), m_dockingView(nullptr), m_window(nullptr) {
    }
    DockingImpl::~DockingImpl() = default;
    QObject *DockingImpl::dockingView() const {
        return m_dockingView;
    }
    void DockingImpl::setDockingView(QObject *dockingView) {
        if (m_dockingView != dockingView) {
            m_dockingView = dockingView;
            emit dockingViewChanged();
        }
    }
    QQuickWindow *DockingImpl::window() const {
        return m_window;
    }
    void DockingImpl::setWindow(QQuickWindow *window) {
        if (m_window != window) {
            m_window = window;
            emit windowChanged();
        }
    }
}

#include "moc_DockingImpl_p.cpp"
