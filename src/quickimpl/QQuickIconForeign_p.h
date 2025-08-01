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

#ifndef SVSCRAFT_QQUICKICONFOREIGN_P_H
#define SVSCRAFR_QQUICKICONFOREIGN_P_H

#include <QtQuickTemplates2/private/qquickicon_p.h>
#include <QMetaType>
#include <qqmlintegration.h>

namespace SVS {

    struct QQuickIconForeign {
        Q_GADGET
        QML_NAMED_ELEMENT(qQuickIcon)
        QML_FOREIGN(QQuickIcon)
        QML_CONSTRUCTIBLE_VALUE
    };

}

#endif //SVSCRAFT_QQUICKICONFOREIGN_P_H
