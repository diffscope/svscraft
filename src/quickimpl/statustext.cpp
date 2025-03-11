#include "statustext_p.h"
#include "statustext_p_p.h"

#include <QQuickWindow>
#include <QQmlInfo>

namespace SVS {
    StatusText *StatusTextAttachedType::qmlAttachedProperties(QObject *object) {
        auto *window = qobject_cast<QQuickWindow *>(object);
        if (!window)
            qmlWarning(object) << "StatusText should be attached to a Window";
        return new StatusText(object);
    }

    StatusText::StatusText(QObject *parent) : QObject(parent), d_ptr(new StatusTextPrivate) {
        Q_D(StatusText);
        d->q_ptr = this;
    }
    StatusText::~StatusText() = default;
    QString StatusText::text() const {
        Q_D(const StatusText);
        return d->text;
    }
    void StatusText::setText(const QString &text) {
        Q_D(StatusText);
        if (d->text != text) {
            d->text = text;
            emit textChanged();
            emit displayTextChanged();
        }
    }
    void StatusText::setContextObject(QObject *object) {
        Q_D(StatusText);
        d->contextObject = object;
    }
    QObject *StatusText::contextObject() const {
        Q_D(const StatusText);
        return d->contextObject;
    }
    QString StatusText::defaultText() const {
        Q_D(const StatusText);
        return d->defaultText;
    }
    void StatusText::setDefaultText(const QString &defaultText) {
        Q_D(StatusText);
        if (d->defaultText != defaultText) {
            d->defaultText = defaultText;
            emit defaultTextChanged();
            if (d->text.isEmpty())
                emit displayTextChanged();
        }
    }
    QString StatusText::displayText() const {
        Q_D(const StatusText);
        return d->text.isEmpty() ? d->defaultText : d->text;
    }
}