#include "LongTimeValidator.h"

#include <QRegularExpression>

#include <SVSCraftCore/LongTime.h>

namespace SVS {
    LongTimeValidator::LongTimeValidator(QObject *parent) : QIntValidator(parent) {
    }
    LongTimeValidator::LongTimeValidator(int minimum, int maximum, QObject *parent) : QIntValidator(minimum, maximum, parent) {
    }
    QValidator::State LongTimeValidator::validate(QString &s, int &) const {
        static QRegularExpression rx(R"(^(-?)(\d*)([:\x{ff1a}]?)(\d*)([:\x{ff1a}]?)(\d*)[.\x{3002}\x{ff0e}]?(\d*)$)");
        static QRegularExpression rxPositive(R"(^(\d*)([:\x{ff1a}]?)(\d*)([:\x{ff1a}]?)(\d*)[.\x{3002}\x{ff0e}]?(\d*)$)");
        auto match = (bottom() < 0 ? rx : rxPositive).match(s);
        if (!match.hasMatch())
            return Invalid;
        if (s != qBound(LongTime(bottom()), LongTime::fromString(s), LongTime(top())).toString())
            return Intermediate;
        return Acceptable;
    }
    void LongTimeValidator::fixup(QString &s) const {
        s = qBound(LongTime(bottom()), LongTime::fromString(s), LongTime(top())).toString();
    }
}