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

        Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
        Q_PROPERTY(QString defaultText READ defaultText WRITE setDefaultText NOTIFY defaultTextChanged)
        Q_PROPERTY(QString displayText READ displayText NOTIFY displayTextChanged)

    public:
        ~StatusText() override;

        QString text() const;
        void setText(const QString &text);

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
