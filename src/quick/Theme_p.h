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

#ifndef SVSCRAFT_THEME_P_H
#define SVSCRAFT_THEME_P_H

#include <SVSCraftQuick/Theme.h>

#include <QColor>
#include <QVariant>

#include <SVSCraftGui/ColorChange.h>
#include <SVSCraftQuick/AttachedPropertyPropagatorProperties.h>

class QQmlEngine;
class QJSEngine;

namespace SVS {

    class ThemePrivate : public AttachedPropertyPropagatorProperties {
    public:
        static Theme defaultTheme;

        inline explicit ThemePrivate(Theme *theme) : AttachedPropertyPropagatorProperties(theme, defaultTheme.properties(), {
            {"accentColor", QVariant::fromValue(QColor(0x5566ff))},
            {"warningColor", QVariant::fromValue(QColor(0xeeaa66))},
            {"errorColor", QVariant::fromValue(QColor(0xcc4455))},
            {"buttonColor", QVariant::fromValue(QColor(0x333437))},
            {"textFieldColor", QVariant::fromValue(QColor(0x27282b))},
            {"scrollBarColor", QVariant::fromValue(QColor::fromRgba(0x7f7f7f7f))},
            {"borderColor", QVariant::fromValue(QColor(0x4a4b4c))},
            {"backgroundPrimaryColor", QVariant::fromValue(QColor(0x212124))},
            {"backgroundSecondaryColor", QVariant::fromValue(QColor(0x232427))},
            {"backgroundTertiaryColor", QVariant::fromValue(QColor(0x252629))},
            {"backgroundQuaternaryColor", QVariant::fromValue(QColor(0x313235))},
            {"splitterColor", QVariant::fromValue(QColor(0x121315))},
            {"foregroundPrimaryColor", QVariant::fromValue(QColor(0xdadada))},
            {"foregroundSecondaryColor", QVariant::fromValue(QColor::fromRgba(0xa0dadada))},
            {"linkColor", QVariant::fromValue(QColor(0x5566ff))},
            {"navigationColor", QVariant::fromValue(QColor(0xffffff))},
            {"shadowColor", QVariant::fromValue(QColor(0x101113))},
            {"highlightColor", QVariant::fromValue(QColor(0xb28300))},
            {"controlDisabledColorChange", QVariant::fromValue(ColorChange(QColor::fromRgba(0x33000000)))},
            {"foregroundDisabledColorChange", QVariant::fromValue(ColorChange({}, 0.5))},
            {"controlHoveredColorChange", QVariant::fromValue(ColorChange(QColor::fromRgba(0x1affffff)))},
            {"foregroundHoveredColorChange", QVariant::fromValue(ColorChange())},
            {"controlPressedColorChange", QVariant::fromValue(ColorChange())},
            {"foregroundPressedColorChange", QVariant::fromValue(ColorChange({}, 0.8))},
            {"controlCheckedColorChange", QVariant::fromValue(ColorChange(QColor::fromRgba(0x1affffff)))},
            {"annotationPopupTitleColorChange", QVariant::fromValue(ColorChange({}, 0.72, 0x212124))},
            {"annotationPopupContentColorChange", QVariant::fromValue(ColorChange({}, 0.16, 0x212124))},
            {"colorAnimationDuration", 250},
            {"visualEffectAnimationDuration", 250},
            {"toolTipDelay", 1000},
            {"toolTipTimeout", -1},
            {"doubleClickResetEnabled", true},
        }) {
        }

    };
}


#endif // SVSCRAFT_THEME_P_H
