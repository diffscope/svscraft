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

#ifndef SVSCRAFT_GLOBALHELPER_P_H
#define SVSCRAFT_GLOBALHELPER_P_H

#include <QObject>
#include <qqmlintegration.h>

class QQuickItem;

namespace SVS {

    class GlobalHelper : public QObject {
        Q_OBJECT
        QML_ELEMENT
        QML_SINGLETON
    public:
        explicit GlobalHelper(QObject *parent = nullptr);
        ~GlobalHelper() override;

        Q_INVOKABLE static QPoint cursorPos();
        Q_INVOKABLE static void setProperty(QObject *object, const QString &key, const QVariant &value);
        Q_INVOKABLE static void ungrabMouse(QQuickItem *item);
    };
}

#endif // SVSCRAFT_GLOBALHELPER_P_H
