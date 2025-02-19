#ifndef THEMEDITEM_P_P_H
#define THEMEDITEM_P_P_H

#include <SVSCraftQuick/private/themeditem_p.h>

namespace SVS {

    class ThemedItemAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(ThemedItem)
        QML_ATTACHED(ThemedItem)
    public:
        static ThemedItem *qmlAttachedProperties(QObject *object);
    };

    class ThemedItemPrivate {
        Q_DECLARE_PUBLIC(ThemedItem)
    public:
        ThemedItem *q_ptr;
        Theme::ControlType controlType = Theme::CT_Normal;
        Theme::BackgroundLevel backgroundLevel = Theme::BL_Primary;
        Theme::TabIndicator tabIndicator = Theme::TI_Fill;
        Theme::DividerStroke dividerStroke = Theme::DS_Auto;
        bool foldable = false;
        bool folded = false;
        bool flat = false;
        double trackStart = 0.0;
    };
}

#endif //THEMEDITEM_P_P_H