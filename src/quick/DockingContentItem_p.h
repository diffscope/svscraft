// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGCONTENTITEM_P_H
#define SVSCRAFT_DOCKINGCONTENTITEM_P_H

#include <QPointer>
#include <QQuickItem>

#include <SVSCraftQuick/DockingPanel.h>

namespace SVS {
    class DockingContentItem : public QQuickItem {
        Q_OBJECT
        QML_NAMED_ELEMENT(DockingContentTemplate)
        Q_PROPERTY(DockingPanel *panel READ panel WRITE setPanel NOTIFY panelChanged)
        Q_PROPERTY(bool header READ header WRITE setHeader NOTIFY panelChanged)
    public:
        explicit DockingContentItem(QQuickItem *parent = nullptr);
        ~DockingContentItem() override;
        DockingPanel *panel() const;
        void setPanel(DockingPanel *panel);
        bool header() const;
        void setHeader(bool header);
    Q_SIGNALS:
        void panelChanged();

    protected:
        void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

    private:
        void update();
        void release();
        QPointer<DockingPanel> m_panel;
        QPointer<QQuickItem> m_item;
        bool m_header = false;
        QList<QMetaObject::Connection> m_connections;
    };
}

#endif

