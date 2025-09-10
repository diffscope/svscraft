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

#include "StatusTextContext.h"
#include "StatusTextContext_p.h"

#include <qqml.h>
#include <qqmlinfo.h>
#include <QQuickWindow>

namespace SVS {

    StatusTextContextWindowContext * StatusTextContextAttachedType::qmlAttachedProperties(QObject *object) {
        auto *window = qobject_cast<QQuickWindow *>(object);
        if (!window)
            qmlWarning(object) << "StatusTextContext should be attached to a Window";
        return new StatusTextContextWindowContext(object);
    }

    void StatusTextContextPrivate::updateTextProperty() {
        Q_Q(StatusTextContext);
        emit q->textChanged();
        emit q->contextObjectChanged();
    }

    StatusTextContext::StatusTextContext(QObject *parent) 
        : QObject(parent), d_ptr(new StatusTextContextPrivate) {
        Q_D(StatusTextContext);
        d->q_ptr = this;
    }

    StatusTextContext::~StatusTextContext() = default;

    QString StatusTextContext::text() const {
        Q_D(const StatusTextContext);
        if (d->contextList.isEmpty()) {
            return QString();
        }
        return d->contextList.last().second;
    }

    QObject *StatusTextContext::contextObject() const {
        Q_D(const StatusTextContext);
        if (d->contextList.isEmpty()) {
            return nullptr;
        }
        return d->contextList.last().first;
    }

    void StatusTextContext::push(QObject *contextObject, const QString &text) {
        Q_D(StatusTextContext);
        
        // Remove all existing entries with the same contextObject
        d->contextList.removeIf([contextObject](const QPair<QObject *, QString> &item) {
            return item.first == contextObject;
        });
        
        // Add new entry to the end
        d->contextList.append(qMakePair(contextObject, text));
        
        // Connect to destroyed signal for automatic cleanup
        connect(contextObject, &QObject::destroyed, this, [this, contextObject]() {
            pop(contextObject);
        });
        
        d->updateTextProperty();
    }

    void StatusTextContext::update(QObject *contextObject, const QString &text) {
        Q_D(StatusTextContext);
        
        // Find existing entry with the same contextObject
        bool found = false;
        for (auto &item : d->contextList) {
            if (item.first == contextObject) {
                item.second = text;
                found = true;
                break;
            }
        }
        
        // If not found, add new entry to the end
        if (!found) {
            d->contextList.append(qMakePair(contextObject, text));
            
            // Connect to destroyed signal for automatic cleanup
            connect(contextObject, &QObject::destroyed, this, [this, contextObject]() {
                pop(contextObject);
            });
        }
        
        d->updateTextProperty();
    }

    void StatusTextContext::pop(QObject *contextObject) {
        Q_D(StatusTextContext);
        
        const int sizeBefore = d->contextList.size();
        d->contextList.removeIf([contextObject](const QPair<QObject *, QString> &item) {
            return item.first == contextObject;
        });
        
        if (sizeBefore != d->contextList.size()) {
            d->updateTextProperty();
        }
    }

    void StatusTextContext::setStatusContext(QQuickWindow *window, StatusTextContext *context) {
        auto windowContext = qobject_cast<StatusTextContextWindowContext *>(qmlAttachedPropertiesObject<StatusTextContextAttachedType>(window));
        Q_ASSERT(windowContext);
        windowContext->setStatusContext(context);
    }

    StatusTextContext * StatusTextContext::statusContext(QQuickWindow *window) {
        auto windowContext = qobject_cast<StatusTextContextWindowContext *>(qmlAttachedPropertiesObject<StatusTextContextAttachedType>(window));
        Q_ASSERT(windowContext);
        return windowContext->statusContext();
    }

    void StatusTextContext::setContextHelpContext(QQuickWindow *window, StatusTextContext *context) {
        auto windowContext = qobject_cast<StatusTextContextWindowContext *>(qmlAttachedPropertiesObject<StatusTextContextAttachedType>(window));
        Q_ASSERT(windowContext);
        windowContext->setContextHelpContext(context);
    }

    StatusTextContext * StatusTextContext::contextHelpContext(QQuickWindow *window) {
        auto windowContext = qobject_cast<StatusTextContextWindowContext *>(qmlAttachedPropertiesObject<StatusTextContextAttachedType>(window));
        Q_ASSERT(windowContext);
        return windowContext->contextHelpContext();
    }

}
