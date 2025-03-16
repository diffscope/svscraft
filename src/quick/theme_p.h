#ifndef SVSCRAFT_THEME_P_H
#define SVSCRAFT_THEME_P_H

#include <SVSCraftQuick/theme.h>

#include <QColor>
#include <QSharedData>

#include <SVSCraftGui/colorchange.h>

class QQmlEngine;
class QJSEngine;

namespace SVS {

    class ThemeAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(Theme)
        QML_ATTACHED(Theme)
    public:
        static Theme *qmlAttachedProperties(QObject *object);
    };
    class ThemePrivate : public QSharedData {
    public:
        static void inherit(Theme *object, Theme *parent);
        static Theme defaultTheme;

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
        ColorChange annotationPopupTitleColorChange;
        ColorChange annotationPopupContentColorChange;
        int colorAnimationDuration{};
        int visualEffectAnimationDuration{};
        int toolTipDelay{};
        int toolTipTimeout = -1;

    };
}


#endif // SVSCRAFT_THEME_P_H
