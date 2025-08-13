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
#include <QQmlComponent>

#include <SVSCraftQml/private/SVSQmlNamespace_p.h>

#define IMPLEMENTATION(property, getFunc, setFunc, resetFunc) SVS_ATTACHED_PROPERTY_PROPAGATOR_IMPLEMENTATION(Theme, property, getFunc, setFunc, resetFunc)

namespace SVS {

    static const char themeHelperPropertyName[] = "_svscraft_ThemeHelper";

    static void initializeThemeHelper(QQmlEngine *engine) {
        QQmlComponent component(engine, "SVSCraft.UIComponents.impl", "ThemeHelper");
        auto helper = component.create();
        Q_ASSERT(helper);
        helper->setParent(engine);
        engine->setProperty(themeHelperPropertyName, QVariant::fromValue(helper));
    }

    static QObject *getHelper(QQmlEngine *engine) {
        if (!engine)
            return nullptr;
        if (!engine->property(themeHelperPropertyName).value<QObject *>())
            initializeThemeHelper(engine);
        return engine->property(themeHelperPropertyName).value<QObject *>();
    }

    Theme *Theme::qmlAttachedProperties(QObject *object) {
        return new Theme(object);
    }
    QJSValue Theme::controlColor() const {
        auto helper = getHelper(qmlEngine(parent()));
        QVariant ret;
        QMetaObject::invokeMethod(helper, "controlColor", qReturnArg(ret), QVariant::fromValue(this));
        return ret.value<QJSValue>();
    }
    QJSValue Theme::backgroundColor() const {
        auto helper = getHelper(qmlEngine(parent()));
        QVariant ret;
        QMetaObject::invokeMethod(helper, "backgroundColor", qReturnArg(ret), QVariant::fromValue(this));
        return ret.value<QJSValue>();
    }
    QJSValue Theme::foregroundColor() const {
        auto helper = getHelper(qmlEngine(parent()));
        QVariant ret;
        QMetaObject::invokeMethod(helper, "foregroundColor", qReturnArg(ret), QVariant::fromValue(this));
        return ret.value<QJSValue>();
    }
    QJSValue Theme::dividerStrokeColor() const {
        auto helper = getHelper(qmlEngine(parent()));
        QVariant ret;
        QMetaObject::invokeMethod(helper, "dividerStrokeColor", qReturnArg(ret), QVariant::fromValue(this));
        return ret.value<QJSValue>();
    }

    AttachedPropertyPropagatorProperties *Theme::properties() const {
        return d.data();
    }
    Theme::Theme(QObject *parent) : AttachedPropertyPropagator(parent), d(new ThemePrivate(this)) {
        initialize();
        Theme::properties()->inheritAll();

        connect(this, &Theme::buttonColorChanged, this, &Theme::controlColorChanged);
        connect(this, &Theme::accentColorChanged, this, &Theme::controlColorChanged);
        connect(this, &Theme::warningColorChanged, this, &Theme::controlColorChanged);
        connect(this, &Theme::errorColorChanged, this, &Theme::controlColorChanged);

        connect(this, &Theme::backgroundPrimaryColorChanged, this, &Theme::backgroundColorChanged);
        connect(this, &Theme::backgroundSecondaryColorChanged, this, &Theme::backgroundColorChanged);
        connect(this, &Theme::backgroundTertiaryColorChanged, this, &Theme::backgroundColorChanged);
        connect(this, &Theme::backgroundQuaternaryColorChanged, this, &Theme::backgroundColorChanged);

        connect(this, &Theme::foregroundPrimaryColorChanged, this, &Theme::foregroundColorChanged);
        connect(this, &Theme::foregroundSecondaryColorChanged, this, &Theme::foregroundColorChanged);

        connect(this, &Theme::splitterColorChanged, this, &Theme::dividerStrokeColorChanged);
        connect(this, &Theme::borderColorChanged, this, &Theme::dividerStrokeColorChanged);
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