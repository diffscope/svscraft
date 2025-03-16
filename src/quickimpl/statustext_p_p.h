#ifndef SVSCRAFT_STATUSTEXT_P_P_H
#define SVSCRAFT_STATUSTEXT_P_P_H

#include <SVSCraftQuickImpl/private/statustext_p.h>

#include <QPointer>

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
        QPointer<QObject> contextObject;
        QString defaultText;
    };
}

#endif //STATUSTEXT_P_P_H
