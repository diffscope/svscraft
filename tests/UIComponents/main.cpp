#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QColor>
#include <QSurfaceFormat>
#include <QQuickStyle>

#include <SVSCraftQuick/theme.h>
#include <SVSCraftQuick/colorchange.h>

using namespace SVS;

int main(int argc, char *argv[]) {
    QGuiApplication a(argc, argv);

    auto sf = QSurfaceFormat::defaultFormat();
    sf.setSamples(8);
    QSurfaceFormat::setDefaultFormat(sf);

    Theme::instance()->setAccentColor(0x5566ff);
    Theme::instance()->setWarningColor(0xeeaa66);
    Theme::instance()->setErrorColor(0xcc4455);
    Theme::instance()->setButtonColor(0x292a2b);
    Theme::instance()->setBorderColor(0x4a4b4c);
    Theme::instance()->setForegroundPrimaryColor(0xdadada);
    Theme::instance()->setControlDisabledColorChange({1, 0.75});
    Theme::instance()->setForegroundDisabledColorChange({1, 0.75, 0.75});
    Theme::instance()->setControlHoveredColorChange({0.9, 1.1});
    Theme::instance()->setForegroundHoveredColorChange({});
    Theme::instance()->setControlPressedColorChange({});
    Theme::instance()->setForegroundPressedColorChange({1, 1, 0.8});
    Theme::instance()->setColorAnimationDuration(250);

    QQuickStyle::setStyle("SVSCraft.UIComponents");
    QQuickStyle::setFallbackStyle("Basic");

    QQmlApplicationEngine engine(":/qt/qml/SVSCraft/Test/UIComponents/main.qml");

    return a.exec();
}
