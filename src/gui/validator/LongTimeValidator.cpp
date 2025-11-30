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

#include <limits>

#include <QRegularExpression>

#include <SVSCraftCore/LongTime.h>

namespace SVS {
    LongTimeValidator::LongTimeValidator(QObject *parent) 
        : QValidator(parent), m_bottom(LongTime()), m_top(LongTime(std::numeric_limits<int>::max())) {
    }
    
    LongTimeValidator::LongTimeValidator(const LongTime &minimum, const LongTime &maximum, QObject *parent) 
        : QValidator(parent), m_bottom(minimum), m_top(maximum) {
    }
    
    LongTime LongTimeValidator::bottom() const {
        return m_bottom;
    }
    
    void LongTimeValidator::setBottom(const LongTime &bottom) {
        setRange(bottom, top());
    }
    
    LongTime LongTimeValidator::top() const {
        return m_top;
    }
    
    void LongTimeValidator::setTop(const LongTime &top) {
        setRange(bottom(), top);
    }
    
    void LongTimeValidator::setRange(const LongTime &bottom, const LongTime &top) {
        bool rangeChanged = false;
        if (m_bottom != bottom) {
            m_bottom = bottom;
            rangeChanged = true;
            emit bottomChanged(m_bottom);
        }

        if (m_top != top) {
            m_top = top;
            rangeChanged = true;
            emit topChanged(m_top);
        }

        if (rangeChanged)
            emit changed();
    }
    
    QValidator::State LongTimeValidator::validate(QString &s, int &) const {
        static QRegularExpression rx(R"(^(-?)(\d*)([:\x{ff1a}]?)(\d*)([:\x{ff1a}]?)(\d*)[.\x{3002}\x{ff0e}]?(\d*)$)");
        static QRegularExpression rxPositive(R"(^(\d*)([:\x{ff1a}]?)(\d*)([:\x{ff1a}]?)(\d*)[.\x{3002}\x{ff0e}]?(\d*)$)");
        auto match = (m_bottom.negative() ? rx : rxPositive).match(s);
        if (!match.hasMatch())
            return Invalid;
        if (s != qBound(m_bottom, LongTime::fromString(s), m_top).toString())
            return Intermediate;
        return Acceptable;
    }
    
    void LongTimeValidator::fixup(QString &s) const {
        s = qBound(m_bottom, LongTime::fromString(s), m_top).toString();
    }
}
