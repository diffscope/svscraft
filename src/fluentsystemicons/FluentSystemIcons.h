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

        enum Mirror {
            NoMirror,
            Horizontal,
            Vertical
        };

        enum Rotate {
            NoRotate,
            Rotate90,
            Rotate180,
            Rotate270
        };

        static QPixmap getIcon(const QString &name, Direction direction, int size, Style style, Mirror mirror = NoMirror, Rotate rotate = NoRotate, int pixmapSize = -1);
    };

}

#endif //SVSCRAFT_FLUENTSYSTEMICONS_H
