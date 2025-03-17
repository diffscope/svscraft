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

#ifndef SVSCRAFT_SVSQMLNAMESPACE_P_H
#define SVSCRAFT_SVSQMLNAMESPACE_P_H

#include <QObject>
#include <qqmlintegration.h>

namespace SVS {

    class SVSQmlNamespace : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(SVS)
        QML_SINGLETON
    public:
        enum ControlType {
            CT_Normal, CT_Accent, CT_Warning, CT_Error,
        };
        Q_ENUM(ControlType)

        enum BackgroundLevel {
            BL_Primary, BL_Secondary, BL_Tertiary, BL_Quaternary,
        };
        Q_ENUM(BackgroundLevel)

        enum ForegroundLevel {
            FL_Primary, FL_Secondary,
        };
        Q_ENUM(ForegroundLevel)

        enum TabIndicator {
            TI_Fill, TI_Top, TI_Bottom, TI_Left, TI_Right,
        };
        Q_ENUM(TabIndicator)

        enum DividerStroke {
            DS_Auto, DS_None, DS_Border, DS_Splitter,
        };
        Q_ENUM(DividerStroke)
    };

}

#endif // SVSCRAFT_SVSQMLNAMESPACE_P_H
