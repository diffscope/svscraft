#include "FluentSystemIconsImageProvider.h"

#include <QQmlEngine>
#include <QUrlQuery>

#include <SVSCraftFluentSystemIcons/FluentSystemIcons.h>

namespace SVS {

    FluentSystemIconsImageProvider::FluentSystemIconsImageProvider() : QQuickImageProvider(Pixmap) {
    }

    FluentSystemIconsImageProvider::~FluentSystemIconsImageProvider() = default;

    QPixmap FluentSystemIconsImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {
        if (!requestedSize.isValid()) {
            size->setWidth(64);
            size->setHeight(64);
        } else {
            *size = requestedSize;
        }
        QUrl url(id);
        auto iconId = url.path();
        auto query = QUrlQuery(url);
        auto directionStr = query.queryItemValue("direction");
        FluentSystemIcons::Direction direction;
        if (directionStr == "rtl") {
            direction = FluentSystemIcons::Rtl;
        } else if (directionStr == "ltr") {
            direction = FluentSystemIcons::Ltr;
        } else {
            direction = FluentSystemIcons::Auto;
        }
        auto sizeStr = query.queryItemValue("size");
        int sz = sizeStr.toInt();
        if (!sz) {
            sz = 16;
        }
        auto styleStr = query.queryItemValue("style");
        FluentSystemIcons::Style style;
        if (styleStr == "regular") {
            style = FluentSystemIcons::Regular;
        } else {
            style = FluentSystemIcons::Filled;
        }
        auto mirrorStr = query.queryItemValue("mirror");
        FluentSystemIcons::Mirror mirror = FluentSystemIcons::NoMirror;
        if (mirrorStr == "h") {
            mirror = FluentSystemIcons::Horizontal;
        } else if (mirrorStr == "v") {
            mirror = FluentSystemIcons::Vertical;
        }
        auto rotateStr = query.queryItemValue("rotate");
        FluentSystemIcons::Rotate rotate = FluentSystemIcons::NoRotate;
        if (rotateStr == "1") {
            rotate = FluentSystemIcons::Rotate90;
        } else if (rotateStr == "2") {
            rotate = FluentSystemIcons::Rotate180;
        } else if (rotateStr == "3") {
            rotate = FluentSystemIcons::Rotate270;
        }
        auto pixmap = FluentSystemIcons::getIcon(iconId, direction, sz, style, mirror, rotate, qMax(size->width(), size->height()));
        return pixmap;
    }

    void FluentSystemIconsImageProvider::addToEngine(QQmlEngine *engine) {
        engine->addImageProvider("fluent-system-icons", new FluentSystemIconsImageProvider);
    }

}