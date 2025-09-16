/******************************************************************************
 * Copyright (c) 2025 OpenVPI                                                 *
 *                                                                            *
 * This file is part of SVSCraft                                              *
 *                                                                            *
 * SVSCraft is free software: you can redistribute it and/or modify it under  *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License, or (at your     *
 * option) any later version.                                                 *
 *                                                                            *
 * SVSCraft is distributed in the hope that it will be useful, but WITHOUT    *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public        *
 * License for more details.                                                  *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with SVSCraft. If not, see <https://www.gnu.org/licenses/>.          *
 ******************************************************************************/

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
