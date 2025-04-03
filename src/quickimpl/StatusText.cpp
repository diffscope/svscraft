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

#include "StatusText_p.h"
#include "StatusText_p_p.h"

#include <QQuickWindow>
#include <QQmlInfo>
#include <QTimer>

namespace SVS {
    StatusText *StatusTextAttachedType::qmlAttachedProperties(QObject *object) {
        auto *window = qobject_cast<QQuickWindow *>(object);
        if (!window)
            qmlWarning(object) << "StatusText should be attached to a Window";
        return new StatusText(object);
    }

    StatusText::StatusText(QObject *parent) : QObject(parent), d_ptr(new StatusTextPrivate) {
        Q_D(StatusText);
        d->q_ptr = this;
        d->pushTimer.setSingleShot(true);
        d->popTimer.setSingleShot(true);
    }
    StatusText::~StatusText() = default;

    void StatusText::pushStatusText(QObject *contextObject, const QString &text) {
        Q_D(StatusText);
        d->statusTextStack.removeIf([=](const auto &p) { return p.first == contextObject; });
        d->statusTextStack.append({contextObject, text});
        emit statusTextChanged();
    }
    void StatusText::popStatusText(QObject *contextObject) {
        Q_D(StatusText);
        bool notify = !d->statusTextStack.isEmpty() && d->statusTextStack.back().first == contextObject;
        d->statusTextStack.removeIf([=](const auto &p) { return p.first == contextObject; });
        if (notify)
            emit statusTextChanged();
    }
    void StatusText::pushContextHelpText(QObject *contextObject, const QString &text, int delay) {
        Q_D(StatusText);
        if (delay == 0) {
            d->contextHelpTextStack = {contextObject, text};
            emit contextHelpTextChanged();
        } else {
            d->pushTimer.setInterval(delay);
            d->pushTimer.setProperty("contextObject", QVariant::fromValue(contextObject));
            d->pushTimer.callOnTimeout([=] {
                d->popTimer.stop();
                d->contextHelpTextStack = {contextObject, text};
                emit contextHelpTextChanged();
            });
            d->pushTimer.start();
        }
    }
    void StatusText::popContextHelpText(QObject *contextObject, int delay) {
        Q_D(StatusText);
        if (d->pushTimer.property("contextObject").value<QObject *>() == contextObject) {
            d->pushTimer.stop();
        }
        if (d->contextHelpTextStack.first != contextObject)
            return;
        if (delay == 0) {
            d->contextHelpTextStack = {};
            emit contextHelpTextChanged();
        } else {
            d->popTimer.setInterval(delay);
            d->popTimer.start();
        }
    }
    QString StatusText::statusText() const {
        Q_D(const StatusText);
        return d->statusTextStack.isEmpty() ? QString() : d->statusTextStack.back().second;
    }
    QString StatusText::contextHelpText() const {
        Q_D(const StatusText);
        return d->contextHelpTextStack.second;
    }

}