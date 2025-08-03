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

#ifndef SVSCRAFT_DOCKING_P_P_H
#define SVSCRAFT_DOCKING_P_P_H

#include <SVSCraftQuick/private/Docking_p.h>

#include <QRect>

namespace SVS {

    class DockingImpl;

    class DockingAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(Docking)
        QML_ATTACHED(Docking)
    public:
        static Docking *qmlAttachedProperties(QObject *object);
    };

    class DockingPrivate {
        Q_DECLARE_PUBLIC(Docking)
    public:
        Docking *q_ptr;
        DockingImpl *dockingImpl;
        QRect windowGeometryHint;
    };
}

#endif // SVSCRAFT_DOCKING_P_P_H
