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

#ifndef SVSCRAFT_MUSICTIMEVALIDATOR_H
#define SVSCRAFT_MUSICTIMEVALIDATOR_H

#include <QValidator>

#include <SVSCraftGui/SVSCraftGuiGlobal.h>

namespace SVS {

    class MusicTimeline;
    class MusicTimeValidatorPrivate;

    class SVSCRAFT_GUI_EXPORT MusicTimeValidator : public QValidator {
        Q_OBJECT
        Q_DECLARE_PRIVATE(MusicTimeValidator)
        
        Q_PROPERTY(int top READ top WRITE setTop NOTIFY topChanged)
        Q_PROPERTY(int bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
        Q_PROPERTY(int measureWidth READ measureWidth WRITE setMeasureWidth NOTIFY measureWidthChanged)
        Q_PROPERTY(int beatWidth READ beatWidth WRITE setBeatWidth NOTIFY beatWidthChanged)
        Q_PROPERTY(int tickWidth READ tickWidth WRITE setTickWidth NOTIFY tickWidthChanged)
        Q_PROPERTY(MusicTimeline *timeline READ timeline WRITE setTimeline NOTIFY timelineChanged)

    public:
        explicit MusicTimeValidator(QObject *parent = nullptr);
        MusicTimeValidator(int minimum, int maximum, QObject *parent = nullptr);
        ~MusicTimeValidator() override;

        int top() const;
        void setTop(int top);
        
        int bottom() const;
        void setBottom(int bottom);
        
        int measureWidth() const;
        void setMeasureWidth(int measureWidth);
        
        int beatWidth() const;
        void setBeatWidth(int beatWidth);
        
        int tickWidth() const;
        void setTickWidth(int tickWidth);
        
        MusicTimeline *timeline() const;
        void setTimeline(MusicTimeline *timeline);

        void setRange(int bottom, int top);

        State validate(QString &, int &) const override;
        void fixup(QString &) const override;

    Q_SIGNALS:
        void topChanged(int top);
        void bottomChanged(int bottom);
        void measureWidthChanged(int measureWidth);
        void beatWidthChanged(int beatWidth);
        void tickWidthChanged(int tickWidth);
        void timelineChanged(MusicTimeline *timeline);

    private:
        QScopedPointer<MusicTimeValidatorPrivate> d_ptr;
    };

}

#endif // SVSCRAFT_MUSICTIMEVALIDATOR_H
