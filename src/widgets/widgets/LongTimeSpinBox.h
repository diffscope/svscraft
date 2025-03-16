#ifndef SVSCRAFT_LONGTIMESPINBOX_H
#define SVSCRAFT_LONGTIMESPINBOX_H

#include <QSpinBox>

#include <SVSCraftCore/LongTime.h>
#include <SVSCraftWidgets/SVSCraftWidgetsGlobal.h>

namespace SVS {

    class LongTimeSpinBoxPrivate;

    class SVSCRAFT_WIDGETS_EXPORT LongTimeSpinBox : public QSpinBox {
        Q_OBJECT
        Q_DECLARE_PRIVATE(LongTimeSpinBox)
    public:
        LongTimeSpinBox(QWidget *parent = nullptr);
        ~LongTimeSpinBox() override;

        QString cleanText() const = delete;
        int displayIntegerBase() const = delete;
        void setDisplayIntegerBase(int) = delete;
        int singleStep() const = delete;
        void setSingleStep(int) = delete;
        StepType stepType() const = delete;
        void setStepType(StepType) = delete;

        QString prefix() const = delete;
        void setPrefix(const QString &) = delete;
        QString suffix() const = delete;
        void setSuffix(const QString &) = delete;

        LongTime maximum() const;
        void setMaximum(LongTime maximum);
        LongTime minimum() const;
        void setMinimum(LongTime minimum);
        void setRange(LongTime minimum, LongTime maximum);

        enum FieldType {
            Minute,
            Second,
            Msec,
        };
        int fieldWidth(FieldType fieldType) const;
        void setFieldWidth(FieldType fieldType, int width);

        LongTime value() const;
        Q_INVOKABLE void setValue(LongTime value);

    Q_SIGNALS:
        void valueChanged(LongTime value);

    protected:
        QValidator::State validate(QString &input, int &pos) const override;
        void fixup(QString &input) const override;
        int valueFromText(const QString &text) const override;
        QString textFromValue(int val) const override;

    protected:
        QScopedPointer<LongTimeSpinBoxPrivate> d_ptr;
    };

}

#endif // SVSCRAFT_LONGTIMESPINBOX_H
