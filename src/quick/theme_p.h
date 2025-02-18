#ifndef THEME_P_H
#define THEME_P_H

#include <SVSCraftQuick/theme.h>

#include <QColor>

#include <SVSCraftGui/colorchange.h>

#include <SVSCraftQuick/themeattachedtype.h>

class QQmlEngine;
class QJSEngine;

namespace SVS {

    class ThemedItem : public QObject {
        Q_OBJECT
        QML_ELEMENT
        QML_ATTACHED(ThemeAttachedType)
    public:
        static ThemeAttachedType *qmlAttachedProperties(QObject *object);
    };

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
        ColorChange controlDisabledColorChange;
        ColorChange foregroundDisabledColorChange;
        ColorChange controlHoveredColorChange;
        ColorChange foregroundHoveredColorChange;
        ColorChange controlPressedColorChange;
        ColorChange foregroundPressedColorChange;
        int colorAnimationDuration{};
        int visualEffectAnimationDuration{};

    };
}


#endif // THEME_P_H
