#ifndef SEEKBAR_H
#define SEEKBAR_H

#include <SVSCraftWidgets/svscraftwidgetsglobal.h>

#include <QWidget>

namespace SVS {

    class SeekBarPrivate;

    class SVSCRAFT_WIDGETS_EXPORT SeekBar : public QWidget {
        Q_OBJECT
        Q_DECLARE_PRIVATE(SeekBar)
    public:
        explicit SeekBar(QWidget *parent = nullptr);
        ~SeekBar() override;

        double maximum() const;
        void setMaximum(double maximum);
        double minimum() const;
        void setMinimum(double minimum);
        void setRange(double minimum, double maximum);

        double interval() const;
        void setInterval(double interval);

        double singleStep() const;
        void setSingleStep(double step);
        double pageStep() const;
        void setPageStep(double step);

        bool hasTracking() const;
        void setTracking(bool enable);

        bool isSliderDown() const;
        void setSliderDown(bool down);

        double sliderPosition() const;
        void setSliderPosition(double position);

        double trackActiveStartValue() const;
        void setTrackActiveStartValue(double pos);

        double value() const;

        double defaultValue() const;
        void setDefaultValue(double value);

        double displayValue() const;

    public Q_SLOTS:
        void setValue(double value);
        void resetValue();

    Q_SIGNALS:
        void sliderPressed();
        void sliderReleased();
        void sliderMoved(double value);
        void valueChanged(double value);

    protected:
        bool eventFilter(QObject *object, QEvent *event) override;
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;

        explicit SeekBar(QWidget *parent, SeekBarPrivate &d);

        virtual double displayValueFromActualValue(double value) const;

    private:
        QScopedPointer<SeekBarPrivate> d_ptr;

    };

} // SVS

#endif // SEEKBAR_H
