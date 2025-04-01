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

#ifndef SVSCRAFT_THEME_H
#define SVSCRAFT_THEME_H

#include <qqmlintegration.h>

#include <SVSCraftQuick/AttachedPropertyPropagator.h>

namespace SVS {

    class ColorChange;

    class ThemePrivate;

    class SVSCRAFT_QUICK_EXPORT Theme : public AttachedPropertyPropagator {
        Q_OBJECT
        QML_ELEMENT
        QML_ATTACHED(Theme)
        QML_UNCREATABLE("")

        Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor RESET resetAccentColor NOTIFY accentColorChanged)
        Q_PROPERTY(QColor warningColor READ warningColor WRITE setWarningColor RESET resetWarningColor NOTIFY warningColorChanged)
        Q_PROPERTY(QColor errorColor READ errorColor WRITE setErrorColor RESET resetErrorColor NOTIFY errorColorChanged)

        Q_PROPERTY(QColor buttonColor READ buttonColor WRITE setButtonColor RESET resetButtonColor NOTIFY buttonColorChanged)
        Q_PROPERTY(QColor textFieldColor READ textFieldColor WRITE setTextFieldColor RESET resetTextFieldColor NOTIFY textFieldColorChanged)
        Q_PROPERTY(QColor scrollBarColor READ scrollBarColor WRITE setScrollBarColor RESET resetScrollBarColor NOTIFY scrollBarColorChanged)
        Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor RESET resetBorderColor NOTIFY borderColorChanged)

        Q_PROPERTY(QColor backgroundPrimaryColor READ backgroundPrimaryColor WRITE setBackgroundPrimaryColor RESET resetBackgroundPrimaryColor NOTIFY backgroundPrimaryColorChanged)
        Q_PROPERTY(QColor backgroundSecondaryColor READ backgroundSecondaryColor WRITE setBackgroundSecondaryColor RESET resetBackgroundSecondaryColor NOTIFY backgroundSecondaryColorChanged)
        Q_PROPERTY(QColor backgroundTertiaryColor READ backgroundTertiaryColor WRITE setBackgroundTertiaryColor RESET resetBackgroundTertiaryColor NOTIFY backgroundTertiaryColorChanged)
        Q_PROPERTY(QColor backgroundQuaternaryColor READ backgroundQuaternaryColor WRITE setBackgroundQuaternaryColor RESET resetBackgroundQuaternaryColor NOTIFY backgroundQuaternaryColorChanged)
        Q_PROPERTY(QColor splitterColor READ splitterColor WRITE setSplitterColor RESET resetSplitterColor NOTIFY splitterColorChanged)

        Q_PROPERTY(QColor foregroundPrimaryColor READ foregroundPrimaryColor WRITE setForegroundPrimaryColor RESET resetForegroundPrimaryColor NOTIFY foregroundPrimaryColorChanged)
        Q_PROPERTY(QColor foregroundSecondaryColor READ foregroundSecondaryColor WRITE setForegroundSecondaryColor RESET resetForegroundSecondaryColor NOTIFY foregroundSecondaryColorChanged)
        Q_PROPERTY(QColor linkColor READ linkColor WRITE setLinkColor RESET resetLinkColor NOTIFY linkColorChanged)

        Q_PROPERTY(QColor navigationColor READ navigationColor WRITE setNavigationColor RESET resetNavigationColor NOTIFY navigationColorChanged)
        Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor RESET resetShadowColor NOTIFY shadowColorChanged)
        Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor RESET resetHighlightColor NOTIFY highlightColorChanged)

        Q_PROPERTY(ColorChange controlDisabledColorChange READ controlDisabledColorChange WRITE setControlDisabledColorChange RESET resetControlDisabledColorChange NOTIFY controlDisabledColorChangeChanged)
        Q_PROPERTY(ColorChange foregroundDisabledColorChange READ foregroundDisabledColorChange WRITE setForegroundDisabledColorChange RESET resetForegroundDisabledColorChange NOTIFY foregroundDisabledColorChangeChanged)
        Q_PROPERTY(ColorChange controlHoveredColorChange READ controlHoveredColorChange WRITE setControlHoveredColorChange RESET resetControlHoveredColorChange NOTIFY controlHoveredColorChangeChanged)
        Q_PROPERTY(ColorChange foregroundHoveredColorChange READ foregroundHoveredColorChange WRITE setForegroundHoveredColorChange RESET resetForegroundHoveredColorChange NOTIFY foregroundHoveredColorChangeChanged)
        Q_PROPERTY(ColorChange controlPressedColorChange READ controlPressedColorChange WRITE setControlPressedColorChange RESET resetControlPressedColorChange NOTIFY controlPressedColorChangeChanged)
        Q_PROPERTY(ColorChange foregroundPressedColorChange READ foregroundPressedColorChange WRITE setForegroundPressedColorChange RESET resetForegroundPressedColorChange NOTIFY foregroundPressedColorChangeChanged)
        Q_PROPERTY(ColorChange controlCheckedColorChange READ controlCheckedColorChange WRITE setControlCheckedColorChange RESET resetControlCheckedColorChange NOTIFY controlCheckedColorChangeChanged)

        Q_PROPERTY(ColorChange annotationPopupTitleColorChange READ annotationPopupTitleColorChange WRITE setAnnotationPopupTitleColorChange RESET resetAnnotationPopupTitleColorChange NOTIFY annotationPopupTitleColorChangeChanged)
        Q_PROPERTY(ColorChange annotationPopupContentColorChange READ annotationPopupContentColorChange WRITE setAnnotationPopupContentColorChange RESET resetAnnotationPopupContentColorChange NOTIFY annotationPopupContentColorChangeChanged)


        Q_PROPERTY(int colorAnimationDuration READ colorAnimationDuration WRITE setColorAnimationDuration RESET resetColorAnimationDuration NOTIFY colorAnimationDurationChanged)
        Q_PROPERTY(int visualEffectAnimationDuration READ visualEffectAnimationDuration WRITE setVisualEffectAnimationDuration RESET resetVisualEffectAnimationDuration NOTIFY visualEffectAnimationDurationChanged)

        Q_PROPERTY(int toolTipDelay READ toolTipDelay WRITE setToolTipDelay RESET resetToolTipDelay NOTIFY toolTipDelayChanged)
        Q_PROPERTY(int toolTipTimeout READ toolTipTimeout WRITE setToolTipTimeout RESET resetToolTipTimeout NOTIFY toolTipTimeoutChanged)

        Q_PROPERTY(bool doubleClickResetEnabled READ doubleClickResetEnabled WRITE setDoubleClickResetEnabled RESET resetDoubleClickResetEnabled NOTIFY doubleClickResetEnabledChanged)

    public:
        ~Theme() override;

        static Theme *qmlAttachedProperties(QObject *object);
        static Theme *get(QObject *item);
        static Theme *defaultTheme();

        QColor accentColor() const;
        void setAccentColor(const QColor &accentColor);
        void resetAccentColor();

        QColor warningColor() const;
        void setWarningColor(const QColor &warningColor);
        void resetWarningColor();

        QColor errorColor() const;
        void setErrorColor(const QColor &errorColor);
        void resetErrorColor();

        QColor buttonColor() const;
        void setButtonColor(const QColor &buttonColor);
        void resetButtonColor();

        QColor scrollBarColor() const;
        void setScrollBarColor(const QColor &scrollBarColor);
        void resetScrollBarColor();

        QColor textFieldColor() const;
        void setTextFieldColor(const QColor &textFieldColor);
        void resetTextFieldColor();

        QColor borderColor() const;
        void setBorderColor(const QColor &borderColor);
        void resetBorderColor();

        QColor backgroundPrimaryColor() const;
        void setBackgroundPrimaryColor(const QColor &backgroundPrimaryColor);
        void resetBackgroundPrimaryColor();

        QColor backgroundSecondaryColor() const;
        void setBackgroundSecondaryColor(const QColor &backgroundSecondaryColor);
        void resetBackgroundSecondaryColor();

        QColor backgroundTertiaryColor() const;
        void setBackgroundTertiaryColor(const QColor &backgroundTertiaryColor);
        void resetBackgroundTertiaryColor();

        QColor backgroundQuaternaryColor() const;
        void setBackgroundQuaternaryColor(const QColor &backgroundQuaternaryColor);
        void resetBackgroundQuaternaryColor();

        QColor splitterColor() const;
        void setSplitterColor(const QColor &splitterColor);
        void resetSplitterColor();

        QColor foregroundPrimaryColor() const;
        void setForegroundPrimaryColor(const QColor &foregroundPrimaryColor);
        void resetForegroundPrimaryColor();

        QColor foregroundSecondaryColor() const;
        void setForegroundSecondaryColor(const QColor &foregroundSecondaryColor);
        void resetForegroundSecondaryColor();

        QColor linkColor() const;
        void setLinkColor(const QColor &linkColor);
        void resetLinkColor();

        QColor navigationColor() const;
        void setNavigationColor(const QColor &navigationColor);
        void resetNavigationColor();

        QColor shadowColor() const;
        void setShadowColor(const QColor &shadowColor);
        void resetShadowColor();

        QColor highlightColor() const;
        void setHighlightColor(const QColor &highlightColor);
        void resetHighlightColor();

        ColorChange controlDisabledColorChange() const;
        void setControlDisabledColorChange(const ColorChange &controlDisabledColorChange);
        void resetControlDisabledColorChange();

        ColorChange foregroundDisabledColorChange() const;
        void setForegroundDisabledColorChange(const ColorChange &foregroundDisabledColorChange);
        void resetForegroundDisabledColorChange();

        ColorChange controlHoveredColorChange() const;
        void setControlHoveredColorChange(const ColorChange &controlHoveredColorChange);
        void resetControlHoveredColorChange();

        ColorChange foregroundHoveredColorChange() const;
        void setForegroundHoveredColorChange(const ColorChange &foregroundHoveredColorChange);
        void resetForegroundHoveredColorChange();

        ColorChange controlPressedColorChange() const;
        void setControlPressedColorChange(const ColorChange &controlPressedColorChange);
        void resetControlPressedColorChange();

        ColorChange foregroundPressedColorChange() const;
        void setForegroundPressedColorChange(const ColorChange &foregroundPressedColorChange);
        void resetForegroundPressedColorChange();

        ColorChange controlCheckedColorChange() const;
        void setControlCheckedColorChange(const ColorChange &controlCheckedColorChange);
        void resetControlCheckedColorChange();

        ColorChange annotationPopupTitleColorChange() const;
        void setAnnotationPopupTitleColorChange(const ColorChange &annotationPopupTitleColorChange);
        void resetAnnotationPopupTitleColorChange();

        ColorChange annotationPopupContentColorChange() const;
        void setAnnotationPopupContentColorChange(const ColorChange &annotationPopupContentColorChange);
        void resetAnnotationPopupContentColorChange();

        int colorAnimationDuration() const;
        void setColorAnimationDuration(int colorAnimationDuration);
        void resetColorAnimationDuration();

        int visualEffectAnimationDuration() const;
        void setVisualEffectAnimationDuration(int visualEffectAnimationDuration);
        void resetVisualEffectAnimationDuration();

        int toolTipDelay() const;
        void setToolTipDelay(int toolTipDelay);
        void resetToolTipDelay();

        int toolTipTimeout() const;
        void setToolTipTimeout(int toolTipTimeout);
        void resetToolTipTimeout();

        bool doubleClickResetEnabled() const;
        void setDoubleClickResetEnabled(bool doubleClickResetEnabled);
        void resetDoubleClickResetEnabled();

        Q_INVOKABLE QColor controlColor(int controlType) const;
        Q_INVOKABLE QColor backgroundColor(int backgroundLevel) const;
        Q_INVOKABLE QColor foregroundColor(int foregroundLevel) const;
        Q_INVOKABLE QColor dividerStrokeColor(int dividerStroke) const;
        Q_INVOKABLE QColor dividerStrokeColor(int dividerStroke, const QColor &autoColor) const;

    signals:
        void accentColorChanged();
        void warningColorChanged();
        void errorColorChanged();
        void buttonColorChanged();
        void textFieldColorChanged();
        void scrollBarColorChanged();
        void borderColorChanged();
        void backgroundPrimaryColorChanged();
        void backgroundSecondaryColorChanged();
        void backgroundTertiaryColorChanged();
        void backgroundQuaternaryColorChanged();
        void splitterColorChanged();
        void foregroundPrimaryColorChanged();
        void foregroundSecondaryColorChanged();
        void linkColorChanged();
        void navigationColorChanged();
        void shadowColorChanged();
        void highlightColorChanged();
        void controlDisabledColorChangeChanged();
        void foregroundDisabledColorChangeChanged();
        void controlHoveredColorChangeChanged();
        void foregroundHoveredColorChangeChanged();
        void controlPressedColorChangeChanged();
        void foregroundPressedColorChangeChanged();
        void controlCheckedColorChangeChanged();
        void annotationPopupTitleColorChangeChanged();
        void annotationPopupContentColorChangeChanged();
        void colorAnimationDurationChanged();
        void visualEffectAnimationDurationChanged();
        void toolTipDelayChanged();
        void toolTipTimeoutChanged();
        void doubleClickResetEnabledChanged();

    protected:
        AttachedPropertyPropagatorProperties *properties() const override;

    private:
        friend class ThemePrivate;
        explicit Theme(QObject *parent = nullptr);
        explicit Theme(QPrivateSignal);
        QScopedPointer<ThemePrivate> d;

    };

}

#endif // SVSCRAFT_THEME_H
