#ifndef MUSICTIMESPINBOX_P_H
#define MUSICTIMESPINBOX_P_H

#include <SVSCraftWidgets/musictimespinbox.h>

namespace SVS {
    class MusicTimeSpinBoxPrivate {
        Q_DECLARE_PUBLIC(MusicTimeSpinBox)
    public:
        MusicTimeSpinBox *q_ptr;
        MusicTimeline *timeline;
        int measureWidth = 1;
        int beatWidth = 1;
        int tickWidth = 3;
    };
}

#endif // MUSICTIMESPINBOX_P_H
