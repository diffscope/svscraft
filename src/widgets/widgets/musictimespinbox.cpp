#include "musictimespinbox.h"

#include <SVSCraftCore/musictimeline.h>

namespace SVS {

    class MusicTimeSpinBoxPrivate : public QObject {
        Q_DECLARE_PUBLIC(MusicTimeSpinBox)
    public:
        MusicTimeSpinBox *q_ptr;
        MusicTimeline *timeline;

        int measureWidth = 1;
        int beatWidth = 1;
        int tickWidth = 3;

    private:
        void _q_valueChanged(int value) {
            Q_Q(MusicTimeSpinBox);
            Q_EMIT q->valueChanged(timeline->create(0, 0, value));
        }

        void _q_timeSignatureChanged() {
            Q_Q(MusicTimeSpinBox);
            q->QSpinBox::setValue(q->QSpinBox::value());
        }
    };

    MusicTimeSpinBox::MusicTimeSpinBox(MusicTimeline *timeline, QWidget *parent)
        : QSpinBox(parent), d_ptr(new MusicTimeSpinBoxPrivate) {
        Q_D(MusicTimeSpinBox);
        d->q_ptr = this;
        d->timeline = timeline;
        QSpinBox::setRange(0, std::numeric_limits<int>::max());
        QSpinBox::setSingleStep(480);

        connect(this, QOverload<int>::of(&QSpinBox::valueChanged), d,
                &MusicTimeSpinBoxPrivate::_q_valueChanged);
        connect(timeline, &MusicTimeline::timeSignatureChanged, d,
                &MusicTimeSpinBoxPrivate::_q_timeSignatureChanged);
    }

    MusicTimeSpinBox::~MusicTimeSpinBox() {
    }

    MusicTimeline *MusicTimeSpinBox::timeline() const {
        Q_D(const MusicTimeSpinBox);
        return d->timeline;
    }

    PersistentMusicTime MusicTimeSpinBox::maximum() const {
        Q_D(const MusicTimeSpinBox);
        return d->timeline->create(0, 0, QSpinBox::maximum());
    }

    void MusicTimeSpinBox::setMaximum(const PersistentMusicTime &maximum) {
        QSpinBox::setMaximum(maximum.totalTick());
    }

    PersistentMusicTime MusicTimeSpinBox::minimum() const {
        Q_D(const MusicTimeSpinBox);
        return d->timeline->create(0, 0, QSpinBox::minimum());
    }

    void MusicTimeSpinBox::setMinimum(const PersistentMusicTime &minimum) {
        QSpinBox::setMinimum(minimum.totalTick());
    }

    void MusicTimeSpinBox::setRange(const PersistentMusicTime &minimum,
                                    const PersistentMusicTime &maximum) {
        QSpinBox::setRange(minimum.totalTick(), maximum.totalTick());
    }

    int MusicTimeSpinBox::fieldWidth(MusicTimeSpinBox::FieldType fieldType) const {
        Q_D(const MusicTimeSpinBox);
        switch (fieldType) {
            case Measure:
                return d->measureWidth;
            case Beat:
                return d->beatWidth;
            case Tick:
                return d->tickWidth;
        }
        return 0;
    }

    void MusicTimeSpinBox::setFieldWidth(MusicTimeSpinBox::FieldType fieldType, int width) {
        Q_D(MusicTimeSpinBox);
        switch (fieldType) {
            case Measure:
                d->measureWidth = width;
                break;
            case Beat:
                d->beatWidth = width;
                break;
            case Tick:
                d->tickWidth = width;
                break;
        }
    }

    PersistentMusicTime MusicTimeSpinBox::value() const {
        Q_D(const MusicTimeSpinBox);
        return d->timeline->create(0, 0, QSpinBox::value());
    }

    void MusicTimeSpinBox::setValue(const PersistentMusicTime &value) {
        QSpinBox::setValue(value.totalTick());
    }

    QValidator::State MusicTimeSpinBox::validate(QString &input, int &pos) const {
        Q_D(const MusicTimeSpinBox);
        if (d->timeline->create(input).toString(d->measureWidth, d->beatWidth, d->tickWidth) ==
            input)
            return QValidator::Acceptable;
        else
            return QValidator::Intermediate;
    }

    void MusicTimeSpinBox::fixup(QString &input) const {
        Q_D(const MusicTimeSpinBox);
        bool ok;
        auto ret = d->timeline->create(input, &ok).toString(d->measureWidth, d->beatWidth, d->tickWidth);
        if (ok)
            input = ret;
    }

    int MusicTimeSpinBox::valueFromText(const QString &text) const {
        Q_D(const MusicTimeSpinBox);
        return d->timeline->create(text).totalTick();
    }

    QString MusicTimeSpinBox::textFromValue(int val) const {
        Q_D(const MusicTimeSpinBox);
        return d->timeline->create(0, 0, val).toString(d->measureWidth, d->beatWidth, d->tickWidth);
    }

}