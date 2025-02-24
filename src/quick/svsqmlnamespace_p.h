#ifndef SVSQMLNAMESPACE_P_H
#define SVSQMLNAMESPACE_P_H

#include <QObject>
#include <qqmlintegration.h>

namespace SVS {

    class SVSQmlNamespace : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(SVS)
        QML_SINGLETON
    public:
        enum ControlType {
            CT_Normal, CT_Accent, CT_Warning, CT_Error,
        };
        Q_ENUM(ControlType)

        enum BackgroundLevel {
            BL_Primary, BL_Secondary, BL_Tertiary, BL_Quaternary,
        };
        Q_ENUM(BackgroundLevel)

        enum ForegroundLevel {
            FL_Primary, FL_Secondary,
        };
        Q_ENUM(ForegroundLevel)

        enum TabIndicator {
            TI_Fill, TI_Top, TI_Bottom, TI_Left, TI_Right,
        };
        Q_ENUM(TabIndicator)

        enum DividerStroke {
            DS_Auto, DS_None, DS_Border, DS_Splitter,
        };
        Q_ENUM(DividerStroke)
    };

}

#endif //SVSQMLNAMESPACE_P_H
