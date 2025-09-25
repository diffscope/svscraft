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

    struct SVSCRAFT_GUI_EXPORT AbstractColorFilter {
        AbstractColorFilter() = default;

        QColor apply(const QColor &color) const;

        bool operator==(const AbstractColorFilter &) const = default;

        SVSCRAFT_GUI_EXPORT friend QDataStream &operator<<(QDataStream &stream, const AbstractColorFilter &filter);
        SVSCRAFT_GUI_EXPORT friend QDataStream &operator>>(QDataStream &stream, AbstractColorFilter &filter);

    protected:
        explicit AbstractColorFilter(qintptr d, int type) : d(d), type(type) {}
    private:
        qintptr d{};
        int type{};
    };

    struct SVSCRAFT_GUI_EXPORT AlphaColorFilter : AbstractColorFilter {
    public:
        AlphaColorFilter(double alphaFactor);
    };

    struct SVSCRAFT_GUI_EXPORT SaturationColorFilter : AbstractColorFilter {
        SaturationColorFilter(double saturationFactor);
    };

    struct SVSCRAFT_GUI_EXPORT ValueColorFilter : AbstractColorFilter {
        ValueColorFilter(double valueFactor);
    };

    struct SVSCRAFT_GUI_EXPORT HslSaturationColorFilter : AbstractColorFilter {
        HslSaturationColorFilter(double saturationFactor);
    };

    struct SVSCRAFT_GUI_EXPORT LightnessColorFilter : AbstractColorFilter {
        LightnessColorFilter(double lightnessFactor);
    };

    struct SVSCRAFT_GUI_EXPORT LighterColorChange : AbstractColorFilter {
        LighterColorChange(int factor);
    };

    struct SVSCRAFT_GUI_EXPORT TopBlendColorFilter : AbstractColorFilter {
        TopBlendColorFilter(const QColor &blendColor);
    };

    struct SVSCRAFT_GUI_EXPORT BottomBlendColorFilter : AbstractColorFilter {
        BottomBlendColorFilter(const QColor &blendColor);
    };

    class SVSCRAFT_GUI_EXPORT ColorChange : public QList<AbstractColorFilter> {
        Q_GADGET

    public:
        ColorChange() = default;
        ColorChange(std::initializer_list<AbstractColorFilter> args) : QList(args) {}

        Q_INVOKABLE QColor apply(const QColor &color) const;
    };

}

template class SVSCRAFT_GUI_EXPORT QList<SVS::AbstractColorFilter>;

#endif // SVSCRAFT_COLORCHANGE_H
