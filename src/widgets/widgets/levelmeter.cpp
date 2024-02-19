#include "levelmeter.h"

#include <QMouseEvent>
#include <QPainter>
#include <QTimer>

namespace SVS {
    
    class LevelMeterPrivate : public QObject {
        Q_DECLARE_PUBLIC(LevelMeter)
    public:
        LevelMeter *q_ptr;
        QColor colorBackground = QColor(0, 0, 0, 60);
        QColor colorSafe = QColor(155, 255, 155);
        QColor colorWarn = QColor(255, 205, 155);
        QColor colorCritical = QColor(255, 155, 157);
        
        double safeThreshold = -18;
        double warnThreshold = -6;
        double minimum = -60;

        double smoothedLevelL = 0;
        double smoothedLevelR = 0;
        bool clippedL = false;
        bool clippedR = false;

        double clipIndicatorLength = 6; // px
        double spacing = 1.0;

        void setAccessibleClipIndicator();
        void reloadStrings();
    };

    void LevelMeterPrivate::setAccessibleClipIndicator() {
        Q_Q(LevelMeter);
        if (clippedL && clippedR) {
            q->setAccessibleDescription(tr("Clipping was detected in left and right channel during playback. Press enter to reset."));
        } else if (clippedL) {
            q->setAccessibleDescription(tr("Clipping was detected in left channel during playback. Press enter to reset."));
        } else if (clippedR) {
            q->setAccessibleDescription(tr("Clipping was detected in right channel during playback. Press enter to reset."));
        } else {
            q->setAccessibleDescription(tr("No clipping detected."));
        }
    }

    void LevelMeterPrivate::reloadStrings() {
        Q_Q(LevelMeter);
        q->setAccessibleName(tr("Level meter"));
        setAccessibleClipIndicator();
    }

    LevelMeter::LevelMeter(QWidget *parent) : LevelMeter(parent, *new LevelMeterPrivate) {
        Q_D(LevelMeter);
        d->q_ptr = this;
        d->reloadStrings();
    }

    LevelMeter::LevelMeter(QWidget *parent, LevelMeterPrivate &d) : QWidget(parent), d_ptr(&d) {
    }

    LevelMeter::~LevelMeter() = default;

