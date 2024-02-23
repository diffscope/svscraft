#ifndef LEVELMETER_H
#define LEVELMETER_H

#include <QWidget>

#include <SVSCraftWidgets/svscraftwidgetsglobal.h>

namespace SVS {

    class LevelMeterPrivate;

    class SVSCRAFT_WIDGETS_EXPORT LevelMeter : public QWidget {
        Q_OBJECT
        Q_DECLARE_PRIVATE(LevelMeter)
    public:
        explicit LevelMeter(QWidget *parent = nullptr);
        ~LevelMeter() override;

        QPair<bool, bool> clipped() const;
        void resetClipIndicator();

        void setValue(double valueL, double valueR);
        QPair<double, double> value() const;

        void setSafeThreshold(double threshold);
        double safeThreshold() const;

        void setWarnThreshold(double threshold);
        double warnThreshold() const;

        void setMinimum(double minimum);
        double minimum() const;

    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;

        LevelMeter(QWidget *parent, LevelMeterPrivate &d);

    private:
        QScopedPointer<LevelMeterPrivate> d_ptr;
    };

} // SVS

#endif // LEVELMETER_H
