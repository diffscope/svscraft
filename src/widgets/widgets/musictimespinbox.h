#ifndef MUSICTIMESPINBOX_H
#define MUSICTIMESPINBOX_H

#include <SVSCraftWidgets/svscraftwidgetsglobal.h>

#include <QSpinBox>

#include <SVSCraftCore/musictime.h>

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

        PersistentMusicTime maximum() const;
        void setMaximum(const PersistentMusicTime& maximum);
        PersistentMusicTime minimum() const;
        void setMinimum(const PersistentMusicTime& minimum);
        void setRange(const PersistentMusicTime& minimum, const PersistentMusicTime& maximum);

        enum FieldType {
            Measure,
            Beat,
            Tick,
        };
        int fieldWidth(FieldType fieldType) const;
        void setFieldWidth(FieldType fieldType, int width);

        PersistentMusicTime value() const;

    public slots:
        void setValue(const PersistentMusicTime& value);

    signals:
        void valueChanged(const PersistentMusicTime &value);

    protected:
        QScopedPointer<MusicTimeSpinBoxPrivate> d_ptr;
        QValidator::State validate(QString &input, int &pos) const override;
        void fixup(QString &input) const override;
        int valueFromText(const QString &text) const override;
        QString textFromValue(int val) const override;
    };

} // SVS

#endif // MUSICTIMESPINBOX_H
