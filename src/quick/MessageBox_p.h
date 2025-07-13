#ifndef SVSCRAFT_MESSAGEBOX_P_H
#define SVSCRAFT_MESSAGEBOX_P_H

#include <SVSCraftQuick/MessageBox.h>

class QEventLoop;

namespace SVS {

    class MessageBoxAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(MessageBox)
        QML_ATTACHED(MessageBox)
    public:
        static MessageBox *qmlAttachedProperties(QObject *object);
    };

    class MessageBoxDialogDoneListener : public QObject {
        Q_OBJECT
    public:
        inline explicit MessageBoxDialogDoneListener(QEventLoop *eventLoop) : eventLoop(eventLoop) {
        }

    public slots:
        void done(const QVariant &id) const;

    private:
        QEventLoop *eventLoop;
    };

}

#endif //SVSCRAFT_MESSAGEBOX_P_H
