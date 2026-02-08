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
#include <SVSCraftGui/ColorUtils.h>

namespace SVS {

    enum FilterType {
        Alpha,
        Saturation,
        Value,
        HslSaturation,
        Lightness,
        Lighter,
        TopBlend,
        BottomBlend,
        OkLabLighter,
    };

    static QColor filterAlpha(const QColor &color, qintptr d) {
        auto alphaFactor = std::bit_cast<double>(d);
        auto c = color;
        c.setAlphaF(color.alphaF() * alphaFactor);
        return c;
    }

    static QColor filterSaturation(const QColor &color, qintptr d) {
        auto saturationFactor = std::bit_cast<double>(d);
        return QColor::fromHsvF(color.hueF(), color.saturationF() * saturationFactor, color.valueF(), color.alphaF());
    }

    static QColor filterValue(const QColor &color, qintptr d) {
        auto valueFactor = std::bit_cast<double>(d);
        return QColor::fromHsvF(color.hueF(), color.saturationF(), color.valueF() * valueFactor, color.alphaF());
    }

    static QColor filterHslSaturation(const QColor &color, qintptr d) {
        auto saturationFactor = std::bit_cast<double>(d);
        return QColor::fromHslF(color.hslHueF(), color.hslSaturationF() * saturationFactor, color.lightnessF(), color.alphaF());
    }

    static QColor filterLightness(const QColor &color, qintptr d) {
        auto lightnessFactor = std::bit_cast<double>(d);
        return QColor::fromHslF(color.hslHueF(), color.hslSaturationF(), color.lightnessF() * lightnessFactor, color.alphaF());
    }

    static QColor filterLighter(const QColor &color, qintptr d) {
        auto factor = static_cast<int>(d);
        return color.lighter(factor);
    }

    static QColor blendTop(const QColor &color, qintptr d) {
        auto blendColor = static_cast<QRgb>(d);
        return ColorBlender::blend<ColorBlender::Normal>(blendColor, color.rgba());
    }

    static QColor blendBottom(const QColor &color, qintptr d) {
        auto blendColor = static_cast<QRgb>(d);
        return ColorBlender::blend<ColorBlender::Normal>(color.rgba(), blendColor);
    }

    static QColor filterOkLabLighter(const QColor &color, qintptr d) {
        auto factor = std::bit_cast<double>(d);
        auto oklab = ColorUtils::toOkLab(color);
        oklab.L = std::clamp(oklab.L * factor, 0.0, 100.0);
        return ColorUtils::fromOkLab(oklab);
    }

    using Filter = QColor (*)(const QColor &, qintptr);

    static Filter m_filters[] = {
        &filterAlpha,     &filterSaturation, &filterValue, &filterHslSaturation,
        &filterLightness, &filterLighter,    &blendTop,    &blendBottom,
        &filterOkLabLighter,
    };

    QColor AbstractColorFilter::apply(const QColor &color) const {
        return m_filters[type](color, d);
    }
    QDataStream &operator<<(QDataStream &stream, const AbstractColorFilter &filter) {
        stream << filter.d;
        stream << filter.type;
        return stream;
    }
    QDataStream &operator>>(QDataStream &stream, AbstractColorFilter &filter) {
        stream >> filter.d;
        stream >> filter.type;
        return stream;
    }

    AlphaColorFilter::AlphaColorFilter(double alphaFactor) : AbstractColorFilter(std::bit_cast<qintptr>(alphaFactor), Alpha) {}

    SaturationColorFilter::SaturationColorFilter(double saturationFactor) : AbstractColorFilter(std::bit_cast<qintptr>(saturationFactor), Saturation) {}

    ValueColorFilter::ValueColorFilter(double valueFactor) : AbstractColorFilter(std::bit_cast<qintptr>(valueFactor), Value) {}

    HslSaturationColorFilter::HslSaturationColorFilter(double saturationFactor) : AbstractColorFilter(std::bit_cast<qintptr>(saturationFactor), HslSaturation) {}

    LightnessColorFilter::LightnessColorFilter(double lightnessFactor) : AbstractColorFilter(std::bit_cast<qintptr>(lightnessFactor), Lightness) {
    }

    LighterColorChange::LighterColorChange(int factor)
        : AbstractColorFilter(static_cast<qintptr>(factor), Lighter) {
    }

    OkLabLighterColorChange::OkLabLighterColorChange(double factor)
        : AbstractColorFilter(std::bit_cast<qintptr>(factor), OkLabLighter){
    }

    TopBlendColorFilter::TopBlendColorFilter(const QColor &blendColor) : AbstractColorFilter(static_cast<qintptr>(blendColor.rgba()), TopBlend) {}

    BottomBlendColorFilter::BottomBlendColorFilter(const QColor &blendColor) : AbstractColorFilter(static_cast<qintptr>(blendColor.rgba()), BottomBlend) {}

    QColor ColorChange::apply(const QColor &color) const {
        auto c = color;
        for (const auto &f : *this) {
            c = f.apply(c);
        }
        return c;
    }
}

#include "moc_ColorChange.cpp"
