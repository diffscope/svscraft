#ifndef SVSCRAFT_MUSICPITCHSPINBOX_H
#define SVSCRAFT_MUSICPITCHSPINBOX_H

#include <QSpinBox>

#include <SVSCraftCore/musicpitch.h>

namespace SVS {

    class MusicPitchSpinBoxPrivate;

    class MusicPitchSpinBox : public QSpinBox {
        Q_OBJECT
        Q_DECLARE_PRIVATE(MusicPitchSpinBox)
    public:
        MusicPitchSpinBox(QWidget *parent = nullptr);
        ~MusicPitchSpinBox() override;
        
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

        QString prefix() const = delete;
        void setPrefix(const QString &) = delete;
        QString suffix() const = delete;
        void setSuffix(const QString &) = delete;

        bool isWildcardEnabled() const;
        void setWildcardEnabled(bool enabled);

        MusicPitch::Accidental accidental() const;
        void setAccidental(MusicPitch::Accidental accidental);

        MusicPitch value() const;

    public Q_SLOTS:
        void setValue(MusicPitch value);

    Q_SIGNALS:
        void valueChanged(MusicPitch value);

    protected:
        QValidator::State validate(QString &input, int &pos) const override;
        void fixup(QString &input) const override;
        int valueFromText(const QString &text) const override;
        QString textFromValue(int val) const override;

    protected:
        QScopedPointer<MusicPitchSpinBoxPrivate> d_ptr;
    };

} // SVS

#endif // SVSCRAFT_MUSICPITCHSPINBOX_H
