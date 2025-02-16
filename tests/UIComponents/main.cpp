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

    Theme::instance()->setAccentColor(0x5566ff);
    Theme::instance()->setWarningColor(0xeeaa66);
    Theme::instance()->setErrorColor(0xcc4455);
    Theme::instance()->setButtonColor(0x333437);
    Theme::instance()->setTextFieldColor(0x27282b);
    Theme::instance()->setScrollBarColor(QColor::fromRgba(0x7f7f7f7f));
    Theme::instance()->setBorderColor(0x4a4b4c);

    Theme::instance()->setBackgroundPrimaryColor(0x212124);
    Theme::instance()->setBackgroundSecondaryColor(0x232427);
    Theme::instance()->setBackgroundTertiaryColor(0x252629);
    Theme::instance()->setBackgroundQuaternaryColor(0x313235);

    Theme::instance()->setForegroundPrimaryColor(0xdadada);
    Theme::instance()->setForegroundSecondaryColor(0x888888);

    Theme::instance()->setNavigationColor(0xffffff);

    Theme::instance()->setControlDisabledColorChange({QColor::fromRgba(0x33000000)});
    Theme::instance()->setForegroundDisabledColorChange({{}, 0.5});
    Theme::instance()->setControlHoveredColorChange({QColor::fromRgba(0x1affffff)});
    Theme::instance()->setForegroundHoveredColorChange({});
    Theme::instance()->setControlPressedColorChange({});
    Theme::instance()->setForegroundPressedColorChange({{}, 0.8});
    Theme::instance()->setColorAnimationDuration(250);
    Theme::instance()->setVisualEffectAnimationDuration(250);

    QQuickStyle::setStyle("SVSCraft.UIComponents");
    QQuickStyle::setFallbackStyle("Basic");

    QQmlApplicationEngine engine(":/qt/qml/SVSCraft/Test/UIComponents/main.qml");

    return a.exec();
}
