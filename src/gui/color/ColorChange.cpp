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

#include "ColorChange.h"

#include <QColor>

#include <SVSCraftGui/ColorBlender.h>

namespace SVS {
    QColor ColorChange::apply(const QColor &color) const {
        auto c = color;
        c.setAlphaF(c.alphaF() * m_alphaFactor);
        if (topBlend().isValid()) {
            c = QColor::fromRgba(ColorBlender::blend<ColorBlender::Normal>(topBlend().rgba(), c.rgba()));
        }
        if (bottomBlend().isValid()) {
            c = QColor::fromRgba(ColorBlender::blend<ColorBlender::Normal>(c.rgba(), bottomBlend().rgba()));
        }
        return c;
    }
}

#include "moc_ColorChange.cpp"