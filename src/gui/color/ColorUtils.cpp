#include "ColorUtils.h"

#include <algorithm>

#include <QColor>

namespace SVS {

    static inline double toLinear(double c) {
        return c <= 0.04045 ? c / 12.92 : std::pow((c + 0.055) / 1.055, 2.4);
    }

    static inline double toLuminance(const QColor &color) {
        return 0.2126 * toLinear(color.redF()) + 0.7152 * toLinear(color.greenF()) + 0.0722 * toLinear(color.blueF());
    }

    static inline double contrast(double luminance1, double luminance2) {
        return (std::max(luminance1, luminance2) + 0.05) / (std::min(luminance1, luminance2) + 0.05);
    }

    QColor ColorUtils::selectHighestContrastColor(const QColor &baseColor, const QList<QColor> &availableColors) {
        auto l = toLuminance(baseColor);
        return *std::ranges::max_element(availableColors, [l](const auto &c1, const auto &c2) {
            auto l1 = toLuminance(c1);
            auto l2 = toLuminance(c2);
            return contrast(l1, l) < contrast(l2, l);
        });
    }
}
