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

#include "Theme.h"
#include "Theme_p.h"

#include <type_traits>

#include <QQmlEngine>
#include <QFont>

#include <SVSCraftQml/private/SVSQmlNamespace_p.h>

#define IMPLEMENTATION(property, getFunc, setFunc, resetFunc) SVS_ATTACHED_PROPERTY_PROPAGATOR_IMPLEMENTATION(Theme, property, getFunc, setFunc, resetFunc)

namespace SVS {

    Theme *Theme::qmlAttachedProperties(QObject *object) {
        return new Theme(object);
    }
    QColor Theme::controlColor(int controlType) const {
        switch (controlType) {
            case SVSCraft::CT_Normal:
                return buttonColor();
            case SVSCraft::CT_Accent:
                return accentColor();
            case SVSCraft::CT_Warning:
                return warningColor();
            case SVSCraft::CT_Error:
                return errorColor();
        }
        return {};
    }
    QColor Theme::backgroundColor(int backgroundLevel) const {
        switch (backgroundLevel) {
            case SVSCraft::BL_Primary:
                return backgroundPrimaryColor();
            case SVSCraft::BL_Secondary:
                return backgroundSecondaryColor();
            case SVSCraft::BL_Tertiary:
                return backgroundTertiaryColor();
            case SVSCraft::BL_Quaternary:
                return backgroundQuaternaryColor();
        }
        return {};
    }
    QColor Theme::foregroundColor(int foregroundLevel) const {
        switch (foregroundLevel) {
            case SVSCraft::FL_Primary:
                return foregroundPrimaryColor();
            case SVSCraft::FL_Secondary:
                return foregroundSecondaryColor();
        }
        return {};
    }
    QColor Theme::dividerStrokeColor(int dividerStroke) const {
        return dividerStrokeColor(dividerStroke, Qt::transparent);
    }
    QColor Theme::dividerStrokeColor(int dividerStroke, const QColor &autoColor) const {
        switch (dividerStroke) {
            case SVSCraft::DS_Auto:
                return autoColor;
            case SVSCraft::DS_None:
                return Qt::transparent;
            case SVSCraft::DS_Border:
                return borderColor();
            case SVSCraft::DS_Splitter:
                return splitterColor();
        }
        return {};
    }

    AttachedPropertyPropagatorProperties *Theme::properties() const {
        return d.data();
    }
    Theme::Theme(QObject *parent) : AttachedPropertyPropagator(parent), d(new ThemePrivate(this)) {
        initialize();
        Theme::properties()->inheritAll();
    }
    Theme::Theme(QPrivateSignal) : d(new ThemePrivate(this)) {
        initialize();
    }
    Theme::~Theme() = default;
    Theme *Theme::get(QObject *item) {
        return qobject_cast<Theme *>(qmlAttachedPropertiesObject<Theme>(item));
    }
    Theme ThemePrivate::defaultTheme = Theme(Theme::QPrivateSignal{});
    Theme *Theme::defaultTheme() {
        return &ThemePrivate::defaultTheme;
    }

    IMPLEMENTATION(font, font, setFont, resetFont)

    IMPLEMENTATION(accentColor, accentColor, setAccentColor, resetAccentColor)
    IMPLEMENTATION(warningColor, warningColor, setWarningColor, resetWarningColor)
    IMPLEMENTATION(errorColor, errorColor, setErrorColor, resetErrorColor)

    IMPLEMENTATION(buttonColor, buttonColor, setButtonColor, resetButtonColor)
    IMPLEMENTATION(textFieldColor, textFieldColor, setTextFieldColor, resetTextFieldColor)
    IMPLEMENTATION(scrollBarColor, scrollBarColor, setScrollBarColor, resetScrollBarColor)
    IMPLEMENTATION(borderColor, borderColor, setBorderColor, resetBorderColor)

    IMPLEMENTATION(backgroundPrimaryColor, backgroundPrimaryColor, setBackgroundPrimaryColor, resetBackgroundPrimaryColor)
    IMPLEMENTATION(backgroundSecondaryColor, backgroundSecondaryColor, setBackgroundSecondaryColor, resetBackgroundSecondaryColor)
    IMPLEMENTATION(backgroundTertiaryColor, backgroundTertiaryColor, setBackgroundTertiaryColor, resetBackgroundTertiaryColor)
    IMPLEMENTATION(backgroundQuaternaryColor, backgroundQuaternaryColor, setBackgroundQuaternaryColor, resetBackgroundQuaternaryColor)
    IMPLEMENTATION(splitterColor, splitterColor, setSplitterColor, resetSplitterColor)

    IMPLEMENTATION(foregroundPrimaryColor, foregroundPrimaryColor, setForegroundPrimaryColor, resetForegroundPrimaryColor)
    IMPLEMENTATION(foregroundSecondaryColor, foregroundSecondaryColor, setForegroundSecondaryColor, resetForegroundSecondaryColor)
    IMPLEMENTATION(linkColor, linkColor, setLinkColor, resetLinkColor)

    IMPLEMENTATION(navigationColor, navigationColor, setNavigationColor, resetNavigationColor)
    IMPLEMENTATION(shadowColor, shadowColor, setShadowColor, resetShadowColor)
    IMPLEMENTATION(highlightColor, highlightColor, setHighlightColor, resetHighlightColor)

    IMPLEMENTATION(controlDisabledColorChange, controlDisabledColorChange, setControlDisabledColorChange, resetControlDisabledColorChange)
    IMPLEMENTATION(foregroundDisabledColorChange, foregroundDisabledColorChange, setForegroundDisabledColorChange, resetForegroundDisabledColorChange)
    IMPLEMENTATION(controlHoveredColorChange, controlHoveredColorChange, setControlHoveredColorChange, resetControlHoveredColorChange)
    IMPLEMENTATION(foregroundHoveredColorChange, foregroundHoveredColorChange, setForegroundHoveredColorChange, resetForegroundHoveredColorChange)
    IMPLEMENTATION(controlPressedColorChange, controlPressedColorChange, setControlPressedColorChange, resetControlPressedColorChange)
    IMPLEMENTATION(foregroundPressedColorChange, foregroundPressedColorChange, setForegroundPressedColorChange, resetForegroundPressedColorChange)
    IMPLEMENTATION(controlCheckedColorChange, controlCheckedColorChange, setControlCheckedColorChange, resetControlCheckedColorChange)

    IMPLEMENTATION(annotationPopupTitleColorChange, annotationPopupTitleColorChange, setAnnotationPopupTitleColorChange, resetAnnotationPopupTitleColorChange)
    IMPLEMENTATION(annotationPopupContentColorChange, annotationPopupContentColorChange, setAnnotationPopupContentColorChange, resetAnnotationPopupContentColorChange)


    IMPLEMENTATION(colorAnimationDuration, colorAnimationDuration, setColorAnimationDuration, resetColorAnimationDuration)
    IMPLEMENTATION(visualEffectAnimationDuration, visualEffectAnimationDuration, setVisualEffectAnimationDuration, resetVisualEffectAnimationDuration)

    IMPLEMENTATION(toolTipDelay, toolTipDelay, setToolTipDelay, resetToolTipDelay)
    IMPLEMENTATION(toolTipTimeout, toolTipTimeout, setToolTipTimeout, resetToolTipTimeout)

    IMPLEMENTATION(doubleClickResetEnabled, doubleClickResetEnabled, setDoubleClickResetEnabled, resetDoubleClickResetEnabled)

    IMPLEMENTATION(splitterHandleSize, splitterHandleSize, setSplitterHandleSize, resetSplitterHandleSize)


}

#include "moc_Theme.cpp"