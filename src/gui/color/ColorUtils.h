#ifndef SVSCRAFT_COLORUTILS_H
#define SVSCRAFT_COLORUTILS_H

#include <SVSCraftGui/SVSCraftGuiGlobal.h>

class QColor;

namespace SVS {

    class SVSCRAFT_GUI_EXPORT ColorUtils {
    public:
        static QColor selectHighestContrastColor(const QColor &baseColor, const QList<QColor> &availableColors);

    };

}

#endif //SVSCRAFT_COLORUTILS_H
