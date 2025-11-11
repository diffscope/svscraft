#ifndef SVSCRAFT_FLUENTSYSTEMICONS_H
#define SVSCRAFT_FLUENTSYSTEMICONS_H

#include <SVSCraftFluentSystemIcons/SVSCraftFluentSystemIconsGlobal.h>

class QPixmap;

namespace SVS {

    class SVSCRAFT_FLUENT_SYSTEM_ICONS_EXPORT FluentSystemIcons {
    public:
        enum Direction {
            Auto,
            Ltr,
            Rtl
        };

        enum Style {
            Filled,
            Regular,
        };

        static QPixmap getIcon(const QString &name, Direction direction, int size, Style style);
    };

}

#endif //SVSCRAFT_FLUENTSYSTEMICONS_H
