#ifndef SVSCRAFT_LEVELMETER_H
#define SVSCRAFT_LEVELMETER_H

#include <QWidget>

#include <SVSCraftWidgets/SVSCraftWidgetsGlobal.h>

namespace SVS {

    class LevelMeterPrivate;

    class SVSCRAFT_WIDGETS_EXPORT LevelMeter : public QWidget {
        Q_OBJECT
        Q_DECLARE_PRIVATE(LevelMeter)
        Q_PROPERTY(QColor colorBackground READ colorBackground WRITE setColorBackground)
        Q_PROPERTY(QColor colorSafe READ colorSafe WRITE setColorSafe)
        Q_PROPERTY(QColor colorWarn READ colorWarn WRITE setColorWarn)
        Q_PROPERTY(QColor colorCritical READ colorCritical WRITE setColorCritical)
        Q_PROPERTY(double safeThreshold READ safeThreshold WRITE setSafeThreshold)
        Q_PROPERTY(double warnThreshold READ warnThreshold WRITE setWarnThreshold)
        Q_PROPERTY(double minimum READ minimum WRITE setMinimum)
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

        QColor colorBackground() const;
        void setColorBackground(const QColor &color);
        QColor colorSafe() const;
        void setColorSafe(const QColor &color);
        QColor colorWarn() const;
        void setColorWarn(const QColor &color);
        QColor colorCritical() const;
        void setColorCritical(const QColor &color);
    };

} // SVS

#endif // SVSCRAFT_LEVELMETER_H
