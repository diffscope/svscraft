#include "musicpitchspinbox.h"

namespace SVS {

    namespace {
        struct initializer {
            initializer() {
                qRegisterMetaType<MusicPitch>();
            }
        } _;
    }

    class MusicPitchSpinBoxPrivate : public QObject {
        Q_DECLARE_PUBLIC(MusicPitchSpinBox)
    public:
        MusicPitchSpinBox *q_ptr;
        MusicPitch::Accidental accidental = MusicPitch::Flat;
        void _q_valueChanged(int value) {
            Q_Q(MusicPitchSpinBox);
            emit q->valueChanged(MusicPitch(qint8(value)));
        }
    };


    MusicPitchSpinBox::MusicPitchSpinBox(QWidget *parent) : QSpinBox(parent), d_ptr(new MusicPitchSpinBoxPrivate) {
        Q_D(MusicPitchSpinBox);
        d->q_ptr = this;
        QSpinBox::setRange(0, 127);
        connect(this, QOverload<int>::of(&QSpinBox::valueChanged), d, &MusicPitchSpinBoxPrivate::_q_valueChanged);
    }
    MusicPitchSpinBox::~MusicPitchSpinBox() = default;

    bool MusicPitchSpinBox::isWildcardEnabled() const {
        return QSpinBox::minimum() < 0;
    }
    void MusicPitchSpinBox::setWildcardEnabled(bool enabled) {
        QSpinBox::setRange(enabled ? -12 : 0, 127);
    }

    MusicPitch MusicPitchSpinBox::value() const {
        return MusicPitch(qint8(QSpinBox::value()));
    }
    void MusicPitchSpinBox::setValue(MusicPitch value) {
        QSpinBox::setValue(value.pitch());
    }

    MusicPitch::Accidental MusicPitchSpinBox::accidental() const {
        Q_D(const MusicPitchSpinBox);
        return d->accidental;
    }
    void MusicPitchSpinBox::setAccidental(MusicPitch::Accidental accidental) {
        Q_D(MusicPitchSpinBox);
        if (accidental != d->accidental) {
            d->accidental = accidental;
            QSpinBox::setValue(QSpinBox::value());
        }
    }

    QValidator::State MusicPitchSpinBox::validate(QString &input, int &pos) const {
        Q_D(const MusicPitchSpinBox);
        if (MusicPitch::fromString(input).toString(d->accidental) == input)
            return QValidator::Acceptable;
        else
            return QValidator::Intermediate;
    }
    void MusicPitchSpinBox::fixup(QString &input) const {
        Q_D(const MusicPitchSpinBox);
        bool ok;
        auto ret = MusicPitch::fromString(input, &ok).toString(d->accidental);
        if (ok)
            input = ret;
    }
    int MusicPitchSpinBox::valueFromText(const QString &text) const {
        return MusicPitch::fromString(text).pitch();
    }
    QString MusicPitchSpinBox::textFromValue(int val) const {
        Q_D(const MusicPitchSpinBox);
        return MusicPitch(qint8(val)).toString(d->accidental);
    }


} // SVS