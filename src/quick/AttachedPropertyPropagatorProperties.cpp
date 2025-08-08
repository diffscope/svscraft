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

#include "AttachedPropertyPropagatorProperties.h"

#include <QMetaProperty>

#include <SVSCraftQuick/AttachedPropertyPropagator.h>

namespace SVS {
    AttachedPropertyPropagatorProperties::AttachedPropertyPropagatorProperties(AttachedPropertyPropagator *propagator, AttachedPropertyPropagatorProperties *defaultProperties, const QVariantMap &initialProperties) : m_propagator(propagator), m_defaultProperties(defaultProperties), m_m(initialProperties) {
        if (!propagator->attachedParent() && defaultProperties) {
            defaultProperties->m_rootPropagators.insert(propagator);
        }
    }
    AttachedPropertyPropagatorProperties::~AttachedPropertyPropagatorProperties() {
        if (m_defaultProperties) {
            m_defaultProperties->m_rootPropagators.remove(m_propagator);
        }
    }
    void AttachedPropertyPropagatorProperties::propagateAndNotify(const QString &property) const {
        auto q = m_propagator;
        for (auto child : q->attachedChildren()) {
            child->properties()->inherit(property);
        }
        for (auto child : m_rootPropagators) {
            child->properties()->inherit(property);
        }
        auto i = q->metaObject()->indexOfProperty(property.toUtf8());
        q->metaObject()->property(i).notifySignal().invoke(q);
    }
    static bool variantEquals(const QVariant &a, const QVariant &b) {
        auto t = a.metaType();
        if (t != b.metaType()) {
            return false;
        }
        return t.equals(a.data(), b.data());
    }
    void AttachedPropertyPropagatorProperties::inherit(const QString &property) {
        auto q = m_propagator;
        if (m_explicitSetProperties.contains(property))
            return;
        auto parent = q->attachedParent() ? q->attachedParent()->properties() : m_defaultProperties;
        auto v = parent->getValue(property);
        if (variantEquals(v, m_m.value(property)))
            return;
        m_m.insert(property, v);
        propagateAndNotify(property);
    }
    void AttachedPropertyPropagatorProperties::inheritAll() {
        auto q = m_propagator;
        QStringList propertiesChanged;
        auto parent = q->attachedParent() ? q->attachedParent()->properties() : m_defaultProperties;
        if (m_explicitSetProperties.isEmpty()) {
            for (auto p = m_m.keyValueBegin(); p != m_m.keyValueEnd(); p++) {
                auto [property, value] = *p;
                if (!variantEquals(value, parent->getValue(property)))
                    propertiesChanged.append(property);
            }
            m_m = parent->m_m;
        } else {
            for (auto p = m_m.keyValueBegin(); p != m_m.keyValueEnd(); p++) {
                auto [property, value] = *p;
                if (m_explicitSetProperties.contains(property))
                    continue;
                if (!variantEquals(value, parent->getValue(property))) {
                    propertiesChanged.append(property);
                    value = parent->getValue(property);
                }
            }
        }
        for (const auto &property : propertiesChanged) {
            propagateAndNotify(property);
        }
    }
}