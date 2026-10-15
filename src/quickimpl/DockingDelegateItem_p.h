// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGDELEGATEITEM_P_H
#define SVSCRAFT_DOCKINGDELEGATEITEM_P_H

#include <QPointer>
#include <QQmlComponent>
#include <QQuickItem>
#include <QVariantMap>
#include <qqmlintegration.h>

namespace SVS {
    class DockingDelegateItem : public QQuickItem {
        Q_OBJECT
        QML_NAMED_ELEMENT(DockingDelegate)
        Q_PROPERTY(QQmlComponent *sourceComponent READ sourceComponent WRITE setSourceComponent NOTIFY sourceComponentChanged)
        Q_PROPERTY(QVariantMap initialProperties READ initialProperties WRITE setInitialProperties NOTIFY initialPropertiesChanged)
        Q_PROPERTY(QQuickItem *item READ item NOTIFY itemChanged)
    public:
        explicit DockingDelegateItem(QQuickItem *parent = nullptr);
        QQmlComponent *sourceComponent() const;
        void setSourceComponent(QQmlComponent *component);
        QVariantMap initialProperties() const;
        void setInitialProperties(const QVariantMap &properties);
        QQuickItem *item() const;
    Q_SIGNALS:
        void sourceComponentChanged();
        void initialPropertiesChanged();
        void itemChanged();

    protected:
        void componentComplete() override;
        void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
        void updatePolish() override;

    private:
        void updateImplicitSize();
        QPointer<QQmlComponent> m_component;
        QPointer<QQuickItem> m_item;
        QVariantMap m_properties;
        QList<QMetaObject::Connection> m_componentConnections;
        QList<QMetaObject::Connection> m_itemConnections;
    };
}

#endif
