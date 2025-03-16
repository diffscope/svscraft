#ifndef SVSCRAFT_NONESCROLLAREA_H
#define SVSCRAFT_NONESCROLLAREA_H

#include <QFrame>

#include <SVSCraftWidgets/svscraftwidgetsglobal.h>

namespace SVS {

    class SVSCRAFT_WIDGETS_EXPORT NoneScrollArea : public QFrame {
        Q_OBJECT
    public:
        explicit NoneScrollArea(QWidget *parent = nullptr);
        ~NoneScrollArea();

    public:
        void setWidget(QWidget *widget);
        QWidget *widget() const;
        QWidget *takeWidget();

        void setPercentageX(double value);
        void setPercentageY(double value);

        double percentageX();
        double percentageY();

        void setValueX(int value);
        int valueX() const;

        void setValueY(int value);
        int valueY() const;

    protected:
        QWidget *m_widget;

        void resizeEvent(QResizeEvent *event) override;

    Q_SIGNALS:
        void resized(const QSize &oldSize, const QSize &size);
    };

}

#endif // SVSCRAFT_NONESCROLLAREA_H
