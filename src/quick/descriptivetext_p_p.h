#ifndef SVSCRAFT_DESCRIPTIVETEXT_P_P_H
#define SVSCRAFT_DESCRIPTIVETEXT_P_P_H

#include <SVSCraftQuick/private/descriptivetext_p.h>

#include <QPointer>

#include <SVSCraftQuickImpl/private/statustext_p.h>

namespace SVS {
    class DescriptiveTextAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(DescriptiveText)
        QML_ATTACHED(DescriptiveText)
    public:
        static DescriptiveText *qmlAttachedProperties(QObject *object);
    };;

    class DescriptiveTextPrivate {
        Q_DECLARE_PUBLIC(DescriptiveText)
    public:
        DescriptiveText *q_ptr;
        QQuickItem *item;
        bool activated = false;
        QString toolTip;
        QString statusTip;

        QPointer<StatusText> statusText;
    };
}

#endif //DESCRIPTIVETEXT_P_P_H
