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

#ifndef SVSCRAFT_CURSORBINDING_P_P_H
#define SVSCRAFT_CURSORBINDING_P_P_H

#include <SVSCraftQuick/private/CursorBinding_p.h>

namespace SVS {

    class CursorBindingAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(CursorBinding)
        QML_ATTACHED(CursorBinding)
    public:
        static CursorBinding *qmlAttachedProperties(QObject *object);
    };

    class CursorBindingPrivate {
        Q_DECLARE_PUBLIC(CursorBinding)
    public:
        CursorBinding *q_ptr;
        QQuickItem *item = nullptr;
        Qt::CursorShape cursorShape = Qt::ArrowCursor;
        bool enabled = false;
        bool cursorApplied = false;
    };

}

#endif // SVSCRAFT_CURSORBINDING_P_P_H
