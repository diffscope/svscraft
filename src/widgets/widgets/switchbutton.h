#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QAbstractButton>

namespace SVS {

    class SwitchButtonPrivate;

    class SwitchButton : public QAbstractButton {
        Q_OBJECT
        Q_DECLARE_PRIVATE(SwitchButton)
        Q_PROPERTY(QColor thumbColor READ thumbColor WRITE setThumbColor)
        Q_PROPERTY(QColor inactiveColor READ inactiveColor WRITE setInactiveColor)
        Q_PROPERTY(QColor activeColor READ activeColor WRITE setActiveColor)
        Q_PROPERTY(int valueAnimationDuration READ valueAnimationDuration WRITE setValueAnimationDuration)
        Q_PROPERTY(int thumbHoverAnimationDuration READ thumbHoverAnimationDuration WRITE setThumbHoverAnimationDuration)
    public:
        explicit SwitchButton(QWidget *parent = nullptr);
        ~SwitchButton() override;

    protected:
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
        bool eventFilter(QObject *object, QEvent *event) override;

        SwitchButton(QWidget *parent, SwitchButtonPrivate &d);


    private:
        QScopedPointer<SwitchButtonPrivate> d_ptr;

        QColor thumbColor() const;
        void setThumbColor(const QColor& color);
        QColor inactiveColor() const;
        void setInactiveColor(const QColor& color);
        QColor activeColor() const;
        void setActiveColor(const QColor& color);
        int valueAnimationDuration() const;
        void setValueAnimationDuration(int dur);
        int thumbHoverAnimationDuration() const;
        void setThumbHoverAnimationDuration(int dur);
    };

} // SVS

#endif // SWITCHBUTTON_H
