// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingSplit.h"
#include "DockingSplit_p.h"

#include <algorithm>
#include <cmath>

#include <SVSCraftQuick/DockingLayout.h>

namespace SVS {
    DockingSplit::DockingSplit(const QString &id, DockingLayout *layout) : DockingNode(Split, id, layout), d_ptr(new DockingSplitPrivate) {
        d_ptr->q_ptr = this;
    }

    DockingSplit::~DockingSplit() = default;

    Qt::Orientation DockingSplit::orientation() const {
        Q_D(const DockingSplit);
        return d->orientation;
    }

    void DockingSplit::setOrientation(Qt::Orientation v) {
        Q_D(DockingSplit);
        if ((v != Qt::Horizontal && v != Qt::Vertical) || d->orientation == v)
            return;
        d->orientation = v;
        layout()->commit();
    }

    QList<DockingNode *> DockingSplit::children() const {
        Q_D(const DockingSplit);
        return d->children;
    }

    QVariantList DockingSplit::childNodes() const {
        QVariantList result;
        for (auto n : children())
            result.append(QVariant::fromValue(n));
        return result;
    }

    QVariantList DockingSplit::weights() const {
        Q_D(const DockingSplit);
        QVariantList result;
        for (auto v : d->weights)
            result.append(v);
        return result;
    }

    void DockingSplit::setWeights(const QVariantList &values) {
        Q_D(DockingSplit);
        if (values.size() != d->children.size())
            return;
        QVector<qreal> w;
        for (const auto &v : values) {
            auto x = v.toDouble();
            if (!std::isfinite(x) || x <= 0)
                return;
            w.append(x);
        }
        if (w == d->weights)
            return;
        d->weights = w;
        layout()->commit();
    }

    QSizeF DockingSplitPrivate::aggregate(int hint) const {
        QSizeF result(0, 0);
        for (auto child : children) {
            auto s = hint == 0 ? child->minimumSize() : hint == 1 ? child->preferredSize()
                                                                  : child->lockedSize();
            if (orientation == Qt::Horizontal) {
                result.rwidth() += s.width();
                result.setHeight(std::max(result.height(), s.height()));
            } else {
                result.rheight() += s.height();
                result.setWidth(std::max(result.width(), s.width()));
            }
        }
        if (orientation == Qt::Horizontal)
            result.rwidth() += std::max(qsizetype(0), children.size() - 1);
        else
            result.rheight() += std::max(qsizetype(0), children.size() - 1);
        return result;
    }

    QSizeF DockingSplit::minimumSize() const {
        Q_D(const DockingSplit);
        return d->aggregate(0);
    }

    QSizeF DockingSplit::preferredSize() const {
        Q_D(const DockingSplit);
        return d->aggregate(1);
    }

    QSizeF DockingSplit::lockedSize() const {
        Q_D(const DockingSplit);
        return d->aggregate(2);
    }

    bool DockingSplit::widthLocked() const {
        auto c = children();
        return !c.isEmpty() && std::all_of(c.begin(), c.end(), [](auto n) { return n->widthLocked(); });
    }

    bool DockingSplit::heightLocked() const {
        auto c = children();
        return !c.isEmpty() && std::all_of(c.begin(), c.end(), [](auto n) { return n->heightLocked(); });
    }
}

#include "moc_DockingSplit.cpp"
