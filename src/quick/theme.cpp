#include "theme.h"
#include "theme_p.h"

#include <QJSEngine>

namespace SVS {

    Theme *ThemeForeign::create(QQmlEngine *engine, QJSEngine *) {
        auto instance = Theme::instance();
        QJSEngine::setObjectOwnership(instance, QJSEngine::CppOwnership);
        return instance;
    }
    ThemeAttachedType *ThemedItem::qmlAttachedProperties(QObject *object) {
        return new ThemeAttachedType(object);
    }

    Theme ThemePrivate::instance;

    Theme::Theme(QObject *parent) : QObject(parent), d_ptr(new ThemePrivate) {
        Q_D(Theme);
        d->q_ptr = this;
    }
    Theme::~Theme() = default;
    Theme *Theme::instance() {
        return &ThemePrivate::instance;
    }

    QColor Theme::controlColor(SVS::Theme::ControlType controlType) const {
        switch (controlType) {
            case CT_Normal:
                return buttonColor();
            case CT_Accent:
                return accentColor();
            case CT_Warning:
                return warningColor();
            case CT_Error:
                return errorColor();
        }
        return {};
    }
    QColor Theme::accentColor() const {
        Q_D(const Theme);
        return d->accentColor;
    }
    void Theme::setAccentColor(const QColor &accentColor) {
        Q_D(Theme);
        if (d->accentColor != accentColor) {
            d->accentColor = accentColor;
            emit accentColorChanged(d->accentColor);
        }
    }
    QColor Theme::warningColor() const {
        Q_D(const Theme);
        return d->warningColor;
    }
    void Theme::setWarningColor(const QColor &warningColor) {
        Q_D(Theme);
        if (d->warningColor != warningColor) {
            d->warningColor = warningColor;
            emit warningColorChanged(d->warningColor);
        }
    }
    QColor Theme::errorColor() const {
        Q_D(const Theme);
        return d->errorColor;
    }
    void Theme::setErrorColor(const QColor &errorColor) {
        Q_D(Theme);
        if (d->errorColor != errorColor) {
            d->errorColor = errorColor;
            emit errorColorChanged(d->errorColor);
        }
    }
    QColor Theme::buttonColor() const {
        Q_D(const Theme);
        return d->buttonColor;
    }
    void Theme::setButtonColor(const QColor &buttonColor) {
        Q_D(Theme);
        if (d->buttonColor != buttonColor) {
            d->buttonColor = buttonColor;
            emit buttonColorChanged(d->buttonColor);
        }
    }
    QColor Theme::textFieldColor() const {
        Q_D(const Theme);
        return d->textFieldColor;
    }
    void Theme::setTextFieldColor(const QColor &textFieldColor) {
        Q_D(Theme);
        if (d->textFieldColor != textFieldColor) {
            d->textFieldColor = textFieldColor;
            emit textFieldColorChanged(d->textFieldColor);
        }
    }
    QColor Theme::borderColor() const {
        Q_D(const Theme);
        return d->borderColor;
    }
    void Theme::setBorderColor(const QColor &borderColor) {
        Q_D(Theme);
        if (d->borderColor != borderColor) {
            d->borderColor = borderColor;
            emit borderColorChanged(d->borderColor);
        }
    }
    QColor Theme::backgroundPrimaryColor() const {
        Q_D(const Theme);
        return d->backgroundPrimaryColor;
    }
    void Theme::setBackgroundPrimaryColor(const QColor &backgroundPrimaryColor) {
        Q_D(Theme);
        if (d->backgroundPrimaryColor != backgroundPrimaryColor) {
            d->backgroundPrimaryColor = backgroundPrimaryColor;
            emit backgroundPrimaryColorChanged(d->backgroundPrimaryColor);
        }
    }
    QColor Theme::backgroundSecondaryColor() const {
        Q_D(const Theme);
        return d->backgroundSecondaryColor;
    }
    void Theme::setBackgroundSecondaryColor(const QColor &backgroundSecondaryColor) {
        Q_D(Theme);
        if (d->backgroundSecondaryColor != backgroundSecondaryColor) {
            d->backgroundSecondaryColor = backgroundSecondaryColor;
            emit backgroundSecondaryColorChanged(d->backgroundSecondaryColor);
        }
    }
    QColor Theme::backgroundTertiaryColor() const {
        Q_D(const Theme);
        return d->backgroundTertiaryColor;
    }
    void Theme::setBackgroundTertiaryColor(const QColor &backgroundTertiaryColor) {
        Q_D(Theme);
        if (d->backgroundTertiaryColor != backgroundTertiaryColor) {
            d->backgroundTertiaryColor = backgroundTertiaryColor;
            emit backgroundTertiaryColorChanged(d->backgroundTertiaryColor);
        }
    }
    QColor Theme::backgroundQuaternaryColor() const {
        Q_D(const Theme);
        return d->backgroundQuaternaryColor;
    }
    void Theme::setBackgroundQuaternaryColor(const QColor &backgroundQuaternaryColor) {
        Q_D(Theme);
        if (d->backgroundQuaternaryColor != backgroundQuaternaryColor) {
            d->backgroundQuaternaryColor = backgroundQuaternaryColor;
            emit backgroundQuaternaryColorChanged(d->backgroundQuaternaryColor);
        }
    }
    QColor Theme::splitterColor() const {
        Q_D(const Theme);
        return d->splitterColor;
    }
    void Theme::setSplitterColor(const QColor &splitterColor) {
        Q_D(Theme);
        if (d->splitterColor != splitterColor) {
            d->splitterColor = splitterColor;
            emit splitterColorChanged(d->splitterColor);
        }
    }
    QColor Theme::foregroundPrimaryColor() const {
        Q_D(const Theme);
        return d->foregroundPrimaryColor;
    }
    void Theme::setForegroundPrimaryColor(const QColor &foregroundPrimaryColor) {
        Q_D(Theme);
        if (d->foregroundPrimaryColor != foregroundPrimaryColor) {
            d->foregroundPrimaryColor = foregroundPrimaryColor;
            emit foregroundPrimaryColorChanged(d->foregroundPrimaryColor);
        }
    }
    QColor Theme::foregroundSecondaryColor() const {
        Q_D(const Theme);
        return d->foregroundSecondaryColor;
    }
    void Theme::setForegroundSecondaryColor(const QColor &foregroundSecondaryColor) {
        Q_D(Theme);
        if (d->foregroundSecondaryColor != foregroundSecondaryColor) {
            d->foregroundSecondaryColor = foregroundSecondaryColor;
            emit foregroundSecondaryColorChanged(d->foregroundSecondaryColor);
        }
    }
    QColor Theme::linkColor() const {
        Q_D(const Theme);
        return d->linkColor;
    }
    void Theme::setLinkColor(const QColor &linkColor) {
        Q_D(Theme);
        if (d->linkColor != linkColor) {
            d->linkColor = linkColor;
            emit linkColorChanged(d->linkColor);
        }
    }
    QColor Theme::navigationColor() const {
        Q_D(const Theme);
        return d->navigationColor;
    }
    void Theme::setNavigationColor(const QColor &navigationColor) {
        Q_D(Theme);
        if (d->navigationColor != navigationColor) {
            d->navigationColor = navigationColor;
            emit navigationColorChanged(d->navigationColor);
        }
    }
    ColorChange Theme::controlDisabledColorChange() const {
        Q_D(const Theme);
        return d->controlDisabledColorChange;
    }
    void Theme::setControlDisabledColorChange(const ColorChange &controlDisabledColorChange) {
        Q_D(Theme);
        if (d->controlDisabledColorChange != controlDisabledColorChange) {
            d->controlDisabledColorChange = controlDisabledColorChange;
            emit controlDisabledColorChangeChanged(d->controlDisabledColorChange);
        }
    }
    ColorChange Theme::foregroundDisabledColorChange() const {
        Q_D(const Theme);
        return d->foregroundDisabledColorChange;
    }
    void Theme::setForegroundDisabledColorChange(const ColorChange &foregroundDisabledColorChange) {
        Q_D(Theme);
        if (d->foregroundDisabledColorChange != foregroundDisabledColorChange) {
            d->foregroundDisabledColorChange = foregroundDisabledColorChange;
            emit foregroundDisabledColorChangeChanged(d->foregroundDisabledColorChange);
        }
    }
    ColorChange Theme::controlHoveredColorChange() const {
        Q_D(const Theme);
        return d->controlHoveredColorChange;
    }
    void Theme::setControlHoveredColorChange(const ColorChange &controlHoveredColorChange) {
        Q_D(Theme);
        if (d->controlHoveredColorChange != controlHoveredColorChange) {
            d->controlHoveredColorChange = controlHoveredColorChange;
            emit controlHoveredColorChangeChanged(d->controlHoveredColorChange);
        }
    }
    ColorChange Theme::foregroundHoveredColorChange() const {
        Q_D(const Theme);
        return d->foregroundHoveredColorChange;
    }
    void Theme::setForegroundHoveredColorChange(const ColorChange &foregroundHoveredColorChange) {
        Q_D(Theme);
        if (d->foregroundHoveredColorChange != foregroundHoveredColorChange) {
            d->foregroundHoveredColorChange = foregroundHoveredColorChange;
            emit foregroundHoveredColorChangeChanged(d->foregroundHoveredColorChange);
        }
    }
    ColorChange Theme::controlPressedColorChange() const {
        Q_D(const Theme);
        return d->controlPressedColorChange;
    }
    void Theme::setControlPressedColorChange(const ColorChange &controlPressedColorChange) {
        Q_D(Theme);
        if (d->controlPressedColorChange != controlPressedColorChange) {
            d->controlPressedColorChange = controlPressedColorChange;
            emit controlPressedColorChangeChanged(d->controlPressedColorChange);
        }
    }
    ColorChange Theme::foregroundPressedColorChange() const {
        Q_D(const Theme);
        return d->foregroundPressedColorChange;
    }
    void Theme::setForegroundPressedColorChange(const ColorChange &foregroundPressedColorChange) {
        Q_D(Theme);
        if (d->foregroundPressedColorChange != foregroundPressedColorChange) {
            d->foregroundPressedColorChange = foregroundPressedColorChange;
            emit foregroundPressedColorChangeChanged(d->foregroundPressedColorChange);
        }
    }
    int Theme::colorAnimationDuration() const {
        Q_D(const Theme);
        return d->colorAnimationDuration;
    }
    void Theme::setColorAnimationDuration(int colorAnimationDuration) {
        Q_D(Theme);
        if (d->colorAnimationDuration != colorAnimationDuration) {
            d->colorAnimationDuration = colorAnimationDuration;
            emit colorAnimationDurationChanged(d->colorAnimationDuration);
        }
    }
    int Theme::visualEffectAnimationDuration() const {
        Q_D(const Theme);
        return d->visualEffectAnimationDuration;
    }
    void Theme::setVisualEffectAnimationDuration(int visualEffectAnimationDuration) {
        Q_D(Theme);
        if (d->visualEffectAnimationDuration != visualEffectAnimationDuration) {
            d->visualEffectAnimationDuration = visualEffectAnimationDuration;
            emit visualEffectAnimationDurationChanged(d->visualEffectAnimationDuration);
        }
    }
}

#include "moc_theme.cpp"