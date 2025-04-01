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

#ifndef SVSCRAFT_ATTACHEDPROPERTYPROPAGATOR_H
#define SVSCRAFT_ATTACHEDPROPERTYPROPAGATOR_H

#include <algorithm>
#include <iterator>

#include <QQuickAttachedPropertyPropagator>

#include <SVSCraftQuick/SVSCraftQuickGlobal.h>

namespace SVS {

    class AttachedPropertyPropagatorProperties;

    class SVSCRAFT_QUICK_EXPORT AttachedPropertyPropagator : public QQuickAttachedPropertyPropagator {
        Q_OBJECT
        friend class AttachedPropertyPropagatorProperties;
    public:
        inline explicit AttachedPropertyPropagator(QObject* parent = nullptr) : QQuickAttachedPropertyPropagator(parent) {
        }

        inline AttachedPropertyPropagator *attachedParent() const {
            return static_cast<AttachedPropertyPropagator *>(QQuickAttachedPropertyPropagator::attachedParent());
        }
        inline QList<AttachedPropertyPropagator *> attachedChildren() const {
            QList<AttachedPropertyPropagator *> a;
            const auto b = QQuickAttachedPropertyPropagator::attachedChildren();
            a.reserve(b.size());
            std::ranges::transform(b, std::back_inserter(a), [](auto p) { return static_cast<AttachedPropertyPropagator *>(p); });
            return a;
        }

    protected:
        virtual AttachedPropertyPropagatorProperties *properties() const = 0;
        void attachedParentChange(QQuickAttachedPropertyPropagator *newParent, QQuickAttachedPropertyPropagator *oldParent) override;
    };

}

#endif //SVSCRAFT_ATTACHEDPROPERTYPROPAGATOR_H
