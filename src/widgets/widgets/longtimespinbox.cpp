#include "longtimespinbox.h"
#include "longtimespinbox_p.h"

namespace SVS {
    LongTimeSpinBox::LongTimeSpinBox(QWidget *parent) : QSpinBox(parent), d_ptr(new LongTimeSpinBoxPrivate) {
        Q_D(LongTimeSpinBox);
        d->q_ptr = this;
        QSpinBox::setSingleStep(1000);
        QSpinBox::setRange(0, std::numeric_limits<int>::max());
        connect(this, &QSpinBox::valueChanged, this, [=](int value) {
            emit valueChanged(LongTime(value));
        });
    }
    LongTimeSpinBox::~LongTimeSpinBox() = default;

    LongTime LongTimeSpinBox::maximum() const {
        return LongTime(QSpinBox::maximum());
    }
    void LongTimeSpinBox::setMaximum(LongTime maximum) {
        QSpinBox::setMaximum(maximum.totalMsec());
    }
    LongTime LongTimeSpinBox::minimum() const {
        return LongTime(QSpinBox::minimum());
    }
    void LongTimeSpinBox::setMinimum(LongTime minimum) {
        QSpinBox::setMinimum(minimum.totalMsec());
    }
    void LongTimeSpinBox::setRange(LongTime minimum, LongTime maximum) {
        QSpinBox::setRange(minimum.totalMsec(), maximum.totalMsec());
    }

    int LongTimeSpinBox::fieldWidth(LongTimeSpinBox::FieldType fieldType) const {
        Q_D(const LongTimeSpinBox);
        switch (fieldType) {
            case Minute:
                return d->minuteWidth;
            case Second:
                return d->secondWidth;
            case Msec:
                return d->msecWidth;
        }
        return 0;
    }
    void LongTimeSpinBox::setFieldWidth(LongTimeSpinBox::FieldType fieldType, int width) {
        Q_D(LongTimeSpinBox);
        switch (fieldType) {
            case Minute:
                d->minuteWidth = width;
                break;
            case Second:
                d->secondWidth = width;
                break;
            case Msec:
                d->msecWidth = width;
                break;
        }
    }

    LongTime LongTimeSpinBox::value() const {
        return LongTime(QSpinBox::value());
    }

    QValidator::State LongTimeSpinBox::validate(QString &input, int &pos) const {
        Q_D(const LongTimeSpinBox);
        if (LongTime::fromString(input).toString(d->minuteWidth, d->secondWidth, d->msecWidth) == input)
            return QValidator::Acceptable;
        else
            return QValidator::Intermediate;
    }
    void LongTimeSpinBox::fixup(QString &input) const {
        Q_D(const LongTimeSpinBox);
        input = LongTime::fromString(input).toString(d->minuteWidth, d->secondWidth, d->msecWidth);
    }
    void LongTimeSpinBox::setValue(LongTime value) {
        QSpinBox::setValue(value.totalMsec());
    }
    int LongTimeSpinBox::valueFromText(const QString &text) const {
        return LongTime::fromString(text).totalMsec();
    }
    QString LongTimeSpinBox::textFromValue(int val) const {
        Q_D(const LongTimeSpinBox);
        return LongTime(val).toString(d->minuteWidth, d->secondWidth, d->msecWidth);
    }
} // SVS