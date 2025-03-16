#include "MusicPitchValidator.h"

#include <QRegularExpression>

namespace SVS {
    MusicPitchValidator::MusicPitchValidator(QObject *parent) : QIntValidator(parent), m_accidental(MusicPitch::Flat) {
    }
    MusicPitchValidator::MusicPitchValidator(int minimum, int maximum, QObject *parent) : QIntValidator(minimum, maximum, parent), m_accidental(MusicPitch::Flat) {
    }
    MusicPitchValidator::~MusicPitchValidator() = default;
    MusicPitch::Accidental MusicPitchValidator::accidental() const {
        return m_accidental;
    }
    void MusicPitchValidator::setAccidental(MusicPitch::Accidental value) {
        if (m_accidental != value) {
            m_accidental = value;
            emit accidentalChanged(value);
        }
    }
    void MusicPitchValidator::fixup(QString &s) const {
        s = qBound(MusicPitch(qBound(-12, bottom(), 127)), MusicPitch::fromString(s), MusicPitch(qBound(-12, top(), 127))).toString(m_accidental);
    }
    QValidator::State MusicPitchValidator::validate(QString &s, int &) const {
        static QString rxTemplate = R"(^([A-G])([b#]*)(%1)?$)";
        if (s.isEmpty())
            return Intermediate;
        QStringList octaveTypes;
        if (bottom() < 0)
            octaveTypes.append(R"(\?)");
        if (bottom() < MusicPitch(MusicPitch::C, 10).pitch() && top() >= 0) {
            octaveTypes.append(QString("[%1-%2]").arg(MusicPitch(bottom()).octave()).arg(qMin(9, MusicPitch(top()).octave())));
        }
        if (top() >= MusicPitch(MusicPitch::C, 10).pitch())
            octaveTypes.append(R"(1?0?)");
        QRegularExpression rx(rxTemplate.arg(octaveTypes.join("|")));
        auto match = rx.match(s);
        if (!match.hasMatch())
            return Invalid;
        auto correct = qBound(MusicPitch(qBound(-12, bottom(), 127)), MusicPitch::fromString(s), MusicPitch(qBound(-12, top(), 127))).toString(m_accidental);
        if (s == correct)
            return Acceptable;
        return Intermediate;
    }
}