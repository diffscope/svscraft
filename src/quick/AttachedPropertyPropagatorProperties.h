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

#ifndef SVSCRAFT_ATTACHEDPROPERTYPROPAGATORPROPERTIES_H
#define SVSCRAFT_ATTACHEDPROPERTYPROPAGATORPROPERTIES_H

#include <QVariant>

#include <SVSCraftQuick/SVSCraftQuickGlobal.h>

namespace SVS {

    namespace Impl {

        template <typename>
        struct ReturnTypeHelper;
        template <typename ClassType, typename ReturnType>
        struct ReturnTypeHelper<ReturnType (ClassType::*)() const> {
            using type = ReturnType;
        };

        template <typename>
        struct ParameterTypeHelper;
        template <typename ClassType, typename ReturnType, typename Arg>
        struct ParameterTypeHelper<ReturnType (ClassType::*)(Arg)> {
            using type = Arg;
        };

    }


#define SVS_ATTACHED_PROPERTY_PROPAGATOR_GETTER(clazz, property, func) \
    SVS::Impl::ReturnTypeHelper<decltype(&clazz::func)>::type clazz::func() const { \
        using type = SVS::Impl::ReturnTypeHelper<decltype(&clazz::func)>::type; \
        return properties()->getValue(#property).value<type>(); \
    }


#define SVS_ATTACHED_PROPERTY_PROPAGATOR_SETTER(clazz, property, func) \
    void clazz::func(SVS::Impl::ParameterTypeHelper<decltype(&clazz::func)>::type a) { \
        properties()->setValue(#property, QVariant::fromValue(a)); \
    }

#define SVS_ATTACHED_PROPERTY_PROPAGATOR_RESETTER(clazz, property, func) \
    void clazz::func() { \
        properties()->resetValue(#property); \
    }

#define SVS_ATTACHED_PROPERTY_PROPAGATOR_IMPLEMENTATION(clazz, property, getFunc, setFunc, resetFunc) \
    SVS_ATTACHED_PROPERTY_PROPAGATOR_GETTER(clazz, property, getFunc) \
    SVS_ATTACHED_PROPERTY_PROPAGATOR_SETTER(clazz, property, setFunc) \
    SVS_ATTACHED_PROPERTY_PROPAGATOR_RESETTER(clazz, property, resetFunc)

    class AttachedPropertyPropagator;

    class SVSCRAFT_QUICK_EXPORT AttachedPropertyPropagatorProperties {
    public:
        explicit AttachedPropertyPropagatorProperties(AttachedPropertyPropagator *propagator, AttachedPropertyPropagatorProperties *defaultProperties, const QVariantMap &initialProperties = {});
        ~AttachedPropertyPropagatorProperties();

        inline QVariant getValue(const QString &property) const {
            return m_m.value(property);
        }
        inline void setValue(const QString &property, const QVariant &value) {
            m_explicitSetProperties.insert(property);
            if (QVariant::compare(value, m_m.value(property)) == QPartialOrdering::Equivalent)
                return;
            m_m.insert(property, QVariant::fromValue(value));
            propagateAndNotify(property);
        }
        inline void resetValue(const QString &property) {
            if (m_explicitSetProperties.contains(property)) {
                m_explicitSetProperties.remove(property);
                inherit(property);
            }
        }

        void propagateAndNotify(const QString &property) const;
        void inherit(const QString &property);
        void inheritAll();

    private:
        AttachedPropertyPropagator *m_propagator;
        AttachedPropertyPropagatorProperties *m_defaultProperties;
        QVariantMap m_m;
        QSet<QString> m_explicitSetProperties;
    };

}

#endif //SVSCRAFT_ATTACHEDPROPERTYPROPAGATORPROPERTIES_H
