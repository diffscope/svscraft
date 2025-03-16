#ifndef SVSCRAFT_THEMEDITEM_P_P_H
#define SVSCRAFT_THEMEDITEM_P_P_H

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
        SVSQmlNamespace::ControlType controlType = SVSQmlNamespace::CT_Normal;
        SVSQmlNamespace::BackgroundLevel backgroundLevel = SVSQmlNamespace::BL_Primary;
        SVSQmlNamespace::ForegroundLevel foregroundLevel = SVSQmlNamespace::FL_Primary;
        SVSQmlNamespace::TabIndicator tabIndicator = SVSQmlNamespace::TI_Fill;
        SVSQmlNamespace::DividerStroke dividerStroke = SVSQmlNamespace::DS_Auto;
        double trackStart = 0.0;
        bool foldable = false;
        bool folded = false;
        bool flat = false;
        bool splitHandleVisible = true;
        bool splitHandleEnabled = true;

    };
}

#endif //THEMEDITEM_P_P_H