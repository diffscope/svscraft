#include "Theme.h"
#include "Theme_p.h"

#include <QQmlEngine>

#include <SVSCraftQml/private/SVSQmlNamespace_p.h>

namespace SVS {

    Theme *ThemeAttachedType::qmlAttachedProperties(QObject *object) {
        return new Theme(object);
    }
    QColor Theme::controlColor(int controlType) const {
        switch (controlType) {
            case SVSQmlNamespace::CT_Normal:
                return buttonColor();
            case SVSQmlNamespace::CT_Accent:
                return accentColor();
            case SVSQmlNamespace::CT_Warning:
                return warningColor();
            case SVSQmlNamespace::CT_Error:
                return errorColor();
        }
        return {};
    }
    QColor Theme::backgroundColor(int backgroundLevel) const {
        switch (backgroundLevel) {
            case SVSQmlNamespace::BL_Primary:
                return backgroundPrimaryColor();
            case SVSQmlNamespace::BL_Secondary:
                return backgroundSecondaryColor();
            case SVSQmlNamespace::BL_Tertiary:
                return backgroundTertiaryColor();
            case SVSQmlNamespace::BL_Quaternary:
                return backgroundQuaternaryColor();
        }
        return {};
    }
    QColor Theme::foregroundColor(int foregroundLevel) const {
        switch (foregroundLevel) {
            case SVSQmlNamespace::FL_Primary:
                return foregroundPrimaryColor();
            case SVSQmlNamespace::FL_Secondary:
                return foregroundSecondaryColor();
        }
        return {};
    }
    QColor Theme::dividerStrokeColor(int dividerStroke) const {
        return dividerStrokeColor(dividerStroke, Qt::transparent);
    }
    QColor Theme::dividerStrokeColor(int dividerStroke, const QColor &autoColor) const {
        switch (dividerStroke) {
            case SVSQmlNamespace::DS_Auto:
                return autoColor;
            case SVSQmlNamespace::DS_None:
                return Qt::transparent;
            case SVSQmlNamespace::DS_Border:
                return borderColor();
            case SVSQmlNamespace::DS_Splitter:
                return splitterColor();
        }
        return {};
    }
    void ThemePrivate::inherit(Theme *object, Theme *parent) {
        if (!parent)
            return;
        const QSharedDataPointer<ThemePrivate> p = object->d;
        object->d = parent->d;
        const QSharedDataPointer<ThemePrivate> p1 = object->d;
        if (p->accentColor != p1->accentColor)
            emit object->accentColorChanged(p1->accentColor);
        if (p->warningColor != p1->warningColor)
            emit object->warningColorChanged(p1->warningColor);
        if (p->errorColor != p1->errorColor)
            emit object->errorColorChanged(p1->errorColor);
        if (p->buttonColor != p1->buttonColor)
            emit object->buttonColorChanged(p1->buttonColor);
        if (p->textFieldColor != p1->textFieldColor)
            emit object->textFieldColorChanged(p1->textFieldColor);
        if (p->scrollBarColor != p1->scrollBarColor)
            emit object->scrollBarColorChanged(p1->scrollBarColor);
        if (p->borderColor != p1->borderColor)
            emit object->borderColorChanged(p1->borderColor);
        if (p->backgroundPrimaryColor != p1->backgroundPrimaryColor)
            emit object->backgroundPrimaryColorChanged(p1->backgroundPrimaryColor);
        if (p->backgroundSecondaryColor != p1->backgroundSecondaryColor)
            emit object->backgroundSecondaryColorChanged(p1->backgroundSecondaryColor);
        if (p->backgroundTertiaryColor != p1->backgroundTertiaryColor)
            emit object->backgroundTertiaryColorChanged(p1->backgroundTertiaryColor);
        if (p->backgroundQuaternaryColor != p1->backgroundQuaternaryColor)
            emit object->backgroundQuaternaryColorChanged(p1->backgroundQuaternaryColor);
        if (p->splitterColor != p1->splitterColor)
            emit object->splitterColorChanged(p1->splitterColor);
        if (p->foregroundPrimaryColor != p1->foregroundPrimaryColor)
            emit object->foregroundPrimaryColorChanged(p1->foregroundPrimaryColor);
        if (p->foregroundSecondaryColor != p1->foregroundSecondaryColor)
            emit object->foregroundSecondaryColorChanged(p1->foregroundSecondaryColor);
        if (p->linkColor != p1->linkColor)
            emit object->linkColorChanged(p1->linkColor);
        if (p->navigationColor != p1->navigationColor)
            emit object->navigationColorChanged(p1->navigationColor);
        if (p->shadowColor != p1->shadowColor)
            emit object->shadowColorChanged(p1->shadowColor);
        if (p->highlightColor != p1->highlightColor)
            emit object->highlightColorChanged(p1->highlightColor);
        if (p->controlDisabledColorChange != p1->controlDisabledColorChange)
            emit object->controlDisabledColorChangeChanged(p1->controlDisabledColorChange);
        if (p->foregroundDisabledColorChange != p1->foregroundDisabledColorChange)
            emit object->foregroundDisabledColorChangeChanged(p1->foregroundDisabledColorChange);
        if (p->controlHoveredColorChange != p1->controlHoveredColorChange)
            emit object->controlHoveredColorChangeChanged(p1->controlHoveredColorChange);
        if (p->foregroundHoveredColorChange != p1->foregroundHoveredColorChange)
            emit object->foregroundHoveredColorChangeChanged(p1->foregroundHoveredColorChange);
        if (p->controlPressedColorChange != p1->controlPressedColorChange)
            emit object->controlPressedColorChangeChanged(p1->controlPressedColorChange);
        if (p->foregroundPressedColorChange != p1->foregroundPressedColorChange)
            emit object->foregroundPressedColorChangeChanged(p1->foregroundPressedColorChange);
        if (p->controlCheckedColorChange != p1->controlCheckedColorChange)
            emit object->controlCheckedColorChangeChanged(p1->controlCheckedColorChange);
        if (p->colorAnimationDuration != p1->colorAnimationDuration)
            emit object->colorAnimationDurationChanged(p1->colorAnimationDuration);
        if (p->visualEffectAnimationDuration != p1->visualEffectAnimationDuration)
            emit object->visualEffectAnimationDurationChanged(p1->visualEffectAnimationDuration);
        if (p->toolTipDelay != p1->toolTipDelay)
            emit object->toolTipDelayChanged(p1->toolTipDelay);
        if (p->toolTipTimeout != p1->toolTipTimeout)
            emit object->toolTipTimeoutChanged(p1->toolTipTimeout);

        for (auto child : object->attachedChildren()) {
            inherit(qobject_cast<Theme *>(child), object);
        }
    }

