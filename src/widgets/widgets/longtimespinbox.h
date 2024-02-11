#ifndef LONGTIMESPINBOX_H
#define LONGTIMESPINBOX_H

#include <QSpinBox>

#include <SVSCraftCore/longtime.h>
#include <SVSCraftWidgets/svscraftwidgetsglobal.h>

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

    signals:
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

#endif // LONGTIMESPINBOX_H
