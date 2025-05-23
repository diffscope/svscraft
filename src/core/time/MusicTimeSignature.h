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

#ifndef SVSCRAFT_MUSICTIMESIGNATURE_H
#define SVSCRAFT_MUSICTIMESIGNATURE_H

#include <QMetaType>

#include <SVSCraftCore/SVSCraftCoreGlobal.h>

namespace SVS {

    class SVSCRAFT_CORE_EXPORT MusicTimeSignature {
        Q_GADGET
        Q_PROPERTY(int numerator READ numerator CONSTANT)
        Q_PROPERTY(int denominator READ denominator CONSTANT)
        Q_PROPERTY(bool isValid READ isValid CONSTANT)
    public:
        constexpr inline MusicTimeSignature() : MusicTimeSignature(4, 4) {
        }
        constexpr inline MusicTimeSignature(int numerator, int denominator) : m_numerator(numerator), m_denominator(denominator) {
        }

        constexpr inline int numerator() const {
            return m_numerator;
        }
        constexpr inline int denominator() const {
            return m_denominator;
        }
        constexpr inline void setNumerator(int numerator) {
            m_numerator = numerator;
        }
        constexpr inline void setDenominator(int denominator) {
            m_denominator = denominator;
        }

        constexpr bool isValid() const {
            return m_numerator > 0 && m_denominator > 0 && !(m_denominator & (m_denominator - 1));
        }

        Q_INVOKABLE constexpr int ticksPerBar(int resolution) const {
            return resolution * m_numerator * 4 / m_denominator;
        }

        Q_INVOKABLE constexpr int ticksPerBeat(int resolution) const {
            return resolution * 4 / denominator();
        }

        Q_INVOKABLE QString toString() const;

        constexpr inline bool operator==(const MusicTimeSignature &t) const = default;

        friend SVSCRAFT_CORE_EXPORT QDebug operator<<(QDebug debug, const MusicTimeSignature &t);

    private:
        int m_numerator;
        int m_denominator;
    };

}

#endif // SVSCRAFT_MUSICTIMESIGNATURE_H
