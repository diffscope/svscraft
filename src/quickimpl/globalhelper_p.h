#ifndef GLOBALHELPER_P_H
#define GLOBALHELPER_P_H

#include <QObject>
#include <qqmlintegration.h>

namespace SVS {

    class GlobalHelper : public QObject {
        Q_OBJECT
        QML_ELEMENT
        QML_SINGLETON
    public:
        explicit GlobalHelper(QObject *parent = nullptr);
        ~GlobalHelper() override;

        Q_INVOKABLE static QPoint cursorPos();
    };
}

#endif //GLOBALHELPER_P_H
