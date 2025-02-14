#ifndef COLORCHANGE_H
#define COLORCHANGE_H

#include <QMetaObject>

class QColor;

namespace SVS {

    class ColorChange {
        Q_GADGET
        Q_PROPERTY(float valueFactor READ valueFactor CONSTANT)
        Q_PROPERTY(float alphaFactor READ alphaFactor CONSTANT)
    public:
        constexpr ColorChange(float saturationFactor = 1.0f, float valueFactor = 1.0f, float alphaFactor = 1.0f) : m_saturationFactor(saturationFactor), m_valueFactor(valueFactor), m_alphaFactor(alphaFactor) {
        }

        constexpr float saturationFactor() const {
            return m_saturationFactor;
        }

        constexpr float valueFactor() const {
            return m_valueFactor;
        }

        constexpr float alphaFactor() const {
            return m_alphaFactor;
        }

        constexpr bool operator==(const ColorChange &) const = default;

        Q_INVOKABLE QColor apply(const QColor &color) const;

    private:
        float m_saturationFactor;
        float m_valueFactor;
        float m_alphaFactor;
    };

}

#endif //COLORCHANGE_H
