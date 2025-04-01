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

#include <SVSCraftQml/private/SVSQmlNamespace_p.h>


template <typename>
struct ReturnTypeHelper;
template <typename ClassType, typename ReturnType>
struct ReturnTypeHelper<ReturnType (ClassType::*)() const> {
    using type = ReturnType;
};
#define GETTER(property, func) \
    ReturnTypeHelper<decltype(&Theme::func)>::type Theme::func() const { \
        using type = ReturnTypeHelper<decltype(&Theme::func)>::type; \
        Q_D(const Theme); \
        return d->getValue(#property).value<type>(); \
    }

template <typename>
struct ParameterTypeHelper;
template <typename ClassType, typename ReturnType, typename Arg>
struct ParameterTypeHelper<ReturnType (ClassType::*)(Arg)> {
    using type = Arg;
};

#define SETTER(property, func) \
    void Theme::func(ParameterTypeHelper<decltype(&Theme::func)>::type a) { \
        Q_D(Theme); \
        d->setValue(#property, QVariant::fromValue(a)); \
    }

#define RESETTER(property, func) \
    void Theme::func() { \
        Q_D(Theme); \
        d->resetValue(#property); \
    }

#define IMPLEMENTATION(property, getFunc, setFunc, resetFunc) GETTER(property, getFunc)SETTER(property, setFunc)RESETTER(property, resetFunc)

namespace SVS {

    Theme *ThemeAttachedType::qmlAttachedProperties(QObject *object) {
        return new Theme(object);
    }
    void ThemePrivate::propagateAndNotify(const QString &property) {
        Q_Q(Theme);
        for (auto child : q->attachedChildren()) {
            auto theme = static_cast<Theme *>(child);
            theme->d_func()->inherit(property);
        }
        auto i = q->metaObject()->indexOfProperty(property.toUtf8());
        q->metaObject()->property(i).notifySignal().invoke(q);
    }
    void ThemePrivate::inherit(const QString &property) {
        Q_Q(Theme);
        if (explicitSetProperties.contains(property))
            return;
        auto parent = q->attachedParent() ? static_cast<Theme *>(q->attachedParent())->d_func() : defaultTheme.d_func();
        auto v = parent->getValue(property);
        if (QVariant::compare(v, m.value(property)) == QPartialOrdering::equivalent)
            return;
        setValue(property, v);
        propagateAndNotify(property);
    }
    void ThemePrivate::inheritAll() {
        Q_Q(Theme);
        QStringList propertiesChanged;
        auto parent = q->attachedParent() ? static_cast<Theme *>(q->attachedParent())->d_func() : defaultTheme.d_func();
        if (explicitSetProperties.isEmpty()) {
            for (auto p = m.keyValueBegin(); p != m.keyValueEnd(); p++) {
                auto [property, value] = *p;
                if (QVariant::compare(value, parent->getValue(property)) != QPartialOrdering::equivalent)
                    propertiesChanged.append(property);
            }
            m = parent->m;
        } else {
            for (auto p = m.keyValueBegin(); p != m.keyValueEnd(); p++) {
                auto [property, value] = *p;
                if (explicitSetProperties.contains(property))
                    continue;
                if (QVariant::compare(value, parent->getValue(property)) != QPartialOrdering::equivalent) {
                    propertiesChanged.append(property);
                    value = parent->getValue(property);
                }
            }
        }
        for (const auto &property : propertiesChanged) {
            propagateAndNotify(property);
        }
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

    void Theme::attachedParentChange(QQuickAttachedPropertyPropagator *newParent, QQuickAttachedPropertyPropagator *oldParent) {
        Q_D(Theme);
        d->inheritAll();
    }
    Theme::Theme(QObject *parent) : QQuickAttachedPropertyPropagator(parent), d_ptr(new ThemePrivate) {
        Q_D(Theme);
        d->q_ptr = this;
        d->inheritAll();
        initialize();
    }
    Theme::Theme(ThemePrivate *d) : d_ptr(d) {
        d->q_ptr = this;
        initialize();
    }
    Theme::~Theme() = default;
    Theme *Theme::get(QObject *item) {
        return qobject_cast<Theme *>(qmlAttachedPropertiesObject<ThemeAttachedType>(item));
    }
    Theme ThemePrivate::defaultTheme = Theme(new ThemePrivate);
    Theme *Theme::defaultTheme() {
        return &ThemePrivate::defaultTheme;
    }

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

}

#include "moc_Theme.cpp"