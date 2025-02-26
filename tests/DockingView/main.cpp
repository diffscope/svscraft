#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QColor>
#include <QSurfaceFormat>
#include <QQuickStyle>

#include <SVSCraftQuick/theme.h>
#include <SVSCraftGui/colorchange.h>

using namespace SVS;

int main(int argc, char *argv[]) {
    QGuiApplication a(argc, argv);

    auto sf = QSurfaceFormat::defaultFormat();
    sf.setSamples(8);
    QSurfaceFormat::setDefaultFormat(sf);

    Theme::defaultTheme()->setAccentColor(0x5566ff);
    Theme::defaultTheme()->setWarningColor(0xeeaa66);
    Theme::defaultTheme()->setErrorColor(0xcc4455);
    Theme::defaultTheme()->setButtonColor(0x333437);
    Theme::defaultTheme()->setTextFieldColor(0x27282b);
    Theme::defaultTheme()->setScrollBarColor(QColor::fromRgba(0x7f7f7f7f));
    Theme::defaultTheme()->setBorderColor(0x4a4b4c);

    Theme::defaultTheme()->setBackgroundPrimaryColor(0x212124);
    Theme::defaultTheme()->setBackgroundSecondaryColor(0x232427);
    Theme::defaultTheme()->setBackgroundTertiaryColor(0x252629);
    Theme::defaultTheme()->setBackgroundQuaternaryColor(0x313235);
    Theme::defaultTheme()->setSplitterColor(0x121315);

    Theme::defaultTheme()->setForegroundPrimaryColor(0xdadada);
    Theme::defaultTheme()->setForegroundSecondaryColor(QColor::fromRgba(0xa0dadada));

    Theme::defaultTheme()->setLinkColor(0x5566ff);

    Theme::defaultTheme()->setNavigationColor(0xffffff);
    Theme::defaultTheme()->setShadowColor(0x101113);
    Theme::defaultTheme()->setHighlightColor(0xb28300);

    Theme::defaultTheme()->setControlDisabledColorChange({QColor::fromRgba(0x33000000)});
    Theme::defaultTheme()->setForegroundDisabledColorChange({{}, 0.5});
    Theme::defaultTheme()->setControlHoveredColorChange({QColor::fromRgba(0x1affffff)});
    Theme::defaultTheme()->setForegroundHoveredColorChange({});
    Theme::defaultTheme()->setControlPressedColorChange({});
    Theme::defaultTheme()->setForegroundPressedColorChange({{}, 0.8});
    Theme::defaultTheme()->setControlCheckedColorChange({QColor::fromRgba(0x1affffff)});
    Theme::defaultTheme()->setColorAnimationDuration(250);
    Theme::defaultTheme()->setVisualEffectAnimationDuration(250);

    Theme::defaultTheme()->setToolTipDelay(1000);

    QQuickStyle::setStyle("SVSCraft.UIComponents");
    QQuickStyle::setFallbackStyle("Basic");

    QQmlApplicationEngine engine(":/qt/qml/SVSCraft/Test/DockingView/main.qml");

    return a.exec();
}
