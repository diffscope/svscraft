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

#ifndef SVSCRAFT_MUSICTIMEOFFSETVALIDATOR_H
#define SVSCRAFT_MUSICTIMEOFFSETVALIDATOR_H

#include <QValidator>

#include <SVSCraftGui/SVSCraftGuiGlobal.h>

namespace SVS {

    class MusicTimeOffsetValidatorPrivate;

    class SVSCRAFT_GUI_EXPORT MusicTimeOffsetValidator : public QValidator {
        Q_OBJECT
        Q_DECLARE_PRIVATE(MusicTimeOffsetValidator)
        
        Q_PROPERTY(int top READ top WRITE setTop NOTIFY topChanged)
        Q_PROPERTY(int bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
        Q_PROPERTY(int quarterNoteWidth READ quarterNoteWidth WRITE setQuarterNoteWidth NOTIFY quarterNoteWidthChanged)
        Q_PROPERTY(int tickWidth READ tickWidth WRITE setTickWidth NOTIFY tickWidthChanged)

    public:
        explicit MusicTimeOffsetValidator(QObject *parent = nullptr);
        MusicTimeOffsetValidator(int minimum, int maximum, QObject *parent = nullptr);
        ~MusicTimeOffsetValidator() override;

        int top() const;
        void setTop(int top);
        
        int bottom() const;
        void setBottom(int bottom);
        
        int quarterNoteWidth() const;
        void setQuarterNoteWidth(int quarterNoteWidth);
        
        int tickWidth() const;
        void setTickWidth(int tickWidth);

        void setRange(int bottom, int top);

        State validate(QString &, int &) const override;
        void fixup(QString &) const override;

    Q_SIGNALS:
        void topChanged(int top);
        void bottomChanged(int bottom);
        void quarterNoteWidthChanged(int quarterNoteWidth);
        void tickWidthChanged(int tickWidth);

    private:
        QScopedPointer<MusicTimeOffsetValidatorPrivate> d_ptr;
    };

}

#endif // SVSCRAFT_MUSICTIMEOFFSETVALIDATOR_H
