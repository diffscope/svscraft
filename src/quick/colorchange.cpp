#include "colorchange.h"

#include <QColor>

namespace SVS {
    QColor ColorChange::apply(const QColor &color) const {
        if (!qFuzzyCompare(m_valueFactor, 1.0f) || !qFuzzyCompare(m_saturationFactor, 0.0f)) {
            auto oldSpec = color.spec();
            auto hsvColor = color.convertTo(QColor::Hsv);
            return QColor::fromHsvF(hsvColor.hueF(), qBound(0.0f, hsvColor.saturationF() * m_saturationFactor, 1.0f), qBound(0.0f, hsvColor.valueF() * m_valueFactor, 1.0f), qBound(0.0f, hsvColor.alphaF() * m_alphaFactor, 1.0f)).convertTo(oldSpec);
        } else {
            auto newColor = color;
            newColor.setAlphaF(color.alphaF() * m_alphaFactor);
            return newColor;
        }
    }
}

#include "moc_colorchange.cpp"