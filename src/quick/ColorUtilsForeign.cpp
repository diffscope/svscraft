#include "ColorUtilsForeign_p.h"

#include <QColor>

#include <SVSCraftGui/ColorUtils.h>

namespace SVS {
    ColorUtilsForeign::ColorUtilsForeign(QObject *parent) : QObject(parent) {
    }
    ColorUtilsForeign::~ColorUtilsForeign() = default;
    QColor ColorUtilsForeign::selectHighestContrastColor(const QColor &baseColor, const QList<QColor> &availableColors) {
        return ColorUtils::selectHighestContrastColor(baseColor, availableColors);
    }
}

#include "moc_ColorUtilsForeign_p.cpp"