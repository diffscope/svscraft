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
        auto pixmap = FluentSystemIcons::getIcon(iconId, direction, sz, style, qMax(size->width(), size->height()));
        return pixmap;
    }

    void FluentSystemIconsImageProvider::addToEngine(QQmlEngine *engine) {
        engine->addImageProvider("fluent-system-icons", new FluentSystemIconsImageProvider);
    }

}