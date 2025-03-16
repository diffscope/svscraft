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