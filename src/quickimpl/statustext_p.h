#ifndef STATUSTEXT_P_H
#define STATUSTEXT_P_H

#include <QObject>
#include <qqmlintegration.h>

namespace SVS {

    class StatusTextAttachedType;

    class StatusTextPrivate;

    class StatusText : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(StatusText)
        QML_ANONYMOUS

    public:
        ~StatusText() override;

        QString text() const;
        void setText(const QString &text);

        void setContextObject(QObject *object);
        QObject *contextObject() const;

        QString defaultText() const;
        void setDefaultText(const QString &defaultText);

        QString displayText() const;

    signals:
        void textChanged();
        void defaultTextChanged();
        void displayTextChanged();

    private:
        friend class StatusTextAttachedType;
        explicit StatusText(QObject *parent = nullptr);

        QScopedPointer<StatusTextPrivate> d_ptr;

    };

}

#endif //STATUSTEXT_P_H
