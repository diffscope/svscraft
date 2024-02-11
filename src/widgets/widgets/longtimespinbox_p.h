#ifndef LONGTIMESPINBOX_P_H
#define LONGTIMESPINBOX_P_H

#include <SVSCraftWidgets/longtimespinbox.h>

namespace SVS {

    class LongTimeSpinBoxPrivate {
        Q_DECLARE_PUBLIC(LongTimeSpinBox);
    public:
        LongTimeSpinBox *q_ptr;

        int minuteWidth = 1;
        int secondWidth = 2;
        int msecWidth = 3;
    };

} // SVS

#endif // LONGTIMESPINBOX_P_H
