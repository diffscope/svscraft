#ifndef THEMEATTACHEDTYPE_P_H
#define THEMEATTACHEDTYPE_P_H

#include <SVSCraftQuick/themeattachedtype.h>

namespace SVS {
    class ThemeAttachedTypePrivate {
        Q_DECLARE_PUBLIC(ThemeAttachedType)
    public:
        ThemeAttachedType *q_ptr;
        Theme::ControlType controlType = Theme::CT_Normal;
    };
}

#endif //THEMEATTACHEDTYPE_P_H