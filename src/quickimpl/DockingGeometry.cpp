// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingGeometry_p.h"

#include <algorithm>

namespace SVS {
    static void proportional(QVector<qreal> &result, QList<int> indices, qreal available, const QVector<DockingExtent> &extents, bool targets) {
        while (!indices.isEmpty()) {
            qreal maximum = 0;
            for (auto i : indices)
                maximum = std::max(maximum, targets ? extents[i].target : extents[i].weight);

            auto weight = [&](int i) {
                return maximum > 0 ? (targets ? extents[i].target : extents[i].weight) / maximum
                                   : 1;
            };
            qreal total = 0;
            for (auto i : indices)
                total += weight(i);
            if (total <= 0)
                return;

            auto factor = available / total;
            QList<int> next;
            for (auto i : indices) {
                qreal v = factor * weight(i);
                if (v < extents[i].minimum) {
                    result[i] = extents[i].minimum;
                    available -= result[i];
                } else
                    next.append(i);
            }

            if (next.size() == indices.size()) {
                for (auto i : indices)
                    result[i] = std::max(extents[i].minimum, factor * weight(i));
                return;
            }
            indices = next;
        }
    }

    QVector<qreal> dockingAllocate(qreal available, const QVector<DockingExtent> &extents) {
        QVector<qreal> result(extents.size());
        QList<int> locked, flexible;
        qreal lockedTotal = 0, flexibleMinimum = 0;
        for (int i = 0; i < extents.size(); ++i) {
            const auto &e = extents[i];
            if (e.fixed) {
                result[i] = e.minimum;
                available -= e.minimum;
            } else if (e.locked) {
                locked.append(i);
                lockedTotal += std::max(e.minimum, e.target);
            } else {
                flexible.append(i);
                flexibleMinimum += e.minimum;
            }
        }

        if (!flexible.isEmpty() && available >= lockedTotal + flexibleMinimum) {
            for (auto i : locked)
                result[i] = std::max(extents[i].minimum, extents[i].target);
            proportional(result, flexible, available - lockedTotal, extents, false);
        } else {
            for (auto i : flexible)
                result[i] = extents[i].minimum;
            proportional(result, locked, available - flexibleMinimum, extents, true);
        }
        return result;
    }

}
