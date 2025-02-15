#ifndef THEMEATTACHEDTYPE_P_H
#define THEMEATTACHEDTYPE_P_H

#include <SVSCraftQuick/themeattachedtype.h>

namespace SVS {
    class ThemeAttachedTypePrivate {
        Q_DECLARE_PUBLIC(ThemeAttachedType)
    public:
        ThemeAttachedType *q_ptr;
        Theme::ControlType controlType = Theme::CT_Normal;
        Theme::BackgroundLevel backgroundLevel = Theme::BL_Primary;
        bool foldable = false;
        bool folded = false;
        double trackStart = 0.0;
    };
}

#endif //THEMEATTACHEDTYPE_P_H