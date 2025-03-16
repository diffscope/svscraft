#ifndef SVSCRAFT_ACCESSIBLESEEKBAR_H
#define SVSCRAFT_ACCESSIBLESEEKBAR_H

#include <QAccessibleWidget>
#include <SVSCraftWidgets/seekbar.h>

namespace SVS {

    class AccessibleSeekBar : public QAccessibleWidget, public QAccessibleValueInterface {
    public:
        explicit AccessibleSeekBar(QWidget *w);
        void *interface_cast(QAccessible::InterfaceType t) override;
        QVariant currentValue() const override;
        void setCurrentValue(const QVariant &value) override;
        QVariant maximumValue() const override;
        QVariant minimumValue() const override;
        QVariant minimumStepSize() const override;
        QString text(QAccessible::Text t) const override;

    protected:
        SeekBar *seekBar() const;
    };

} // SVS

#endif // SVSCRAFT_ACCESSIBLESEEKBAR_H
