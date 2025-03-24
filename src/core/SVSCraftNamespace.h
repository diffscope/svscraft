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

#ifndef SVSCRAFT_SVSCRAFTNAMESPACE_H
#define SVSCRAFT_SVSCRAFTNAMESPACE_H

#include <SVSCraftCore/SVSCraftCoreGlobal.h>

#include <QMetaObject>

namespace SVS {

    namespace SVSCraft {
        Q_NAMESPACE_EXPORT(SVSCRAFT_CORE_EXPORT)
        enum ControlType {
            CT_Normal, CT_Accent, CT_Warning, CT_Error,
        };
        Q_ENUM_NS(ControlType)

        enum BackgroundLevel {
            BL_Primary, BL_Secondary, BL_Tertiary, BL_Quaternary,
        };
        Q_ENUM_NS(BackgroundLevel)

        enum ForegroundLevel {
            FL_Primary, FL_Secondary,
        };
        Q_ENUM_NS(ForegroundLevel)

        enum TabIndicator {
            TI_Fill, TI_Top, TI_Bottom, TI_Left, TI_Right,
        };
        Q_ENUM_NS(TabIndicator)

        enum DividerStroke {
            DS_Auto, DS_None, DS_Border, DS_Splitter,
        };
        Q_ENUM_NS(DividerStroke)

        enum ColorPickerModeFlag {
            CM_ColorSpecRgb = 0x1,
            CM_ColorSpecHsv = 0x2,
            CM_ColorSpecHsl = 0x4,
            CM_ColorSpecCmyk = 0x8,

            CM_Hex = 0x100,
            CM_Alpha = 0x200,
            CM_AxisChangeable = 0x400,
            CM_Eyedropper = 0x800,
            CM_NativeColorDialog = 0x1000,
            CM_ShowCurrentColor = 0x2000,
        };
        Q_ENUM_NS(ColorPickerModeFlag)
        Q_DECLARE_FLAGS(ColorPickerMode, ColorPickerModeFlag)


        enum ColorAxis {
            CA_Hue,
            CA_Saturation,
            CA_Value,

            CA_SaturationValue = CA_Hue,
            CA_HueValue = CA_Saturation,
            CA_HueSaturation = CA_Value,

            CA_Alpha,
        };
        Q_ENUM_NS(ColorAxis)
    }

}

#endif //SVSCRAFT_SVSCRAFTNAMESPACE_H
