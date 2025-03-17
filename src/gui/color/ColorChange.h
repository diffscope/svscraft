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

#ifndef SVSCRAFT_COLORCHANGE_H
#define SVSCRAFT_COLORCHANGE_H

#include <QColor>
#include <QMetaObject>

#include <SVSCraftGui/SVSCraftGuiGlobal.h>

namespace SVS {

    class SVSCRAFT_GUI_EXPORT ColorChange {
        Q_GADGET
        Q_PROPERTY(QColor topBlend READ topBlend CONSTANT)
        Q_PROPERTY(float alphaFactor READ alphaFactor CONSTANT)
        Q_PROPERTY(QColor bottomBlend READ bottomBlend CONSTANT)
    public:
        constexpr ColorChange(const QColor &topBlend = {}, float alphaFactor = 1.0f, const QColor &bottomBlend = {}) : m_topBlend(topBlend), m_bottomBlend(bottomBlend), m_alphaFactor(alphaFactor) {
        }

        constexpr QColor topBlend() const {
            return m_topBlend;
        }

        constexpr float alphaFactor() const {
            return m_alphaFactor;
        }

        constexpr QColor bottomBlend() const {
            return m_bottomBlend;
        }

        bool operator==(const ColorChange &) const = default;

        Q_INVOKABLE QColor apply(const QColor &color) const;

    private:
        QColor m_topBlend;
        QColor m_bottomBlend;
        float m_alphaFactor;
    };

}

#endif // SVSCRAFT_COLORCHANGE_H
