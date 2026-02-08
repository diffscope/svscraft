#include "ColorUtils.h"

#include <algorithm>

#include <QColor>

namespace SVS {

    static inline double toLinear(double c) {
        return c <= 0.04045 ? c / 12.92 : std::pow((c + 0.055) / 1.055, 2.4);
    }

    static inline double fromLinear(double c) {
        return (c <= 0.0031308) ? (12.92 * c) : (1.055 * std::pow(c, 1.0 / 2.4) - 0.055);
    }

    static inline double toLuminance(const QColor &color) {
        return 0.2126 * toLinear(color.redF()) + 0.7152 * toLinear(color.greenF()) + 0.0722 * toLinear(color.blueF());
    }

    static inline double contrast(double luminance1, double luminance2) {
        return (std::max(luminance1, luminance2) + 0.05) / (std::min(luminance1, luminance2) + 0.05);
    }

    QColor ColorUtils::selectHighestContrastColor(const QColor &baseColor,
                                                  const QList<QColor> &availableColors) {
        auto l = toLuminance(baseColor);
        return *std::ranges::max_element(availableColors, [l](const auto &c1, const auto &c2) {
            auto l1 = toLuminance(c1);
            auto l2 = toLuminance(c2);
            return contrast(l1, l) < contrast(l2, l);
        });
    }

    ColorUtils::OkLab ColorUtils::toOkLab(const QColor &color) {
        double rl = toLinear(color.redF());
        double gl = toLinear(color.greenF());
        double bl = toLinear(color.blueF());

        double l = 0.4122214708 * rl + 0.5363325363 * gl + 0.0514459929 * bl;
        double m = 0.2119034982 * rl + 0.6806995451 * gl + 0.1073969566 * bl;
        double s = 0.0883024619 * rl + 0.2817188376 * gl + 0.6299787005 * bl;

        l = std::cbrt(l);
        m = std::cbrt(m);
        s = std::cbrt(s);

        return {
            0.2104542553 * l + 0.7936177850 * m - 0.0040720468 * s,
            1.9779984951 * l - 2.4285922050 * m + 0.4505937099 * s,
            0.0259040371 * l + 0.7827717662 * m - 0.8086757660 * s,
            color.alphaF()
        };
    }

    QColor ColorUtils::fromOkLab(const OkLab &lab) {
        double l = lab.L + 0.3963377774 * lab.a + 0.2158037573 * lab.b;
        double m = lab.L - 0.1055613458 * lab.a - 0.0638541728 * lab.b;
        double s = lab.L - 0.0894841775 * lab.a - 1.2914855480 * lab.b;

        l = l * l * l;
        m = m * m * m;
        s = s * s * s;

        double rl = +4.0767416621 * l - 3.3077115913 * m + 0.2309699292 * s;
        double gl = -1.2684380046 * l + 2.6097574011 * m - 0.3413193965 * s;
        double bl = -0.0041960863 * l - 0.7034186147 * m + 1.7076147010 * s;

        return QColor::fromRgbF(
            static_cast<float>(std::clamp(fromLinear(rl), 0.0, 1.0)),
            static_cast<float>(std::clamp(fromLinear(gl), 0.0, 1.0)),
            static_cast<float>(std::clamp(fromLinear(bl), 0.0, 1.0)),
            static_cast<float>(lab.alpha)
        );
    }
}