    void LevelMeter::paintEvent(QPaintEvent *event) {
        Q_D(LevelMeter);
        QPainter painter(this);
        // Fill background
        painter.setPen(Qt::NoPen);
        painter.fillRect(rect(), d->colorBackground);
        auto paddedRect = QRectF(rect().left() + d->spacing, rect().top() + d->spacing,
                                 rect().width() - 2 * d->spacing, rect().height() - 2 * d->spacing);
        auto channelWidth = (paddedRect.width() - d->spacing) / 2;
        auto leftChannelLeft = paddedRect.left();
        auto rightChannelLeft = leftChannelLeft + channelWidth + d->spacing;
        auto channelTop = paddedRect.top() + d->clipIndicatorLength + d->spacing;
        auto channelLength = paddedRect.bottom() - channelTop;

        // draw clip indicator
        auto leftIndicatorRect =
            QRectF(leftChannelLeft, paddedRect.top(), channelWidth, d->clipIndicatorLength);
        auto rightIndicatorRect =
            QRectF(rightChannelLeft, paddedRect.top(), channelWidth, d->clipIndicatorLength);

        if (d->clippedL)
            painter.fillRect(leftIndicatorRect, d->colorCritical);
        if (d->clippedR)
            painter.fillRect(rightIndicatorRect, d->colorCritical);

        auto drawVerticalBar = [&](const QRectF &rect, const double level) {
            auto rectLeft = rect.left();
            auto rectTop = rect.top();
            auto rectWidth = rect.width();
            auto rectHeight = rect.height();

            auto safeLength = rectHeight * (d->safeThreshold - d->minimum) / (-d->minimum);
            auto warnLength = rectHeight * (d->warnThreshold - d->minimum) / (-d->minimum) - safeLength;

            auto safeStart = rectTop + rectHeight - safeLength;
            auto warnStart = rectTop + rectHeight - safeLength - warnLength;
            auto criticalStart = rectTop;
            auto levelLength = rectHeight * (level - d->minimum) / (-d->minimum);
            auto criticalLength = rectHeight - safeLength - warnLength;

            auto fullSafeChunk = QRectF(rectLeft, safeStart, rectWidth, safeLength);
            auto fullWarnChunk = QRectF(rectLeft, warnStart, rectWidth, warnLength);
            auto fullCriticalChunk = QRectF(rectLeft, criticalStart, rectWidth, criticalLength);

            // draw safe chunk
            if (level < d->safeThreshold) {
                auto height = levelLength;
                auto top = rectTop + rectHeight - height;
                auto chunk = QRectF(rectLeft, top, rectWidth, height);
                painter.fillRect(chunk, d->colorSafe);
            } else if (level < d->warnThreshold) {
                painter.fillRect(fullSafeChunk, d->colorSafe);

                auto height = levelLength - safeLength;
                auto top = rectTop + rectHeight - levelLength;
                auto chunk = QRectF(rectLeft, top, rectWidth, height);
                painter.fillRect(chunk, d->colorWarn);
            } else if (level < 0) {
                painter.fillRect(fullSafeChunk, d->colorSafe);
                painter.fillRect(fullWarnChunk, d->colorWarn);

                auto height = levelLength - safeLength - warnLength;
                auto top = rectTop + rectHeight - levelLength;
                auto chunk = QRectF(rectLeft, top, rectWidth, height);
                painter.fillRect(chunk, d->colorCritical);
            } else {
                painter.fillRect(fullSafeChunk, d->colorSafe);
                painter.fillRect(fullWarnChunk, d->colorWarn);
                painter.fillRect(fullCriticalChunk, d->colorCritical);
            }
        };

        // draw levels
        auto leftLevelBar = QRectF(leftChannelLeft, channelTop, channelWidth, channelLength);
        auto rightLevelBar = QRectF(rightChannelLeft, channelTop, channelWidth, channelLength);
        drawVerticalBar(leftLevelBar, d->smoothedLevelL);
        drawVerticalBar(rightLevelBar, d->smoothedLevelR);
    }

    void LevelMeter::mousePressEvent(QMouseEvent *event) {
        resetClipIndicator();
    }

    void LevelMeter::keyPressEvent(QKeyEvent *event) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Space)
            resetClipIndicator();
    }

    void LevelMeter::setValue(double valueL, double valueR) {
        Q_D(LevelMeter);
        d->smoothedLevelL = valueL;
        d->smoothedLevelR = valueR;
        if (valueL > 0) {
            bool oldValue = d->clippedL;
            d->clippedL = true;
            if (!oldValue)
                d->setAccessibleClipIndicator();
        }
        if (valueR > 0) {
            bool oldValue = d->clippedR;
            d->clippedR = true;
            if (!oldValue)
                d->setAccessibleClipIndicator();
        }
        update();
    }

    void LevelMeter::resetClipIndicator() {
        Q_D(LevelMeter);
        d->clippedL = d->clippedR = false;
        d->setAccessibleClipIndicator();
        update();
    }

    QPair<bool, bool> LevelMeter::clipped() const {
        Q_D(const LevelMeter);
        return {d->clippedL, d->clippedR};
    }
    QPair<double, double> LevelMeter::value() const {
        Q_D(const LevelMeter);
        return {d->smoothedLevelL, d->smoothedLevelR};
    }
    void LevelMeter::setSafeThreshold(double threshold) {
        Q_D(LevelMeter);
        d->safeThreshold = qMax(d->warnThreshold, threshold);
        update();
    }
    double LevelMeter::safeThreshold() const {
        Q_D(const LevelMeter);
        return d->safeThreshold;
    }
    void LevelMeter::setWarnThreshold(double threshold) {
        Q_D(LevelMeter);
        d->warnThreshold = qBound(d->minimum, threshold, d->safeThreshold);
        update();
    }
    double LevelMeter::warnThreshold() const {
        Q_D(const LevelMeter);
        return d->warnThreshold;
    }
    void LevelMeter::setMinimum(double minimum) {
        Q_D(LevelMeter);
        d->minimum = qMin(d->warnThreshold, minimum);
        update();
    }
    double LevelMeter::minimum() const {
        Q_D(const LevelMeter);
        return d->minimum;
    }

} // SVS