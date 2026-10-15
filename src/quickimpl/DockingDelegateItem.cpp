// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingDelegateItem_p.h"

#include <QQmlEngine>

namespace SVS {
    DockingDelegateItem::DockingDelegateItem(QQuickItem *parent) : QQuickItem(parent) {}

    QQmlComponent *DockingDelegateItem::sourceComponent() const {
        return m_component;
    }

    void DockingDelegateItem::setSourceComponent(QQmlComponent *component) {
        if (m_component == component)
            return;
        for (const auto &connection : m_componentConnections)
            disconnect(connection);
        m_componentConnections.clear();
        m_component = component;
        if (component) {
            m_componentConnections.append(connect(component, &QQmlComponent::statusChanged, this, [this] { polish(); }));
            m_componentConnections.append(connect(component, &QObject::destroyed, this, [this] {
                Q_EMIT sourceComponentChanged();
                polish();
            }));
        }
        Q_EMIT sourceComponentChanged();
        polish();
    }

    QVariantMap DockingDelegateItem::initialProperties() const {
        return m_properties;
    }

    void DockingDelegateItem::setInitialProperties(const QVariantMap &properties) {
        if (m_properties == properties)
            return;
        m_properties = properties;
        Q_EMIT initialPropertiesChanged();
        polish();
    }

    QQuickItem *DockingDelegateItem::item() const {
        return m_item;
    }

    void DockingDelegateItem::componentComplete() {
        QQuickItem::componentComplete();
        polish();
    }

    void DockingDelegateItem::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
        QQuickItem::geometryChange(newGeometry, oldGeometry);
        if (m_item)
            m_item->setSize(size());
    }

    void DockingDelegateItem::updateImplicitSize() {
        setImplicitSize(m_item ? m_item->implicitWidth() : 0, m_item ? m_item->implicitHeight() : 0);
    }

    void DockingDelegateItem::updatePolish() {
        if (!isComponentComplete())
            return;
        for (const auto &connection : m_itemConnections)
            disconnect(connection);
        m_itemConnections.clear();
        if (m_item) {
            m_item->setVisible(false);
            m_item->setParentItem(nullptr);
            m_item->deleteLater();
        }
        m_item = nullptr;
        if (m_component && m_component->isReady()) {
            auto object = m_component->createWithInitialProperties(m_properties, m_component->creationContext() ? m_component->creationContext() : qmlContext(this));
            m_item = qobject_cast<QQuickItem *>(object);
            if (!m_item)
                delete object;
            else {
                m_item->setParent(this);
                QQmlEngine::setObjectOwnership(m_item, QQmlEngine::CppOwnership);
                m_item->setParentItem(this);
                m_item->setSize(size());
                m_itemConnections.append(connect(m_item, &QQuickItem::implicitWidthChanged, this, &DockingDelegateItem::updateImplicitSize));
                m_itemConnections.append(connect(m_item, &QQuickItem::implicitHeightChanged, this, &DockingDelegateItem::updateImplicitSize));
                m_itemConnections.append(connect(m_item, &QObject::destroyed, this, [this] {
                    updateImplicitSize();
                    Q_EMIT itemChanged();
                }));
            }
        }
        updateImplicitSize();
        Q_EMIT itemChanged();
    }
}

#include "moc_DockingDelegateItem_p.cpp"
