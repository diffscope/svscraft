#include "editlabel.h"

#include <QLabel>
#include <QLineEdit>
#include <QAbstractSpinBox>

#include <QMouseEvent>

#include <QMWidgets/qmdecoratorv2.h>

namespace SVS {

    class EditLabelPrivate : public QObject {
        Q_DECLARE_PUBLIC(EditLabel)
    public:
        EditLabel *q_ptr;
        QString prefix;
        QString suffix;

        QLabel *label;
        QWidget *editWidget = nullptr;

        EditLabel::Triggers triggers = EditLabel::Triggers(EditLabel::MousePress | EditLabel::MouseDblClick | EditLabel::KeyboardFocusIn);

        void addEditWidgetToStackedWidget(QWidget *w);
        void postAddLineEdit(QLineEdit *lineEdit) const;
        void postAddSpinBox(QAbstractSpinBox *spinBox) const;
        void updateLabelText();
        QString editWidgetText() const;
        void exitEdit();

        void reloadStrings() const;
    };

    void EditLabelPrivate::addEditWidgetToStackedWidget(QWidget *w) {
        Q_Q(EditLabel);
        bool oldEditing = q->editing();
        if (editWidget) {
            disconnect(editWidget, nullptr, this, nullptr);
            editWidget->removeEventFilter(this);
            q->QStackedWidget::removeWidget(editWidget);
        }
        if (w)
            q->QStackedWidget::addWidget(w);
        editWidget = w;
        q->QStackedWidget::setCurrentWidget(label);
        if (qobject_cast<QLineEdit *>(w))
            postAddLineEdit(qobject_cast<QLineEdit *>(w));
        else if (qobject_cast<QAbstractSpinBox *>(w))
            postAddSpinBox(qobject_cast<QAbstractSpinBox *>(w));
        w->installEventFilter(q);
        updateLabelText();
        if (oldEditing)
            Q_EMIT q->editingChanged(false);
    }

    void EditLabelPrivate::postAddLineEdit(QLineEdit *lineEdit) const {
        if (!lineEdit)
            return;
        connect(lineEdit, &QLineEdit::editingFinished, this, &EditLabelPrivate::exitEdit);
    }

    void EditLabelPrivate::postAddSpinBox(QAbstractSpinBox *spinBox) const {
        if (!spinBox)
            return;
        connect(spinBox, &QAbstractSpinBox::editingFinished, this, &EditLabelPrivate::exitEdit);
    }

    void EditLabelPrivate::updateLabelText() {
        Q_Q(EditLabel);
        auto text = prefix + q->editToLabel(editWidgetText()) + suffix;
        if (label->text() != text) {
            label->setText(text);
            Q_EMIT q->textChanged(text);
        }
    }

    QString EditLabelPrivate::editWidgetText() const {
        if (qobject_cast<QLineEdit *>(editWidget))
            return qobject_cast<QLineEdit *>(editWidget)->text();
        else if (qobject_cast<QAbstractSpinBox *>(editWidget))
            return qobject_cast<QAbstractSpinBox *>(editWidget)->text();
        else
            return {};
    }

    void EditLabelPrivate::exitEdit() {
        Q_Q(EditLabel);
        bool oldEditing = q->editing();
        updateLabelText();
        q->QStackedWidget::setCurrentWidget(label);
        if (oldEditing)
            Q_EMIT q->editingChanged(false);
    }

    void EditLabelPrivate::reloadStrings() const {
        label->setAccessibleDescription(tr("Press enter to edit."));
    }






    EditLabel::EditLabel(QWidget *parent) : EditLabel(parent, *new EditLabelPrivate) {
        Q_D(EditLabel);
        d->q_ptr = this;
        d->label = new QLabel;
        QStackedWidget::addWidget(d->label);
        d->label->setFocusPolicy(Qt::StrongFocus);
        d->label->setCursor(Qt::PointingHandCursor);
        d->label->installEventFilter(this);
        d->reloadStrings();
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

        qIDec->installLocale(d, [d] { d->reloadStrings(); });
    }

