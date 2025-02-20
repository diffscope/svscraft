#ifndef THEME_H
#define THEME_H

#include <QColor>
#include <QObject>
#include <qqmlintegration.h>

#include <SVSCraftQuick/svscraftquickglobal.h>

namespace SVS {

    class ColorChange;

    class ThemePrivate;

    class SVSCRAFT_QUICK_EXPORT Theme : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(Theme)

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

        Q_PROPERTY(ColorChange controlDisabledColorChange READ controlDisabledColorChange WRITE setControlDisabledColorChange NOTIFY controlDisabledColorChangeChanged)
        Q_PROPERTY(ColorChange foregroundDisabledColorChange READ foregroundDisabledColorChange WRITE setForegroundDisabledColorChange NOTIFY foregroundDisabledColorChangeChanged)
        Q_PROPERTY(ColorChange controlHoveredColorChange READ controlHoveredColorChange WRITE setControlHoveredColorChange NOTIFY controlHoveredColorChangeChanged)
        Q_PROPERTY(ColorChange foregroundHoveredColorChange READ foregroundHoveredColorChange WRITE setForegroundHoveredColorChange NOTIFY foregroundHoveredColorChangeChanged)
        Q_PROPERTY(ColorChange controlPressedColorChange READ controlPressedColorChange WRITE setControlPressedColorChange NOTIFY controlPressedColorChangeChanged)
        Q_PROPERTY(ColorChange foregroundPressedColorChange READ foregroundPressedColorChange WRITE setForegroundPressedColorChange NOTIFY foregroundPressedColorChangeChanged)
        Q_PROPERTY(ColorChange controlCheckedColorChange READ controlCheckedColorChange WRITE setControlCheckedColorChange NOTIFY controlCheckedColorChangeChanged)

        Q_PROPERTY(int colorAnimationDuration READ colorAnimationDuration WRITE setColorAnimationDuration NOTIFY colorAnimationDurationChanged)
        Q_PROPERTY(int visualEffectAnimationDuration READ visualEffectAnimationDuration WRITE setVisualEffectAnimationDuration NOTIFY visualEffectAnimationDurationChanged)

        Q_PROPERTY(int toolTipDelay READ toolTipDelay WRITE setToolTipDelay NOTIFY toolTipDelayChanged)
        Q_PROPERTY(int toolTipTimeout READ toolTipTimeout WRITE setToolTipTimeout NOTIFY toolTipTimeoutChanged)

    public:
        ~Theme() override;

        static Theme *instance();

        enum ControlType {
            CT_Normal, CT_Accent, CT_Warning, CT_Error,
        };
        Q_ENUM(ControlType)
        Q_INVOKABLE QColor controlColor(SVS::Theme::ControlType controlType) const;

        enum BackgroundLevel {
            BL_Primary, BL_Secondary, BL_Tertiary, BL_Quaternary,
        };
        Q_ENUM(BackgroundLevel)
        Q_INVOKABLE QColor backgroundColor(SVS::Theme::BackgroundLevel backgroundLevel) const;

        enum ForegroundLevel {
            FL_Primary, FL_Secondary,
        };
        Q_ENUM(ForegroundLevel)
        Q_INVOKABLE QColor foregroundColor(SVS::Theme::ForegroundLevel foregroundLevel) const;

        enum TabIndicator {
            TI_Fill, TI_Top, TI_Bottom, TI_Left, TI_Right,
        };
        Q_ENUM(TabIndicator)

        enum DividerStroke {
            DS_Auto, DS_None, DS_Border, DS_Splitter,
        };
        Q_ENUM(DividerStroke)
        Q_INVOKABLE QColor dividerStrokeColor(SVS::Theme::DividerStroke dividerStroke, const QColor &autoColor = Qt::transparent) const;

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

        int colorAnimationDuration() const;
        void setColorAnimationDuration(int colorAnimationDuration);

        int visualEffectAnimationDuration() const;
        void setVisualEffectAnimationDuration(int visualEffectAnimationDuration);

        int toolTipDelay() const;
        void setToolTipDelay(int toolTipDelay);

        int toolTipTimeout() const;
        void setToolTipTimeout(int toolTipTimeout);

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
        void controlDisabledColorChangeChanged(const SVS::ColorChange &controlDisabledColorChange);
        void foregroundDisabledColorChangeChanged(const SVS::ColorChange &foregroundDisabledColorChange);
        void controlHoveredColorChangeChanged(const SVS::ColorChange &controlHoveredColorChange);
        void foregroundHoveredColorChangeChanged(const SVS::ColorChange &foregroundHoveredColorChange);
        void controlPressedColorChangeChanged(const SVS::ColorChange &controlPressedColorChange);
        void foregroundPressedColorChangeChanged(const SVS::ColorChange &foregroundPressedColorChange);
        void controlCheckedColorChangeChanged(const SVS::ColorChange &controlCheckedColorChange);
        void colorAnimationDurationChanged(int colorAnimationDuration);
        void visualEffectAnimationDurationChanged(int visualEffectAnimationDuration);
        void toolTipDelayChanged(int toolTipDelay);
        void toolTipTimeoutChanged(int toolTipTimeout);

    private:
        explicit Theme(QObject *parent = nullptr);
        QScopedPointer<ThemePrivate> d_ptr;

    };

}

#endif //THEME_H
