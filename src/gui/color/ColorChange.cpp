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
#include <QDebug>

#include <SVSCraftGui/ColorBlender.h>

namespace SVS {

    static QColor filterAlpha(const QColor &color, qintptr d) {
        auto alphaFactor = std::bit_cast<double>(d);
        auto c = color;
        c.setAlphaF(color.alphaF() * alphaFactor);
        return c;
    }
    AlphaColorFilter::AlphaColorFilter(double alphaFactor) : AbstractColorFilter(std::bit_cast<qintptr>(alphaFactor), &filterAlpha) {}

    static QColor filterSaturation(const QColor &color, qintptr d) {
        auto saturationFactor = std::bit_cast<double>(d);
        return QColor::fromHsvF(color.hueF(), color.saturationF() * saturationFactor, color.valueF(), color.alphaF());
    }
    SaturationColorFilter::SaturationColorFilter(double saturationFactor) : AbstractColorFilter(std::bit_cast<qintptr>(saturationFactor), &filterSaturation) {}

    static QColor filterValue(const QColor &color, qintptr d) {
        auto valueFactor = std::bit_cast<double>(d);
        return QColor::fromHsvF(color.hueF(), color.saturationF(), color.valueF() * valueFactor, color.alphaF());
    }
    ValueColorFilter::ValueColorFilter(double valueFactor) : AbstractColorFilter(std::bit_cast<qintptr>(valueFactor), &filterValue) {}

    static QColor filterHslSaturation(const QColor &color, qintptr d) {
        auto saturationFactor = std::bit_cast<double>(d);
        return QColor::fromHslF(color.hslHueF(), color.hslSaturationF() * saturationFactor, color.lightnessF(), color.alphaF());
    }
    HslSaturationColorFilter::HslSaturationColorFilter(double saturationFactor) : AbstractColorFilter(std::bit_cast<qintptr>(saturationFactor), &filterHslSaturation) {}

    static QColor filterLightness(const QColor &color, qintptr d) {
        auto lightnessFactor = std::bit_cast<double>(d);
        return QColor::fromHslF(color.hslHueF(), color.hslSaturationF(), color.lightnessF() * lightnessFactor, color.alphaF());
    }
    LightnessColorFilter::LightnessColorFilter(double lightnessFactor) : AbstractColorFilter(std::bit_cast<qintptr>(lightnessFactor), &filterLightness) {
    }

    static QColor filterLighter(const QColor &color, qintptr d) {
        auto factor = static_cast<int>(d);
        return color.lighter(factor);
    }
    LighterColorChange::LighterColorChange(int factor) : AbstractColorFilter(static_cast<qintptr>(factor), &filterLighter) {
    }

    static QColor blendTop(const QColor &color, qintptr d) {
        auto blendColor = static_cast<QRgb>(d);
        return ColorBlender::blend<ColorBlender::Normal>(blendColor, color.rgba());
    }
    TopBlendColorFilter::TopBlendColorFilter(const QColor &blendColor) : AbstractColorFilter(static_cast<qintptr>(blendColor.rgba()), &blendTop) {}

    static QColor blendBottom(const QColor &color, qintptr d) {
        auto blendColor = static_cast<QRgb>(d);
        return ColorBlender::blend<ColorBlender::Normal>(color.rgba(), blendColor);
    }
    BottomBlendColorFilter::BottomBlendColorFilter(const QColor &blendColor) : AbstractColorFilter(static_cast<qintptr>(blendColor.rgba()), &blendBottom) {}

    QColor ColorChange::apply(const QColor &color) const {
        auto c = color;
        for (const auto &f : *this) {
            c = f.apply(c);
        }
        return c;
    }
}

#include "moc_ColorChange.cpp"