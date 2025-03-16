#include "SwitchButton.h"

#include <QEvent>
#include <QPainter>
#include <QVariantAnimation>

namespace SVS {

    class SwitchButtonPrivate : public QObject {
        Q_DECLARE_PUBLIC(SwitchButton)
    public:
        SwitchButton *q_ptr;
        QRect rect;
        //    int penWidth;
        int thumbRadius;
        int halfRectHeight;
        QPoint trackStart;
        QPoint trackEnd;
        int trackLength;
        QVariantAnimation *valueAnimation;
        QVariantAnimation *thumbHoverAnimation;

        void init();
        void calculateParams();

        int apparentValue = 0;
        int thumbScaleRatio = 100; // max = 100%


        QColor thumbColor = QColor(255, 255, 255);
        QColor inactiveColor = QColor(32, 32, 32);
        QColor activeColor = QColor(112, 156, 255);
        int valueAnimationDuration = 250;
        int thumbHoverAnimationDuration = 200;
    };

    void SwitchButtonPrivate::init() {
        Q_Q(SwitchButton);
        q->setAttribute(Qt::WA_Hover, true);
        q->installEventFilter(q);

        connect(q, &QAbstractButton::toggled, this, [=](bool checked) {
            valueAnimation->stop();
            valueAnimation->setStartValue(apparentValue);
            valueAnimation->setEndValue(checked ? 255 : 0);
            valueAnimation->start();
        });

        valueAnimation = new QVariantAnimation(q);
        valueAnimation->setDuration(valueAnimationDuration);
        valueAnimation->setEasingCurve(QEasingCurve::InOutCubic);
        connect(valueAnimation, &QVariantAnimation::valueChanged, this, [=](const QVariant &value) {
            apparentValue = value.toInt();
            q->repaint();
        });

        thumbHoverAnimation = new QVariantAnimation(q);
        thumbHoverAnimation->setDuration(thumbHoverAnimationDuration);
        thumbHoverAnimation->setEasingCurve(QEasingCurve::OutCubic);
        connect(thumbHoverAnimation, &QVariantAnimation::valueChanged, this,
                [=](const QVariant &value) {
                    thumbScaleRatio = value.toInt();
                    q->repaint();
                });

        q->setMinimumSize(40, 20);
        q->setMaximumSize(40, 20);
        q->setCheckable(true);
        calculateParams();
    }
    void SwitchButtonPrivate::calculateParams() {
        Q_Q(SwitchButton);
        rect = q->rect();
        //    penWidth = qRound(rect().height() / 16.0);
        halfRectHeight = rect.height() / 2;
        thumbRadius = qRound(halfRectHeight / 2.0);
        trackStart.setX(rect.left() + halfRectHeight);
        trackStart.setY(halfRectHeight);
        trackEnd.setX(rect.right() - halfRectHeight);
        trackEnd.setY(halfRectHeight);
        trackLength = trackEnd.x() - trackStart.x();
    }

    SwitchButton::SwitchButton(QWidget *parent) : SwitchButton(parent, *new SwitchButtonPrivate) {
        Q_D(SwitchButton);
        d->q_ptr = this;
        d->init();
    }

    SwitchButton::~SwitchButton() = default;

    void SwitchButton::paintEvent(QPaintEvent *event) {
        Q_D(SwitchButton);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QPen pen;
        QColor penColor;

        // Draw inactive background
        pen.setWidth(d->rect.height());
        penColor = d->inactiveColor;
        penColor.setAlpha(d->apparentValue == 255 ? 0 : 32);
        pen.setColor(penColor);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
        painter.drawLine(d->trackStart, d->trackEnd);

        // Draw active background
        penColor = d->activeColor;
        penColor.setAlpha(d->apparentValue);
        pen.setColor(penColor);
        painter.setPen(pen);
        painter.drawLine(d->trackStart, d->trackEnd);

        // Draw thumb
        auto left = d->apparentValue * d->trackLength / 255.0 + d->halfRectHeight;
        //    qDebug() << d->apparentValue;
        auto handlePos = QPointF(left, d->halfRectHeight);
        auto thumbRadius = d->thumbRadius * d->thumbScaleRatio / 100.0;

        painter.setPen(Qt::NoPen);
        painter.setBrush(d->thumbColor);
        painter.drawEllipse(handlePos, thumbRadius, thumbRadius);
    }

    void SwitchButton::resizeEvent(QResizeEvent *event) {
        Q_D(SwitchButton);
        d->calculateParams();
    }
    bool SwitchButton::eventFilter(QObject *object, QEvent *event) {
        Q_D(SwitchButton);
        auto type = event->type();
        if (type == QEvent::HoverEnter) {
            //        qDebug() << "Hover Enter";
            d->thumbHoverAnimation->stop();
            d->thumbHoverAnimation->setStartValue(d->thumbScaleRatio);
            d->thumbHoverAnimation->setEndValue(125);
            d->thumbHoverAnimation->start();
        } else if (type == QEvent::HoverLeave) {
            //        qDebug() << "Hover Leave";
            d->thumbHoverAnimation->stop();
            d->thumbHoverAnimation->setStartValue(d->thumbScaleRatio);
            d->thumbHoverAnimation->setEndValue(100);
            d->thumbHoverAnimation->start();
        } else if (type == QEvent::MouseButtonPress) {
            d->thumbHoverAnimation->stop();
            d->thumbHoverAnimation->setStartValue(d->thumbScaleRatio);
            d->thumbHoverAnimation->setEndValue(85);
            d->thumbHoverAnimation->start();
        } else if (type == QEvent::MouseButtonRelease) {
            d->thumbHoverAnimation->stop();
            d->thumbHoverAnimation->setStartValue(d->thumbScaleRatio);
            d->thumbHoverAnimation->setEndValue(125);
            d->thumbHoverAnimation->start();
        }
        return QObject::eventFilter(object, event);
    }
    SwitchButton::SwitchButton(QWidget *parent, SwitchButtonPrivate &d)
        : QAbstractButton(parent), d_ptr(&d) {
    }

    QColor SwitchButton::thumbColor() const {
        Q_D(const SwitchButton);
        return d->thumbColor;
    }
    void SwitchButton::setThumbColor(const QColor &color) {
        Q_D(SwitchButton);
        d->thumbColor = color;
        update();
    }
    QColor SwitchButton::inactiveColor() const {
        Q_D(const SwitchButton);
        return d->inactiveColor;
    }
    void SwitchButton::setInactiveColor(const QColor &color) {
        Q_D(SwitchButton);
        d->inactiveColor = color;
        update();
    }
    QColor SwitchButton::activeColor() const {
        Q_D(const SwitchButton);
        return d->activeColor;
    }
    void SwitchButton::setActiveColor(const QColor &color) {
        Q_D(SwitchButton);
        d->activeColor = color;
        update();
    }
    int SwitchButton::valueAnimationDuration() const {
        Q_D(const SwitchButton);
        return d->valueAnimationDuration;
    }
    void SwitchButton::setValueAnimationDuration(int dur) {
        Q_D(SwitchButton);
        d->valueAnimationDuration = dur;
        d->valueAnimation->setDuration(d->valueAnimationDuration);
    }
    int SwitchButton::thumbHoverAnimationDuration() const {
        Q_D(const SwitchButton);
        return d->thumbHoverAnimationDuration;
    }
    void SwitchButton::setThumbHoverAnimationDuration(int dur) {
        Q_D(SwitchButton);
        d->thumbHoverAnimationDuration = dur;
        d->thumbHoverAnimation->setDuration(d->thumbHoverAnimationDuration);
    }

} // SVS