    void Theme::attachedParentChange(QQuickAttachedPropertyPropagator *newParent, QQuickAttachedPropertyPropagator *oldParent) {
        ThemePrivate::inherit(this, qobject_cast<Theme *>(newParent));
    }
    Theme::Theme(QObject *parent) : QQuickAttachedPropertyPropagator(parent), d(ThemePrivate::defaultTheme.d) {
    }
    Theme::Theme(ThemePrivate *d) : d(d) {
    }
    Theme::~Theme() = default;
    Theme *Theme::get(QObject *item) {
        return qobject_cast<Theme *>(qmlAttachedPropertiesObject<ThemeAttachedType>(item));
    }
    Theme ThemePrivate::defaultTheme = Theme(new ThemePrivate);
    Theme *Theme::defaultTheme() {
        return &ThemePrivate::defaultTheme;
    }

    QColor Theme::accentColor() const {
        return d->accentColor;
    }
    void Theme::setAccentColor(const QColor &accentColor) {
        if (d->accentColor != accentColor) {
            d->accentColor = accentColor;
            emit accentColorChanged(d->accentColor);
        }
    }
    QColor Theme::warningColor() const {
        return d->warningColor;
    }
    void Theme::setWarningColor(const QColor &warningColor) {
        if (d->warningColor != warningColor) {
            d->warningColor = warningColor;
            emit warningColorChanged(d->warningColor);
        }
    }
    QColor Theme::errorColor() const {
        return d->errorColor;
    }
    void Theme::setErrorColor(const QColor &errorColor) {
        if (d->errorColor != errorColor) {
            d->errorColor = errorColor;
            emit errorColorChanged(d->errorColor);
        }
    }
    QColor Theme::buttonColor() const {
        return d->buttonColor;
    }
    void Theme::setButtonColor(const QColor &buttonColor) {
        if (d->buttonColor != buttonColor) {
            d->buttonColor = buttonColor;
            emit buttonColorChanged(d->buttonColor);
        }
    }
    QColor Theme::scrollBarColor() const {
        return d->scrollBarColor;
    }
    void Theme::setScrollBarColor(const QColor &scrollBarColor) {
        if (d->scrollBarColor != scrollBarColor) {
            d->scrollBarColor = scrollBarColor;
            emit scrollBarColorChanged(d->scrollBarColor);
        }
    }
    QColor Theme::textFieldColor() const {
        return d->textFieldColor;
    }
    void Theme::setTextFieldColor(const QColor &textFieldColor) {
        if (d->textFieldColor != textFieldColor) {
            d->textFieldColor = textFieldColor;
            emit textFieldColorChanged(d->textFieldColor);
        }
    }
    QColor Theme::borderColor() const {
        return d->borderColor;
    }
    void Theme::setBorderColor(const QColor &borderColor) {
        if (d->borderColor != borderColor) {
            d->borderColor = borderColor;
            emit borderColorChanged(d->borderColor);
        }
    }
    QColor Theme::backgroundPrimaryColor() const {
        return d->backgroundPrimaryColor;
    }
    void Theme::setBackgroundPrimaryColor(const QColor &backgroundPrimaryColor) {
        if (d->backgroundPrimaryColor != backgroundPrimaryColor) {
            d->backgroundPrimaryColor = backgroundPrimaryColor;
            emit backgroundPrimaryColorChanged(d->backgroundPrimaryColor);
        }
    }
    QColor Theme::backgroundSecondaryColor() const {
        return d->backgroundSecondaryColor;
    }
    void Theme::setBackgroundSecondaryColor(const QColor &backgroundSecondaryColor) {
        if (d->backgroundSecondaryColor != backgroundSecondaryColor) {
            d->backgroundSecondaryColor = backgroundSecondaryColor;
            emit backgroundSecondaryColorChanged(d->backgroundSecondaryColor);
        }
    }
    QColor Theme::backgroundTertiaryColor() const {
        return d->backgroundTertiaryColor;
    }
    void Theme::setBackgroundTertiaryColor(const QColor &backgroundTertiaryColor) {
        if (d->backgroundTertiaryColor != backgroundTertiaryColor) {
            d->backgroundTertiaryColor = backgroundTertiaryColor;
            emit backgroundTertiaryColorChanged(d->backgroundTertiaryColor);
        }
    }
    QColor Theme::backgroundQuaternaryColor() const {
        return d->backgroundQuaternaryColor;
    }
    void Theme::setBackgroundQuaternaryColor(const QColor &backgroundQuaternaryColor) {
        if (d->backgroundQuaternaryColor != backgroundQuaternaryColor) {
            d->backgroundQuaternaryColor = backgroundQuaternaryColor;
            emit backgroundQuaternaryColorChanged(d->backgroundQuaternaryColor);
        }
    }
    QColor Theme::splitterColor() const {
        return d->splitterColor;
    }
    void Theme::setSplitterColor(const QColor &splitterColor) {
        if (d->splitterColor != splitterColor) {
            d->splitterColor = splitterColor;
            emit splitterColorChanged(d->splitterColor);
        }
    }
    QColor Theme::foregroundPrimaryColor() const {
        return d->foregroundPrimaryColor;
    }
    void Theme::setForegroundPrimaryColor(const QColor &foregroundPrimaryColor) {
        if (d->foregroundPrimaryColor != foregroundPrimaryColor) {
            d->foregroundPrimaryColor = foregroundPrimaryColor;
            emit foregroundPrimaryColorChanged(d->foregroundPrimaryColor);
        }
    }
    QColor Theme::foregroundSecondaryColor() const {
        return d->foregroundSecondaryColor;
    }
    void Theme::setForegroundSecondaryColor(const QColor &foregroundSecondaryColor) {
        if (d->foregroundSecondaryColor != foregroundSecondaryColor) {
            d->foregroundSecondaryColor = foregroundSecondaryColor;
            emit foregroundSecondaryColorChanged(d->foregroundSecondaryColor);
        }
    }
    QColor Theme::linkColor() const {
        return d->linkColor;
    }
    void Theme::setLinkColor(const QColor &linkColor) {
        if (d->linkColor != linkColor) {
            d->linkColor = linkColor;
            emit linkColorChanged(d->linkColor);
        }
    }
    QColor Theme::navigationColor() const {
        return d->navigationColor;
    }
    void Theme::setNavigationColor(const QColor &navigationColor) {
        if (d->navigationColor != navigationColor) {
            d->navigationColor = navigationColor;
            emit navigationColorChanged(d->navigationColor);
        }
    }
    QColor Theme::shadowColor() const {
        return d->shadowColor;
    }
    void Theme::setShadowColor(const QColor &shadowColor) {
        if (d->shadowColor != shadowColor) {
            d->shadowColor = shadowColor;
            emit shadowColorChanged(d->shadowColor);
        }
    }
    QColor Theme::highlightColor() const {
        return d->highlightColor;
    }
    void Theme::setHighlightColor(const QColor &highlightColor) {
        if (d->highlightColor != highlightColor) {
            d->highlightColor = highlightColor;
            emit highlightColorChanged(d->highlightColor);
        }
    }
    ColorChange Theme::controlDisabledColorChange() const {
        return d->controlDisabledColorChange;
    }
    void Theme::setControlDisabledColorChange(const ColorChange &controlDisabledColorChange) {
        if (d->controlDisabledColorChange != controlDisabledColorChange) {
            d->controlDisabledColorChange = controlDisabledColorChange;
            emit controlDisabledColorChangeChanged(d->controlDisabledColorChange);
        }
    }
    ColorChange Theme::foregroundDisabledColorChange() const {
        return d->foregroundDisabledColorChange;
    }
    void Theme::setForegroundDisabledColorChange(const ColorChange &foregroundDisabledColorChange) {
        if (d->foregroundDisabledColorChange != foregroundDisabledColorChange) {
            d->foregroundDisabledColorChange = foregroundDisabledColorChange;
            emit foregroundDisabledColorChangeChanged(d->foregroundDisabledColorChange);
        }
    }
    ColorChange Theme::controlHoveredColorChange() const {
        return d->controlHoveredColorChange;
    }
    void Theme::setControlHoveredColorChange(const ColorChange &controlHoveredColorChange) {
        if (d->controlHoveredColorChange != controlHoveredColorChange) {
            d->controlHoveredColorChange = controlHoveredColorChange;
            emit controlHoveredColorChangeChanged(d->controlHoveredColorChange);
        }
    }
    ColorChange Theme::foregroundHoveredColorChange() const {
        return d->foregroundHoveredColorChange;
    }
    void Theme::setForegroundHoveredColorChange(const ColorChange &foregroundHoveredColorChange) {
        if (d->foregroundHoveredColorChange != foregroundHoveredColorChange) {
            d->foregroundHoveredColorChange = foregroundHoveredColorChange;
            emit foregroundHoveredColorChangeChanged(d->foregroundHoveredColorChange);
        }
    }
    ColorChange Theme::controlPressedColorChange() const {
        return d->controlPressedColorChange;
    }
    void Theme::setControlPressedColorChange(const ColorChange &controlPressedColorChange) {
        if (d->controlPressedColorChange != controlPressedColorChange) {
            d->controlPressedColorChange = controlPressedColorChange;
            emit controlPressedColorChangeChanged(d->controlPressedColorChange);
        }
    }
    ColorChange Theme::foregroundPressedColorChange() const {
        return d->foregroundPressedColorChange;
    }
    void Theme::setForegroundPressedColorChange(const ColorChange &foregroundPressedColorChange) {
        if (d->foregroundPressedColorChange != foregroundPressedColorChange) {
            d->foregroundPressedColorChange = foregroundPressedColorChange;
            emit foregroundPressedColorChangeChanged(d->foregroundPressedColorChange);
        }
    }
    ColorChange Theme::controlCheckedColorChange() const {
        return d->controlCheckedColorChange;
    }
    void Theme::setControlCheckedColorChange(const ColorChange &controlCheckedColorChange) {
        if (d->controlCheckedColorChange != controlCheckedColorChange) {
            d->controlCheckedColorChange = controlCheckedColorChange;
            emit controlCheckedColorChangeChanged(d->controlCheckedColorChange);
        }
    }
    ColorChange Theme::annotationPopupTitleColorChange() const {
        return d->annotationPopupTitleColorChange;
    }
    void Theme::setAnnotationPopupTitleColorChange(const ColorChange &annotationPopupTitleColorChange) {
        if (d->annotationPopupTitleColorChange != annotationPopupTitleColorChange) {
            d->annotationPopupTitleColorChange = annotationPopupTitleColorChange;
            emit annotationPopupTitleColorChangeChanged(d->annotationPopupTitleColorChange);
        }
    }
    ColorChange Theme::annotationPopupContentColorChange() const {
        return d->annotationPopupContentColorChange;
    }
    void Theme::setAnnotationPopupContentColorChange(const ColorChange &annotationPopupContentColorChange) {
        if (d->annotationPopupContentColorChange != annotationPopupContentColorChange) {
            d->annotationPopupContentColorChange = annotationPopupContentColorChange;
            emit annotationPopupContentColorChangeChanged(d->annotationPopupContentColorChange);
        }
    }
    int Theme::colorAnimationDuration() const {
        return d->colorAnimationDuration;
    }
    void Theme::setColorAnimationDuration(int colorAnimationDuration) {
        if (d->colorAnimationDuration != colorAnimationDuration) {
            d->colorAnimationDuration = colorAnimationDuration;
            emit colorAnimationDurationChanged(d->colorAnimationDuration);
        }
    }
    int Theme::visualEffectAnimationDuration() const {
        return d->visualEffectAnimationDuration;
    }
    void Theme::setVisualEffectAnimationDuration(int visualEffectAnimationDuration) {
        if (d->visualEffectAnimationDuration != visualEffectAnimationDuration) {
            d->visualEffectAnimationDuration = visualEffectAnimationDuration;
            emit visualEffectAnimationDurationChanged(d->visualEffectAnimationDuration);
        }
    }
    int Theme::toolTipDelay() const {
        return d->toolTipDelay;
    }
    void Theme::setToolTipDelay(int toolTipDelay) {
        if (d->toolTipDelay != toolTipDelay) {
            d->toolTipDelay = toolTipDelay;
            emit toolTipDelayChanged(d->toolTipDelay);
        }
    }
    int Theme::toolTipTimeout() const {
        return d->toolTipTimeout;
    }
    void Theme::setToolTipTimeout(int toolTipTimeout) {
        if (d->toolTipTimeout != toolTipTimeout) {
            d->toolTipTimeout = toolTipTimeout;
            emit toolTipTimeoutChanged(d->toolTipTimeout);
        }
    }
}

#include "moc_Theme.cpp"