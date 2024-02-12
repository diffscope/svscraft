#include "musicnotespinbox.h"

namespace SVS {

    class MusicNoteSpinBoxPrivate : public QObject {
        Q_DECLARE_PUBLIC(MusicNoteSpinBox)
    public:
        MusicNoteSpinBox *q_ptr;
        MusicNote::Accidental accidental = MusicNote::Flat;
        void _q_valueChanged(int value) {
            Q_Q(MusicNoteSpinBox);
            emit q->valueChanged(MusicNote(qint8(value)));
        }
    };


    MusicNoteSpinBox::MusicNoteSpinBox(QWidget *parent) : QSpinBox(parent), d_ptr(new MusicNoteSpinBoxPrivate) {
        Q_D(MusicNoteSpinBox);
        d->q_ptr = this;
        QSpinBox::setRange(0, 127);
        connect(this, QOverload<int>::of(&QSpinBox::valueChanged), d, &MusicNoteSpinBoxPrivate::_q_valueChanged);
    }
    MusicNoteSpinBox::~MusicNoteSpinBox() = default;

    bool MusicNoteSpinBox::isWildcardEnabled() const {
        return QSpinBox::minimum() < 0;
    }
    void MusicNoteSpinBox::setWildcardEnabled(bool enabled) {
        QSpinBox::setRange(enabled ? -12 : 0, 127);
    }

    MusicNote MusicNoteSpinBox::value() const {
        return MusicNote(qint8(QSpinBox::value()));
    }
    void MusicNoteSpinBox::setValue(MusicNote value) {
        QSpinBox::setValue(value.pitch());
    }

    MusicNote::Accidental MusicNoteSpinBox::accidental() const {
        Q_D(const MusicNoteSpinBox);
        return d->accidental;
    }
    void MusicNoteSpinBox::setAccidental(MusicNote::Accidental accidental) {
        Q_D(MusicNoteSpinBox);
        if (accidental != d->accidental) {
            d->accidental = accidental;
            QSpinBox::setValue(QSpinBox::value());
        }
    }

    QValidator::State MusicNoteSpinBox::validate(QString &input, int &pos) const {
        Q_D(const MusicNoteSpinBox);
        if (MusicNote::fromString(input).toString(d->accidental) == input)
            return QValidator::Acceptable;
        else
            return QValidator::Intermediate;
    }
    void MusicNoteSpinBox::fixup(QString &input) const {
        Q_D(const MusicNoteSpinBox);
        bool ok;
        auto ret = MusicNote::fromString(input, &ok).toString(d->accidental);
        if (ok)
            input = ret;
    }
    int MusicNoteSpinBox::valueFromText(const QString &text) const {
        return MusicNote::fromString(text).pitch();
    }
    QString MusicNoteSpinBox::textFromValue(int val) const {
        Q_D(const MusicNoteSpinBox);
        return MusicNote(qint8(val)).toString(d->accidental);
    }


} // SVS