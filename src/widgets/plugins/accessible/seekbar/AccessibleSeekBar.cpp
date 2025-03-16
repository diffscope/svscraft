#include "AccessibleSeekBar.h"

namespace SVS {
    AccessibleSeekBar::AccessibleSeekBar(QWidget *w) : QAccessibleWidget(w, QAccessible::Slider) {
        Q_ASSERT(seekBar());
        addControllingSignal(QLatin1String("valueChanged(double)"));
    }
    void *AccessibleSeekBar::interface_cast(QAccessible::InterfaceType t) {
        if (t == QAccessible::ValueInterface)
            return static_cast<QAccessibleValueInterface *>(this);
        return QAccessibleWidget::interface_cast(t);
    }
    QVariant AccessibleSeekBar::currentValue() const {
        return seekBar()->value();
    }
    void AccessibleSeekBar::setCurrentValue(const QVariant &value) {
        seekBar()->setValue(value.toDouble());
    }
    QVariant AccessibleSeekBar::maximumValue() const {
        return seekBar()->maximum();
    }
    QVariant AccessibleSeekBar::minimumValue() const {
        return seekBar()->minimum();
    }
    QVariant AccessibleSeekBar::minimumStepSize() const {
        return seekBar()->singleStep();
    }
    QString AccessibleSeekBar::text(QAccessible::Text t) const {
        if (t == QAccessible::Value) {
            return QString::number(seekBar()->displayValue());
        }
        return QAccessibleWidget::text(t);
    }
    SeekBar *AccessibleSeekBar::seekBar() const {
        return qobject_cast<SeekBar *>(object());
    }
} // SVS