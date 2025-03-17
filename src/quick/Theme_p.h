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
#include <QSharedData>

#include <SVSCraftGui/ColorChange.h>

class QQmlEngine;
class QJSEngine;

namespace SVS {

    class ThemeAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(Theme)
        QML_ATTACHED(Theme)
    public:
        static Theme *qmlAttachedProperties(QObject *object);
    };
    class ThemePrivate : public QSharedData {
    public:
        static void inherit(Theme *object, Theme *parent);
        static Theme defaultTheme;

        QColor accentColor;
        QColor warningColor;
        QColor errorColor;
        QColor buttonColor;
        QColor textFieldColor;
        QColor scrollBarColor;
        QColor borderColor;
        QColor backgroundPrimaryColor;
        QColor backgroundSecondaryColor;
        QColor backgroundTertiaryColor;
        QColor backgroundQuaternaryColor;
        QColor splitterColor;
        QColor foregroundPrimaryColor;
        QColor foregroundSecondaryColor;
        QColor linkColor;
        QColor navigationColor;
        QColor shadowColor;
        QColor highlightColor;
        ColorChange controlDisabledColorChange;
        ColorChange foregroundDisabledColorChange;
        ColorChange controlHoveredColorChange;
        ColorChange foregroundHoveredColorChange;
        ColorChange controlPressedColorChange;
        ColorChange foregroundPressedColorChange;
        ColorChange controlCheckedColorChange;
        ColorChange annotationPopupTitleColorChange;
        ColorChange annotationPopupContentColorChange;
        int colorAnimationDuration{};
        int visualEffectAnimationDuration{};
        int toolTipDelay{};
        int toolTipTimeout = -1;
        bool doubleClickResetEnabled = true;

    };
}


#endif // SVSCRAFT_THEME_P_H
