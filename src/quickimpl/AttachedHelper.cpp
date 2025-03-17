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

#include "AttachedHelper_p.h"

#include <QEvent>
#include <QQuickItem>

namespace SVS {
    AttachedHelper::AttachedHelper(QObject *parent) : QObject(parent) {
    }
    AttachedHelper::~AttachedHelper() = default;
    bool AttachedHelper::eventFilter(QObject *watched, QEvent *event) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            auto item = qobject_cast<QQuickItem *>(watched);
            if (item) {
                item->ungrabMouse();
            }
            emit doubleClicked();
            return true;
        }
        return QObject::eventFilter(watched, event);
    }
    void AttachedHelper::installDoubleClickEventFilter() {
        parent()->installEventFilter(this);
    }
    void AttachedHelper::removeDoubleClickEventFilter() {
        parent()->removeEventFilter(this);
    }
    void AttachedHelper::setProperty(const QString &key, const QVariant &value) const {
        parent()->setProperty(key.toUtf8(), value);
    }
    AttachedHelper *AttachedHelperAttachedType::qmlAttachedProperties(QObject *parent) {
        return new AttachedHelper(parent);
    }
}