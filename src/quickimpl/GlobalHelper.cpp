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

#include "GlobalHelper_p.h"

#include <QCursor>
#include <QQuickItem>

namespace SVS {
    GlobalHelper::GlobalHelper(QObject *parent) : QObject(parent) {
    }
    GlobalHelper::~GlobalHelper() = default;
    QPoint GlobalHelper::cursorPos() {
        return QCursor::pos();
    }
    void GlobalHelper::setProperty(QObject *object, const QString &key, const QVariant &value) {
        if (!object)
            return;
        object->setProperty(key.toUtf8(), value);
    }
    void GlobalHelper::ungrabMouse(QQuickItem *item) {
        if (!item)
            return;
        item->ungrabMouse();
    }
}

#include "moc_GlobalHelper_p.cpp"