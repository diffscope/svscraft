#ifndef SVSCRAFT_THEME_H
#define SVSCRAFT_THEME_H

#include <QSharedDataPointer>
#include <QQuickAttachedPropertyPropagator>
#include <qqmlintegration.h>

#include <SVSCraftQuick/SVSCraftQuickGlobal.h>

namespace SVS {

    class ColorChange;

    class ThemeAttachedType;

    class ThemePrivate;

    class SVSCRAFT_QUICK_EXPORT Theme : public QQuickAttachedPropertyPropagator {
        Q_OBJECT
        QML_ANONYMOUS

        Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)
        Q_PROPERTY(QColor warningColor READ warningColor WRITE setWarningColor NOTIFY warningColorChanged)
        Q_PROPERTY(QColor errorColor READ errorColor WRITE setErrorColor NOTIFY errorColorChanged)

        Q_PROPERTY(QColor buttonColor READ buttonColor WRITE setButtonColor NOTIFY buttonColorChanged)
        Q_PROPERTY(QColor textFieldColor READ textFieldColor WRITE setTextFieldColor NOTIFY textFieldColorChanged)
        Q_PROPERTY(QColor scrollBarColor READ scrollBarColor WRITE setScrollBarColor NOTIFY scrollBarColorChanged)
        Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)

        Q_PROPERTY(QColor backgroundPrimaryColor READ backgroundPrimaryColor WRITE setBackgroundPrimaryColor NOTIFY backgroundPrimaryColorChanged)
        Q_PROPERTY(QColor backgroundSecondaryColor READ backgroundSecondaryColor WRITE setBackgroundSecondaryColor NOTIFY backgroundSecondaryColorChanged)
        Q_PROPERTY(QColor backgroundTertiaryColor READ backgroundTertiaryColor WRITE setBackgroundTertiaryColor NOTIFY backgroundTertiaryColorChanged)
        Q_PROPERTY(QColor backgroundQuaternaryColor READ backgroundQuaternaryColor WRITE setBackgroundQuaternaryColor NOTIFY backgroundQuaternaryColorChanged)
        Q_PROPERTY(QColor splitterColor READ splitterColor WRITE setSplitterColor NOTIFY splitterColorChanged)

        Q_PROPERTY(QColor foregroundPrimaryColor READ foregroundPrimaryColor WRITE setForegroundPrimaryColor NOTIFY foregroundPrimaryColorChanged)
        Q_PROPERTY(QColor foregroundSecondaryColor READ foregroundSecondaryColor WRITE setForegroundSecondaryColor NOTIFY foregroundSecondaryColorChanged)
        Q_PROPERTY(QColor linkColor READ linkColor WRITE setLinkColor NOTIFY linkColorChanged)

        Q_PROPERTY(QColor navigationColor READ navigationColor WRITE setNavigationColor NOTIFY navigationColorChanged)
        Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor NOTIFY shadowColorChanged)
        Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor NOTIFY highlightColorChanged)

        Q_PROPERTY(ColorChange controlDisabledColorChange READ controlDisabledColorChange WRITE setControlDisabledColorChange NOTIFY controlDisabledColorChangeChanged)
        Q_PROPERTY(ColorChange foregroundDisabledColorChange READ foregroundDisabledColorChange WRITE setForegroundDisabledColorChange NOTIFY foregroundDisabledColorChangeChanged)
        Q_PROPERTY(ColorChange controlHoveredColorChange READ controlHoveredColorChange WRITE setControlHoveredColorChange NOTIFY controlHoveredColorChangeChanged)
        Q_PROPERTY(ColorChange foregroundHoveredColorChange READ foregroundHoveredColorChange WRITE setForegroundHoveredColorChange NOTIFY foregroundHoveredColorChangeChanged)
        Q_PROPERTY(ColorChange controlPressedColorChange READ controlPressedColorChange WRITE setControlPressedColorChange NOTIFY controlPressedColorChangeChanged)
        Q_PROPERTY(ColorChange foregroundPressedColorChange READ foregroundPressedColorChange WRITE setForegroundPressedColorChange NOTIFY foregroundPressedColorChangeChanged)
        Q_PROPERTY(ColorChange controlCheckedColorChange READ controlCheckedColorChange WRITE setControlCheckedColorChange NOTIFY controlCheckedColorChangeChanged)

        Q_PROPERTY(ColorChange annotationPopupTitleColorChange READ annotationPopupTitleColorChange WRITE setAnnotationPopupTitleColorChange NOTIFY annotationPopupTitleColorChangeChanged)
        Q_PROPERTY(ColorChange annotationPopupContentColorChange READ annotationPopupContentColorChange WRITE setAnnotationPopupContentColorChange NOTIFY annotationPopupContentColorChangeChanged)


        Q_PROPERTY(int colorAnimationDuration READ colorAnimationDuration WRITE setColorAnimationDuration NOTIFY colorAnimationDurationChanged)
        Q_PROPERTY(int visualEffectAnimationDuration READ visualEffectAnimationDuration WRITE setVisualEffectAnimationDuration NOTIFY visualEffectAnimationDurationChanged)

        Q_PROPERTY(int toolTipDelay READ toolTipDelay WRITE setToolTipDelay NOTIFY toolTipDelayChanged)
        Q_PROPERTY(int toolTipTimeout READ toolTipTimeout WRITE setToolTipTimeout NOTIFY toolTipTimeoutChanged)

    public:
        ~Theme() override;

        static Theme *get(QObject *item);
        static Theme *defaultTheme();

        QColor accentColor() const;
        void setAccentColor(const QColor &accentColor);

        QColor warningColor() const;
        void setWarningColor(const QColor &warningColor);

        QColor errorColor() const;
        void setErrorColor(const QColor &errorColor);

        QColor buttonColor() const;
        void setButtonColor(const QColor &buttonColor);

        QColor scrollBarColor() const;
        void setScrollBarColor(const QColor &scrollBarColor);

        QColor textFieldColor() const;
        void setTextFieldColor(const QColor &textFieldColor);

        QColor borderColor() const;
        void setBorderColor(const QColor &borderColor);

        QColor backgroundPrimaryColor() const;
        void setBackgroundPrimaryColor(const QColor &backgroundPrimaryColor);

        QColor backgroundSecondaryColor() const;
        void setBackgroundSecondaryColor(const QColor &backgroundSecondaryColor);

        QColor backgroundTertiaryColor() const;
        void setBackgroundTertiaryColor(const QColor &backgroundTertiaryColor);

        QColor backgroundQuaternaryColor() const;
        void setBackgroundQuaternaryColor(const QColor &backgroundQuaternaryColor);

        QColor splitterColor() const;
        void setSplitterColor(const QColor &splitterColor);

        QColor foregroundPrimaryColor() const;
        void setForegroundPrimaryColor(const QColor &foregroundPrimaryColor);

        QColor foregroundSecondaryColor() const;
        void setForegroundSecondaryColor(const QColor &foregroundSecondaryColor);

        QColor linkColor() const;
        void setLinkColor(const QColor &linkColor);

        QColor navigationColor() const;
        void setNavigationColor(const QColor &navigationColor);

        QColor shadowColor() const;
        void setShadowColor(const QColor &shadowColor);

        QColor highlightColor() const;
        void setHighlightColor(const QColor &highlightColor);

        ColorChange controlDisabledColorChange() const;
        void setControlDisabledColorChange(const ColorChange &controlDisabledColorChange);

        ColorChange foregroundDisabledColorChange() const;
        void setForegroundDisabledColorChange(const ColorChange &foregroundDisabledColorChange);

        ColorChange controlHoveredColorChange() const;
        void setControlHoveredColorChange(const ColorChange &controlHoveredColorChange);

        ColorChange foregroundHoveredColorChange() const;
        void setForegroundHoveredColorChange(const ColorChange &foregroundHoveredColorChange);

        ColorChange controlPressedColorChange() const;
        void setControlPressedColorChange(const ColorChange &controlPressedColorChange);

        ColorChange foregroundPressedColorChange() const;
        void setForegroundPressedColorChange(const ColorChange &foregroundPressedColorChange);

        ColorChange controlCheckedColorChange() const;
        void setControlCheckedColorChange(const ColorChange &controlCheckedColorChange);

        ColorChange annotationPopupTitleColorChange() const;
        void setAnnotationPopupTitleColorChange(const ColorChange &annotationPopupTitleColorChange);

        ColorChange annotationPopupContentColorChange() const;
        void setAnnotationPopupContentColorChange(const ColorChange &annotationPopupContentColorChange);

        int colorAnimationDuration() const;
        void setColorAnimationDuration(int colorAnimationDuration);

        int visualEffectAnimationDuration() const;
        void setVisualEffectAnimationDuration(int visualEffectAnimationDuration);

        int toolTipDelay() const;
        void setToolTipDelay(int toolTipDelay);

        int toolTipTimeout() const;
        void setToolTipTimeout(int toolTipTimeout);

        Q_INVOKABLE QColor controlColor(int controlType) const;
        Q_INVOKABLE QColor backgroundColor(int backgroundLevel) const;
        Q_INVOKABLE QColor foregroundColor(int foregroundLevel) const;
        Q_INVOKABLE QColor dividerStrokeColor(int dividerStroke) const;
        Q_INVOKABLE QColor dividerStrokeColor(int dividerStroke, const QColor &autoColor) const;

    signals:
        void accentColorChanged(const QColor &accentColor);
        void warningColorChanged(const QColor &warningColor);
        void errorColorChanged(const QColor &errorColor);
        void buttonColorChanged(const QColor &buttonColor);
        void textFieldColorChanged(const QColor &textFieldColor);
        void scrollBarColorChanged(const QColor &scrollBarColor);
        void borderColorChanged(const QColor &borderColor);
        void backgroundPrimaryColorChanged(const QColor &backgroundPrimaryColor);
        void backgroundSecondaryColorChanged(const QColor &backgroundSecondaryColor);
        void backgroundTertiaryColorChanged(const QColor &backgroundTertiaryColor);
        void backgroundQuaternaryColorChanged(const QColor &backgroundQuaternaryColor);
        void splitterColorChanged(const QColor &splitterColor);
        void foregroundPrimaryColorChanged(const QColor &foregroundPrimaryColor);
        void foregroundSecondaryColorChanged(const QColor &foregroundSecondaryColor);
        void linkColorChanged(const QColor &linkColor);
        void navigationColorChanged(const QColor &navigationColor);
        void shadowColorChanged(const QColor &shadowColor);
        void highlightColorChanged(const QColor &highlightColor);
        void controlDisabledColorChangeChanged(const SVS::ColorChange &controlDisabledColorChange);
        void foregroundDisabledColorChangeChanged(const SVS::ColorChange &foregroundDisabledColorChange);
        void controlHoveredColorChangeChanged(const SVS::ColorChange &controlHoveredColorChange);
        void foregroundHoveredColorChangeChanged(const SVS::ColorChange &foregroundHoveredColorChange);
        void controlPressedColorChangeChanged(const SVS::ColorChange &controlPressedColorChange);
        void foregroundPressedColorChangeChanged(const SVS::ColorChange &foregroundPressedColorChange);
        void controlCheckedColorChangeChanged(const SVS::ColorChange &controlCheckedColorChange);
        void annotationPopupTitleColorChangeChanged(const SVS::ColorChange &annotationPopupTitleColorChange);
        void annotationPopupContentColorChangeChanged(const SVS::ColorChange &annotationPopupContentColorChange);
        void colorAnimationDurationChanged(int colorAnimationDuration);
        void visualEffectAnimationDurationChanged(int visualEffectAnimationDuration);
        void toolTipDelayChanged(int toolTipDelay);
        void toolTipTimeoutChanged(int toolTipTimeout);

    protected:
        void attachedParentChange(QQuickAttachedPropertyPropagator *newParent, QQuickAttachedPropertyPropagator *oldParent) override;

    private:
        friend class ThemeAttachedType;
        friend class ThemePrivate;
        explicit Theme(QObject *parent = nullptr);
        explicit Theme(ThemePrivate *d);
        QSharedDataPointer<ThemePrivate> d;

    };

}

#endif // SVSCRAFT_THEME_H
