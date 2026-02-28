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

#ifndef SVSCRAFT_MUSICMODE_H
#define SVSCRAFT_MUSICMODE_H

#include <SVSCraftCore/MusicPitch.h>

namespace SVS {

    class SVSCRAFT_CORE_EXPORT MusicMode {
    public:
        constexpr explicit MusicMode(int mask) : m_mask(mask) {
        }

        constexpr int mask() const {
            return m_mask;
        }

        QList<MusicPitch> scale(MusicPitch tonic) const;

    private:
        int m_mask;
    };
    
}

Q_DECLARE_METATYPE(SVS::MusicMode)

#endif // SVSCRAFT_MUSICMODE_H
