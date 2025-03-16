#ifndef SVSCRAFT_MUSICPITCHVALIDATOR_H
#define SVSCRAFT_MUSICPITCHVALIDATOR_H

#include <QIntValidator>

#include <SVSCraftCore/MusicPitch.h>
#include <SVSCraftGui/svscraftguiglobal.h>

namespace SVS {

    class SVSCRAFT_GUI_EXPORT MusicPitchValidator : public QIntValidator {
        Q_OBJECT
        Q_PROPERTY(MusicPitch::Accidental accidental READ accidental WRITE setAccidental NOTIFY accidentalChanged)

    public:
        explicit MusicPitchValidator(QObject *parent = nullptr);
        MusicPitchValidator(int minimum, int maximum, QObject *parent = nullptr);
        ~MusicPitchValidator() override;

        MusicPitch::Accidental accidental() const;
        void setAccidental(MusicPitch::Accidental value);

        void fixup(QString &) const override;
        State validate(QString &, int &) const override;

    signals:
        void accidentalChanged(MusicPitch::Accidental value);

    private:
        MusicPitch::Accidental m_accidental;

    };

}

#endif // SVSCRAFT_MUSICPITCHVALIDATOR_H