    EditLabel::~EditLabel() {
    }

    QWidget *EditLabel::editWidget() const {
        Q_D(const EditLabel);
        return d->editWidget;
    }

    void EditLabel::setEditWidget(QLineEdit *w) {
        Q_D(EditLabel);
        d->addEditWidgetToStackedWidget(w);
    }

    void EditLabel::setEditWidget(QAbstractSpinBox *w) {
        Q_D(EditLabel);
        d->addEditWidgetToStackedWidget(w);
    }

    QString EditLabel::prefix() const {
        Q_D(const EditLabel);
        return d->prefix;
    }
    void EditLabel::setPrefix(const QString &prefix) {
        Q_D(EditLabel);
        d->prefix = prefix;
    }
    QString EditLabel::suffix() const {
        Q_D(const EditLabel);
        return d->suffix;
    }
    void EditLabel::setSuffix(const QString &suffix) {
        Q_D(EditLabel);
        d->suffix = suffix;
    }

    EditLabel::Triggers EditLabel::triggers() const {
        Q_D(const EditLabel);
        return d->triggers;
    }
    void EditLabel::setTriggers(EditLabel::Triggers triggers) {
        Q_D(EditLabel);
        d->triggers = triggers;
    }

    QString EditLabel::text() const {
        Q_D(const EditLabel);
        return d->label->text();
    }

    bool EditLabel::editing() const {
        Q_D(const EditLabel);
        return QStackedWidget::currentWidget() != d->label;
    }

    void EditLabel::setEditing(bool editing_) {
        Q_D(const EditLabel);
        bool oldEditing = editing();
        if (editing_) {
            if (d->editWidget) {
                QStackedWidget::setCurrentWidget(d->editWidget);
            }
        } else {
            QStackedWidget::setCurrentWidget(d->label);
        }
        if (oldEditing != editing())
            Q_EMIT editingChanged(!oldEditing);
    }

    QLabel *EditLabel::label() const {
        Q_D(const EditLabel);
        return d->label;
    }

    QString EditLabel::editToLabel(const QString &editText) const {
        return editText;
    }

    EditLabel::EditLabel(QWidget *parent, EditLabelPrivate &d) : QStackedWidget(parent), d_ptr(&d) {
    }

    bool EditLabel::eventFilter(QObject *watched, QEvent *event) {
        Q_D(EditLabel);
        if (watched == d->label) {
            if ((d->triggers & KeyboardFocusIn) && event->type() == QEvent::FocusIn) {
                auto focusEvent = static_cast<QFocusEvent *>(event);
                if (focusEvent->reason() == Qt::FocusReason::TabFocusReason || focusEvent->reason() == Qt::BacktabFocusReason || focusEvent->reason() == Qt::ShortcutFocusReason) {
                    setEditing(true);
                    return true;
                }
                return false;
            }
            if ((d->triggers & MousePress) && event->type() == QEvent::MouseButtonPress) {
                setEditing(true);
                return true;
            }
            if ((d->triggers & MouseDblClick) && event->type() == QEvent::MouseButtonDblClick) {
                setEditing(true);
                return true;
            }
            if (event->type() == QEvent::KeyPress) {
                auto keyEvent = static_cast<QKeyEvent *>(event);
                if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Space) {
                    setEditing(true);
                    return true;
                }
                return false;
            }
            return false;
        } else {
            if (event->type() == QEvent::FocusOut) {
                setEditing(false);
                return false;
            } else if (event->type() == QEvent::KeyPress) {
                auto keyEvent = static_cast<QKeyEvent *>(event);
                if (keyEvent->key() == Qt::Key_Return) {
                    setEditing(false);
                    return true;
                }
            }
            return false;
        }
    }

} // SVS