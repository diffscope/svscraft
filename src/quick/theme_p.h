#ifndef THEME_P_H
#define THEME_P_H

#include <SVSCraftQuick/theme.h>

#include <QColor>

#include <SVSCraftGui/colorchange.h>

class QQmlEngine;
class QJSEngine;

namespace SVS {

    class ThemeForeign {
        Q_GADGET
        QML_NAMED_ELEMENT(Theme)
        QML_FOREIGN(Theme)
        QML_SINGLETON

    public:
        static Theme *create(QQmlEngine *engine, QJSEngine *);
    };
    class ThemePrivate {
        Q_DECLARE_PUBLIC(Theme)
    public:
        Theme *q_ptr;

        static Theme instance;

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
        int colorAnimationDuration{};
        int visualEffectAnimationDuration{};
        int toolTipDelay{};
        int toolTipTimeout = -1;

    };
}


#endif // THEME_P_H
