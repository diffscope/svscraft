#ifndef SVSCRAFT_MUSICTIMESPINBOX_H
#define SVSCRAFT_MUSICTIMESPINBOX_H

#include <QSpinBox>

#include <SVSCraftCore/MusicTime.h>
#include <SVSCraftWidgets/SVSCraftWidgetsGlobal.h>

namespace SVS {

    class MusicTimeSpinBoxPrivate;

    class SVSCRAFT_WIDGETS_EXPORT MusicTimeSpinBox : public QSpinBox {
        Q_OBJECT
        Q_DECLARE_PRIVATE(MusicTimeSpinBox)
    public:
        MusicTimeSpinBox(MusicTimeline *timeline, QWidget *parent = nullptr);
        ~MusicTimeSpinBox() override;

        MusicTimeline *timeline() const;

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

        PersistentMusicTime maximum() const;
        void setMaximum(const PersistentMusicTime &maximum);
        PersistentMusicTime minimum() const;
        void setMinimum(const PersistentMusicTime &minimum);
        void setRange(const PersistentMusicTime &minimum, const PersistentMusicTime &maximum);

        enum FieldType {
            Measure,
            Beat,
            Tick,
        };
        int fieldWidth(FieldType fieldType) const;
        void setFieldWidth(FieldType fieldType, int width);

        PersistentMusicTime value() const;
        Q_INVOKABLE void setValue(const PersistentMusicTime &value);

    Q_SIGNALS:
        void valueChanged(const PersistentMusicTime &value);

    protected:
        QValidator::State validate(QString &input, int &pos) const override;
        void fixup(QString &input) const override;
        int valueFromText(const QString &text) const override;
        QString textFromValue(int val) const override;

    protected:
        QScopedPointer<MusicTimeSpinBoxPrivate> d_ptr;
    };

}

#endif // SVSCRAFT_MUSICTIMESPINBOX_H
