#include "musictimesignature.h"

#include <QDebug>

namespace SVS {

    QString MusicTimeSignature::toString() const {
        return QStringLiteral("%1/%2").arg(m_numerator).arg(m_denominator);
    }

    QDebug operator<<(QDebug debug, const MusicTimeSignature &t) {
        QDebugStateSaver saver(debug);
        debug.nospace() << "MusicTimeSignature(" << t.m_numerator << "/" << t.m_denominator << ")";
        return debug;
    }
}