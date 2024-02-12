#ifndef MUSICNOTESPINBOX_H
#define MUSICNOTESPINBOX_H

#include <QSpinBox>

#include <SVSCraftCore/musicnote.h>

namespace SVS {

    class MusicNoteSpinBoxPrivate;

    class MusicNoteSpinBox : public QSpinBox {
        Q_OBJECT
        Q_DECLARE_PRIVATE(MusicNoteSpinBox)
    public:
        MusicNoteSpinBox(QWidget *parent = nullptr);
        ~MusicNoteSpinBox() override;
        
        QString cleanText() const = delete;
        int displayIntegerBase() const = delete;
        void setDisplayIntegerBase(int) = delete;
        int singleStep() const = delete;
        void setSingleStep(int) = delete;
        StepType stepType() const = delete;
        void setStepType(StepType) = delete;

        int maximum() const = delete;
        void setMaximum(int) = delete;
        int minimum() const = delete;
        void setMinimum(int minimum) = delete;
        void setRange(int minimum, int maximum) = delete;

        bool isWildcardEnabled() const;
        void setWildcardEnabled(bool enabled);

        MusicNote::Accidental accidental() const;
        void setAccidental(MusicNote::Accidental accidental);

        MusicNote value() const;

    public slots:
        void setValue(MusicNote value);

    signals:
        void valueChanged(MusicNote value);

    protected:
        QValidator::State validate(QString &input, int &pos) const override;
        void fixup(QString &input) const override;
        int valueFromText(const QString &text) const override;
        QString textFromValue(int val) const override;

    protected:
        QScopedPointer<MusicNoteSpinBoxPrivate> d_ptr;
    };

} // SVS

#endif // MUSICNOTESPINBOX_H
