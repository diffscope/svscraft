#ifndef SVSCRAFT_EDITLABEL_H
#define SVSCRAFT_EDITLABEL_H

#include <QStackedWidget>

#include <SVSCraftWidgets/SVSCraftWidgetsGlobal.h>

class QLabel;
class QLineEdit;
class QAbstractSpinBox;

namespace SVS {

    class EditLabelPrivate;

    class SVSCRAFT_WIDGETS_EXPORT EditLabel : public QStackedWidget {
        Q_OBJECT
        Q_DECLARE_PRIVATE(EditLabel)
        Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
        Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
        Q_PROPERTY(Triggers triggers READ triggers WRITE setTriggers)
        Q_PROPERTY(bool editing READ editing WRITE setEditing NOTIFY editingChanged)
        Q_PROPERTY(QString text READ text NOTIFY textChanged)
    public:
        explicit EditLabel(QWidget *parent = nullptr);
        ~EditLabel() override;

        int addWidget(QWidget *) = delete;
        int insertWidget(int, QWidget *) = delete;
        void removeWidget(QWidget *) = delete;
        QWidget *currentWidget() const = delete;
        int currentIndex() const = delete;
        int indexOf(const QWidget *) const = delete;
        QWidget *widget(int) const = delete;
        int count() const = delete;
        void setCurrentIndex(int) = delete;
        void setCurrentWidget(QWidget *) = delete;
        void currentChanged(int) = delete;
        void widgetRemoved(int) = delete;

        QWidget *editWidget() const;
        void setEditWidget(QLineEdit *w);
        void setEditWidget(QAbstractSpinBox *w);

        QString prefix() const;
        void setPrefix(const QString &prefix);
        QString suffix() const;
        void setSuffix(const QString &suffix);

        enum Trigger {
            MousePress = 0x01,
            MouseDblClick = 0x02,
        };
        Q_DECLARE_FLAGS(Triggers, Trigger)
        Q_FLAG(Triggers)
        Triggers triggers() const;
        void setTriggers(Triggers triggers);

        QString text() const;

        bool editing() const;

    public Q_SLOTS:
        void setEditing(bool editing);

    Q_SIGNALS:
        void editingChanged(bool editing);
        void textChanged(const QString &text);

    protected:
        QLabel *label() const;
        virtual QString editToLabel(const QString &editText) const;
        bool eventFilter(QObject *watched, QEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;
        void paintEvent(QPaintEvent *event) override;

        explicit EditLabel(QWidget *parent, EditLabelPrivate &d);

    private:
        QScopedPointer<EditLabelPrivate> d_ptr;
    };

} // SVS

#endif // SVSCRAFT_EDITLABEL_H
