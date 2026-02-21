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

#include "MusicTimeOffsetValidator.h"
#include "MusicTimeOffsetValidator_p.h"

#include <SVSCraftCore/MusicTime.h>

namespace SVS {

    MusicTimeOffsetValidator::MusicTimeOffsetValidator(QObject *parent) 
        : QValidator(parent), d_ptr(new MusicTimeOffsetValidatorPrivate) {
        Q_D(MusicTimeOffsetValidator);
        d->q_ptr = this;
    }
    
    MusicTimeOffsetValidator::MusicTimeOffsetValidator(int minimum, int maximum, QObject *parent) 
        : QValidator(parent), d_ptr(new MusicTimeOffsetValidatorPrivate) {
        Q_D(MusicTimeOffsetValidator);
        d->q_ptr = this;
        d->top = maximum;
        d->bottom = minimum;
    }
    
    MusicTimeOffsetValidator::~MusicTimeOffsetValidator() = default;
    
    int MusicTimeOffsetValidator::top() const {
        Q_D(const MusicTimeOffsetValidator);
        return d->top;
    }
    
    void MusicTimeOffsetValidator::setTop(int top) {
        setRange(bottom(), top);
    }
    
    int MusicTimeOffsetValidator::bottom() const {
        Q_D(const MusicTimeOffsetValidator);
        return d->bottom;
    }
    
    void MusicTimeOffsetValidator::setBottom(int bottom) {
        setRange(bottom, top());
    }
    
    int MusicTimeOffsetValidator::quarterNoteWidth() const {
        Q_D(const MusicTimeOffsetValidator);
        return d->quarterNoteWidth;
    }
    
    void MusicTimeOffsetValidator::setQuarterNoteWidth(int quarterNoteWidth) {
        Q_D(MusicTimeOffsetValidator);
        if (d->quarterNoteWidth == quarterNoteWidth)
            return;
        d->quarterNoteWidth = quarterNoteWidth;
        emit quarterNoteWidthChanged(d->quarterNoteWidth);
        emit changed();
    }
    
    int MusicTimeOffsetValidator::tickWidth() const {
        Q_D(const MusicTimeOffsetValidator);
        return d->tickWidth;
    }
    
    void MusicTimeOffsetValidator::setTickWidth(int tickWidth) {
        Q_D(MusicTimeOffsetValidator);
        if (d->tickWidth == tickWidth)
            return;
        d->tickWidth = tickWidth;
        emit tickWidthChanged(d->tickWidth);
        emit changed();
    }
    
    void MusicTimeOffsetValidator::setRange(int bottom, int top) {
        Q_D(MusicTimeOffsetValidator);
        bool rangeChanged = false;
        if (d->bottom != bottom) {
            d->bottom = bottom;
            rangeChanged = true;
            emit bottomChanged(d->bottom);
        }

        if (d->top != top) {
            d->top = top;
            rangeChanged = true;
            emit topChanged(d->top);
        }

        if (rangeChanged)
            emit changed();
    }
    
    QValidator::State MusicTimeOffsetValidator::validate(QString &input, int &) const {
        Q_D(const MusicTimeOffsetValidator);
        
        bool ok;
        auto offset = MusicTimeOffset::fromString(input, &ok);
        
        if (!ok) {
            return Intermediate;
        }
        
        if (offset.toString(d->quarterNoteWidth, d->tickWidth) == input) {
            return Acceptable;
        } else {
            return Intermediate;
        }
    }
    
    void MusicTimeOffsetValidator::fixup(QString &input) const {
        Q_D(const MusicTimeOffsetValidator);
        
        bool ok;
        auto offset = MusicTimeOffset::fromString(input, &ok);
        if (ok) {
            input = offset.toString(d->quarterNoteWidth, d->tickWidth);
        }
    }

}

#include "moc_MusicTimeOffsetValidator.cpp"
