#include "AccessibleEditLabel.h"

#include <QAbstractSpinBox>
#include <QLineEdit>

#include <SVSCraftWidgets/EditLabel.h>

namespace SVS {
    AccessibleEditLabel::AccessibleEditLabel(QWidget *w) : QAccessibleWidget(w) {
    }
    QAccessible::Role AccessibleEditLabel::role() const {
        auto editWidget = editLabel()->editWidget();
        if (qobject_cast<QAbstractSpinBox *>(editWidget)) {
            return QAccessible::SpinBox;
        } else if (qobject_cast<QLineEdit *>(editWidget)) {
            return QAccessible::EditableText;
        } else {
            return QAccessible::StaticText;
        }
    }
    QStringList AccessibleEditLabel::actionNames() const {
        return QAccessibleWidget::actionNames() << pressAction() << toggleAction();
    }
    void AccessibleEditLabel::doAction(const QString &actionName) {
        if (actionName == pressAction() || actionName == toggleAction()) {
            editLabel()->setEditing(true);
            if (editLabel()->editWidget())
                editLabel()->editWidget()->setFocus();
        } else {
            QAccessibleWidget::doAction(actionName);
        }
    }
    QString AccessibleEditLabel::text(QAccessible::Text t) const {
        switch (t) {
            case QAccessible::Value:
                return editLabel()->text();
            default:
                return QAccessibleWidget::text(t);
        }
    }
    QAccessible::State AccessibleEditLabel::state() const {
        auto state = QAccessibleWidget::state();
        return state;
    }

    int AccessibleEditLabel::childCount() const {
        return 0;
    }

    EditLabel *AccessibleEditLabel::editLabel() const {
        return qobject_cast<EditLabel *>(object());
    }
}