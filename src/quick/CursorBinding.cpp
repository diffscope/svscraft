/******************************************************************************
 * Copyright (c) 2026 OpenVPI                                                 *
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

#include "CursorBinding_p.h"
#include "CursorBinding_p_p.h"

#include <QGuiApplication>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQmlInfo>

namespace SVS {

    CursorBinding *CursorBindingAttachedType::qmlAttachedProperties(QObject *object) {
        auto *item = qobject_cast<QQuickItem *>(object);
        if (!item) {
            qmlWarning(object) << "CursorBinding should be attached to an Item";
        }
        auto *binding = new CursorBinding(item);
        if (!item) {
            binding->setParent(object);
        }
        return binding;
    }

    CursorBinding::CursorBinding(QQuickItem *parent) : QObject(parent), d_ptr(new CursorBindingPrivate) {
        Q_D(CursorBinding);
        d->q_ptr = this;
        d->item = parent;

    }

    CursorBinding::~CursorBinding() {
        Q_D(CursorBinding);
        if (!d->cursorApplied) {
            return;
        }
        QGuiApplication::restoreOverrideCursor();
    }

    bool CursorBinding::enabled() const {
        Q_D(const CursorBinding);
        return d->enabled;
    }

    void CursorBinding::setEnabled(bool enabled) {
        Q_D(CursorBinding);
        if (d->enabled == enabled) {
            return;
        }
        d->enabled = enabled;
        emit enabledChanged();

        if (enabled) {
            if (d->cursorApplied) {
                QGuiApplication::restoreOverrideCursor();
            }
            QGuiApplication::setOverrideCursor(QCursor(d->cursorShape));
            d->cursorApplied = true;
        } else {
            if (d->cursorApplied) {
                QGuiApplication::restoreOverrideCursor();
                d->cursorApplied = false;
            }
        }
    }

    Qt::CursorShape CursorBinding::cursorShape() const {
        Q_D(const CursorBinding);
        return d->cursorShape;
    }

    void CursorBinding::setCursorShape(Qt::CursorShape cursorShape) {
        Q_D(CursorBinding);
        if (d->cursorShape == cursorShape) {
            return;
        }
        d->cursorShape = cursorShape;
        emit cursorShapeChanged();

        if (!d->enabled) {
            return;
        }

        if (d->cursorApplied) {
            QGuiApplication::restoreOverrideCursor();
        }
        QGuiApplication::setOverrideCursor(QCursor(d->cursorShape));
        d->cursorApplied = true;
    }

}
