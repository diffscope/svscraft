#ifndef SVSCRAFT_COLORCHANGE_H
#define SVSCRAFT_COLORCHANGE_H

#include <QColor>
#include <QMetaObject>

#include <SVSCraftGui/svscraftguiglobal.h>

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

#endif //COLORCHANGE_H
