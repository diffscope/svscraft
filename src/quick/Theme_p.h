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

        QColor accentColor = 0x5566ff;
        QColor warningColor = 0xeeaa66;
        QColor errorColor = 0xcc4455;
        QColor buttonColor = 0x333437;
        QColor textFieldColor = 0x27282b;
        QColor scrollBarColor = QColor::fromRgba(0x7f7f7f7f);
        QColor borderColor = 0x4a4b4c;
        QColor backgroundPrimaryColor = 0x212124;
        QColor backgroundSecondaryColor = 0x232427;
        QColor backgroundTertiaryColor = 0x252629;
        QColor backgroundQuaternaryColor = 0x313235;
        QColor splitterColor = 0x121315;
        QColor foregroundPrimaryColor = 0xdadada;
        QColor foregroundSecondaryColor = QColor::fromRgba(0xa0dadada);
        QColor linkColor = 0x5566ff;
        QColor navigationColor = 0xffffff;
        QColor shadowColor = 0x101113;
        QColor highlightColor = 0xb28300;
        ColorChange controlDisabledColorChange{QColor::fromRgba(0x33000000)};
        ColorChange foregroundDisabledColorChange{{}, 0.5};
        ColorChange controlHoveredColorChange{QColor::fromRgba(0x1affffff)};
        ColorChange foregroundHoveredColorChange{};
        ColorChange controlPressedColorChange{};
        ColorChange foregroundPressedColorChange{{}, 0.8};
        ColorChange controlCheckedColorChange{QColor::fromRgba(0x1affffff)};
        ColorChange annotationPopupTitleColorChange{{}, 0.72, 0x212124};
        ColorChange annotationPopupContentColorChange{{}, 0.16, 0x212124};
        int colorAnimationDuration{250};
        int visualEffectAnimationDuration{250};
        int toolTipDelay{1000};
        int toolTipTimeout = -1;
        bool doubleClickResetEnabled = true;

    };
}


#endif // SVSCRAFT_THEME_P_H
