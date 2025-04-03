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

#ifndef SVSCRAFT_STATUSTEXT_P_P_H
#define SVSCRAFT_STATUSTEXT_P_P_H

#include <SVSCraftQuickImpl/private/StatusText_p.h>

#include <QPointer>
#include <QTimer>

namespace SVS {

    class StatusTextAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(StatusText)
        QML_ATTACHED(StatusText)
    public:
        static StatusText *qmlAttachedProperties(QObject *object);
    };

    class StatusTextPrivate {
        Q_DECLARE_PUBLIC(StatusText)
    public:
        StatusText *q_ptr;

        QList<QPair<QObject *, QString>> statusTextStack;
        QPair<QObject *, QString> contextHelpTextStack;

        QTimer pushTimer;
        QTimer popTimer;
    };
}

#endif // SVSCRAFT_STATUSTEXT_P_P_H
