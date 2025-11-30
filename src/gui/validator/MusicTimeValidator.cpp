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

#include "MusicTimeValidator.h"
#include "MusicTimeValidator_p.h"

#include <SVSCraftCore/MusicTime.h>
#include <SVSCraftCore/MusicTimeline.h>

namespace SVS {

    MusicTimeValidator::MusicTimeValidator(QObject *parent) 
        : QValidator(parent), d_ptr(new MusicTimeValidatorPrivate) {
        Q_D(MusicTimeValidator);
        d->q_ptr = this;
    }
    
    MusicTimeValidator::MusicTimeValidator(int minimum, int maximum, QObject *parent) 
        : QValidator(parent), d_ptr(new MusicTimeValidatorPrivate) {
        Q_D(MusicTimeValidator);
        d->q_ptr = this;
        d->top = maximum;
        d->bottom = minimum;
    }
    
    MusicTimeValidator::~MusicTimeValidator() = default;
    
    int MusicTimeValidator::top() const {
        Q_D(const MusicTimeValidator);
        return d->top;
    }
    
    void MusicTimeValidator::setTop(int top) {
        setRange(bottom(), top);
    }
    
    int MusicTimeValidator::bottom() const {
        Q_D(const MusicTimeValidator);
        return d->bottom;
    }
    
    void MusicTimeValidator::setBottom(int bottom) {
        setRange(bottom, top());
    }
    
    int MusicTimeValidator::measureWidth() const {
        Q_D(const MusicTimeValidator);
        return d->measureWidth;
    }
    
    void MusicTimeValidator::setMeasureWidth(int measureWidth) {
        Q_D(MusicTimeValidator);
        if (d->measureWidth == measureWidth)
            return;
        d->measureWidth = measureWidth;
        emit measureWidthChanged(d->measureWidth);
        emit changed();
    }
    
    int MusicTimeValidator::beatWidth() const {
        Q_D(const MusicTimeValidator);
        return d->beatWidth;
    }
    
    void MusicTimeValidator::setBeatWidth(int beatWidth) {
        Q_D(MusicTimeValidator);
        if (d->beatWidth == beatWidth)
            return;
        d->beatWidth = beatWidth;
        emit beatWidthChanged(d->beatWidth);
        emit changed();
    }
    
    int MusicTimeValidator::tickWidth() const {
        Q_D(const MusicTimeValidator);
        return d->tickWidth;
    }
    
    void MusicTimeValidator::setTickWidth(int tickWidth) {
        Q_D(MusicTimeValidator);
        if (d->tickWidth == tickWidth)
            return;
        d->tickWidth = tickWidth;
        emit tickWidthChanged(d->tickWidth);
        emit changed();
    }
    
    MusicTimeline *MusicTimeValidator::timeline() const {
        Q_D(const MusicTimeValidator);
        return d->timeline;
    }
    
    void MusicTimeValidator::setTimeline(MusicTimeline *timeline) {
        Q_D(MusicTimeValidator);
        if (d->timeline == timeline)
            return;
        d->timeline = timeline;
        emit timelineChanged(d->timeline);
        emit changed();
    }
    
    void MusicTimeValidator::setRange(int bottom, int top) {
        Q_D(MusicTimeValidator);
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
    
    QValidator::State MusicTimeValidator::validate(QString &input, int &) const {
        Q_D(const MusicTimeValidator);
        
        if (!d->timeline) {
            return Invalid;
        }
        
        if (d->timeline->create(input).toString(d->measureWidth, d->beatWidth, d->tickWidth) == input) {
            return Acceptable;
        } else {
            return Intermediate;
        }
    }
    
    void MusicTimeValidator::fixup(QString &input) const {
        Q_D(const MusicTimeValidator);
        
        if (!d->timeline) {
            return;
        }
        
        bool ok;
        auto ret = d->timeline->create(input, &ok).toString(d->measureWidth, d->beatWidth, d->tickWidth);
        if (ok) {
            input = ret;
        }
    }

}

#include "moc_MusicTimeValidator.cpp"
