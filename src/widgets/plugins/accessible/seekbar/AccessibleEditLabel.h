#ifndef SVSCRAFT_ACCESSIBLEEDITLABEL_H
#define SVSCRAFT_ACCESSIBLEEDITLABEL_H

#include <QAccessibleWidget>

namespace SVS {

    class EditLabel;

    class AccessibleEditLabel : public QAccessibleWidget {
    public:
        explicit AccessibleEditLabel(QWidget *w);
        QAccessible::Role role() const override;
        QStringList actionNames() const override;
        void doAction(const QString &actionName) override;
        QString text(QAccessible::Text t) const override;
        QAccessible::State state() const override;

        int childCount() const override;

    protected:
        EditLabel *editLabel() const;
    };

}

#endif // SVSCRAFT_ACCESSIBLEEDITLABEL_H
