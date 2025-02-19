#ifndef STATUSTEXT_P_P_H
#define STATUSTEXT_P_P_H

#include <SVSCraftQuick/private/statustext_p.h>

namespace SVS {

    class StatusTextAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(StatusText)
        QML_ATTACHED(StatusText)
    public:
        static StatusText *qmlAttachedProperties(QObject *object);
    };

    class StatusTextPrivate {
        Q_DECLARE_PUBLIC(StatusText)
    public:
        StatusText *q_ptr;

        QString text;
        QString defaultText;
    };
}

#endif //STATUSTEXT_P_P_H
