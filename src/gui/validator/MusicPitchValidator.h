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

#ifndef SVSCRAFT_MUSICPITCHVALIDATOR_H
#define SVSCRAFT_MUSICPITCHVALIDATOR_H

#include <QIntValidator>

#include <SVSCraftCore/MusicPitch.h>
#include <SVSCraftGui/SVSCraftGuiGlobal.h>

namespace SVS {

    class SVSCRAFT_GUI_EXPORT MusicPitchValidator : public QIntValidator {
        Q_OBJECT
        Q_PROPERTY(MusicPitch::Accidental accidental READ accidental WRITE setAccidental NOTIFY accidentalChanged)

    public:
        explicit MusicPitchValidator(QObject *parent = nullptr);
        MusicPitchValidator(int minimum, int maximum, QObject *parent = nullptr);
        ~MusicPitchValidator() override;

        MusicPitch::Accidental accidental() const;
        void setAccidental(MusicPitch::Accidental value);

        void fixup(QString &) const override;
        State validate(QString &, int &) const override;

    signals:
        void accidentalChanged(MusicPitch::Accidental value);

    private:
        MusicPitch::Accidental m_accidental;

    };

}

#endif // SVSCRAFT_MUSICPITCHVALIDATOR_H
