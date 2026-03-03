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
        Q_GADGET
        Q_PROPERTY(int mask READ mask CONSTANT)
    public:
        constexpr explicit MusicMode(int mask = 0) : m_mask(mask & 0xFFF) {
        }

        constexpr int mask() const {
            return m_mask;
        }

        Q_INVOKABLE QList<MusicPitch> scale(MusicPitch tonic) const;

        Q_INVOKABLE constexpr int translateMask(int sourceTonality, int targetTonality) const {
            int delta = (targetTonality - sourceTonality + 12) % 12;
            int deltaMask = (1 << delta) - 1;
            return (m_mask & deltaMask) << (12 - delta) | (m_mask >> delta);
        }

    private:
        int m_mask;
    };
    
}

Q_DECLARE_METATYPE(SVS::MusicMode)

#endif // SVSCRAFT_MUSICMODE_H
