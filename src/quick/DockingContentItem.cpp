// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingContentItem_p.h"

#include <algorithm>

#include <SVSCraftQuick/DockingContext.h>

namespace SVS {
    DockingContentItem::DockingContentItem(QQuickItem *parent) : QQuickItem(parent) {
        connect(this, &QQuickItem::visibleChanged, this, &DockingContentItem::update);
        setFlag(ItemIsFocusScope);
        connect(this, &QQuickItem::activeFocusChanged, this, [this] {
            if (hasActiveFocus() && m_panel && m_panel->isShown())
                m_panel->context()->activate(m_panel, false);
        });
    }

    DockingContentItem::~DockingContentItem() {
        release();
    }

    DockingPanel *DockingContentItem::panel() const {
        return m_panel;
    }

    void DockingContentItem::setPanel(DockingPanel *p) {
        if (m_panel == p)
            return;
        release();
        for (const auto &c : m_connections)
            disconnect(c);
        m_connections.clear();
        m_panel = p;
        if (p) {
            m_connections.append(connect(p, &DockingPanel::contentItemChanged, this, &DockingContentItem::update));
            m_connections.append(connect(p, &DockingPanel::headerItemChanged, this, &DockingContentItem::update));
            m_connections.append(connect(p, &DockingPanel::placementChanged, this, &DockingContentItem::update));
        }
        update();
        Q_EMIT panelChanged();
    }

    bool DockingContentItem::header() const {
        return m_header;
    }

    void DockingContentItem::setHeader(bool v) {
        if (v == m_header)
            return;
        release();
        m_header = v;
        update();
        Q_EMIT panelChanged();
    }

    void DockingContentItem::release() {
        if (m_item && m_item->parentItem() == this) {
            m_item->setVisible(false);
            m_item->setParentItem(nullptr);
        }
        m_item = nullptr;
    }

    void DockingContentItem::update() {
        auto item = m_panel ? m_header ? m_panel->headerItem() : m_panel->contentItem() : nullptr;
        if (!isVisible() || !m_panel || !m_panel->isShown()) {
            release();
            return;
        }
        if (item != m_item) {
            release();
            m_item = item;
        }
        if (!m_item)
            return;
        m_item->setParentItem(this);
        m_item->setPosition({0, 0});
        auto minimum = m_header ? QSizeF(0, 0) : m_panel->minimumSize();
        m_item->setSize({std::max(width(), minimum.width()), std::max(height(), minimum.height())});
        m_item->setVisible(true);
    }

    void DockingContentItem::geometryChange(const QRectF &n, const QRectF &o) {
        QQuickItem::geometryChange(n, o);
        update();
    }
}

#include "moc_DockingContentItem_p.cpp"

