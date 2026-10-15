// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingPanel.h"
#include "DockingPanel_p.h"

#include <algorithm>
#include <cmath>

#include <SVSCraftQuick/DockingContext.h>
#include <SVSCraftQuick/DockingGroup.h>

namespace SVS {
    DockingPanel::DockingPanel(QObject *parent) : QObject(parent), d_ptr(new DockingPanelPrivate) {
        d_ptr->q_ptr = this;
    }

    DockingPanel::~DockingPanel() {
        if (d_ptr->context)
            d_ptr->context->unregisterPanel(this);
    }

    QString DockingPanel::key() const {
        Q_D(const DockingPanel);
        return d->key;
    }

    void DockingPanel::setKey(const QString &value) {
        Q_D(DockingPanel);
        if (d->context || d->key == value)
            return;
        d->key = value;
        Q_EMIT keyChanged();
    }

    QString DockingPanel::title() const {
        Q_D(const DockingPanel);
        return d->title;
    }

    void DockingPanel::setTitle(const QString &v) {
        Q_D(DockingPanel);
        if (d->title == v)
            return;
        d->title = v;
        Q_EMIT metadataChanged();
    }

    QString DockingPanel::description() const {
        Q_D(const DockingPanel);
        return d->description;
    }

    void DockingPanel::setDescription(const QString &v) {
        Q_D(DockingPanel);
        if (d->description == v)
            return;
        d->description = v;
        Q_EMIT metadataChanged();
    }

    QUrl DockingPanel::iconSource() const {
        Q_D(const DockingPanel);
        return d->iconSource;
    }

    void DockingPanel::setIconSource(const QUrl &v) {
        Q_D(DockingPanel);
        if (d->iconSource == v)
            return;
        d->iconSource = v;
        Q_EMIT metadataChanged();
    }

    QColor DockingPanel::iconColor() const {
        Q_D(const DockingPanel);
        return d->iconColor;
    }

    int DockingPanel::badgeNumber() const {
        Q_D(const DockingPanel);
        return d->badgeNumber;
    }

    void DockingPanel::setBadgeNumber(int v) {
        Q_D(DockingPanel);
        if (d->badgeNumber == v)
            return;
        d->badgeNumber = v;
        Q_EMIT metadataChanged();
    }

    int DockingPanel::badgeType() const {
        Q_D(const DockingPanel);
        return d->badgeType;
    }

    void DockingPanel::setBadgeType(int v) {
        Q_D(DockingPanel);
        if (d->badgeType == v || v < SVSCraft::CT_Normal || v > SVSCraft::CT_Error)
            return;
        d->badgeType = v;
        Q_EMIT metadataChanged();
    }

    void DockingPanel::setIconColor(const QColor &v) {
        Q_D(DockingPanel);
        if (d->iconColor == v)
            return;
        d->iconColor = v;
        Q_EMIT metadataChanged();
    }

    QSizeF DockingPanel::minimumSize() const {
        Q_D(const DockingPanel);
        return d->minimumSize;
    }

    void DockingPanel::setMinimumSize(const QSizeF &v) {
        Q_D(DockingPanel);
        if (!std::isfinite(v.width()) || !std::isfinite(v.height()) || v.width() < 0 || v.height() < 0 || v == d->minimumSize)
            return;
        d->minimumSize = v;
        Q_EMIT sizeHintsChanged();
    }

    QSizeF DockingPanel::preferredSize() const {
        Q_D(const DockingPanel);
        return d->preferredSize;
    }

    void DockingPanel::setPreferredSize(const QSizeF &v) {
        Q_D(DockingPanel);
        auto valid = [](qreal x) { return std::isfinite(x) && (x >= 0 || x == -1); };
        if (!valid(v.width()) || !valid(v.height()) || v == d->preferredSize)
            return;
        d->preferredSize = v;
        Q_EMIT sizeHintsChanged();
    }

    QSizeF DockingPanel::effectivePreferredSize() const {
        Q_D(const DockingPanel);
        auto w = d->preferredSize.width() >= 0 ? d->preferredSize.width() : d->contentItem ? d->contentItem->implicitWidth()
                                                                                           : 0;
        auto h = d->preferredSize.height() >= 0 ? d->preferredSize.height() : d->contentItem ? d->contentItem->implicitHeight()
                                                                                             : 0;
        return {std::max(d->minimumSize.width(), std::isfinite(w) ? w : 0), std::max(d->minimumSize.height(), std::isfinite(h) ? h : 0)};
    }

    QQuickItem *DockingPanel::contentItem() const {
        Q_D(const DockingPanel);
        return d->contentItem;
    }

    void DockingPanel::setContentItem(QQuickItem *item) {
        Q_D(DockingPanel);
        if (d->contentItem == item)
            return;
        for (const auto &c : d->contentConnections)
            disconnect(c);
        d->contentConnections.clear();
        d->contentItem = item;
        if (item) {
            d->contentConnections.append(connect(item, &QQuickItem::implicitWidthChanged, this, &DockingPanel::sizeHintsChanged));
            d->contentConnections.append(connect(item, &QQuickItem::implicitHeightChanged, this, &DockingPanel::sizeHintsChanged));
            d->contentConnections.append(connect(item, &QObject::destroyed, this, [this] {
                if (context())
                    context()->remove(this);
                Q_EMIT contentItemChanged();
            }));
        }
        Q_EMIT contentItemChanged();
        Q_EMIT sizeHintsChanged();
    }

    QQuickItem *DockingPanel::headerItem() const {
        Q_D(const DockingPanel);
        return d->headerItem;
    }

    void DockingPanel::setHeaderItem(QQuickItem *v) {
        Q_D(DockingPanel);
        if (d->headerItem == v)
            return;
        disconnect(d->headerConnection);
        d->headerItem = v;
        if (v)
            d->headerConnection = connect(v, &QObject::destroyed, this, &DockingPanel::headerItemChanged);
        Q_EMIT headerItemChanged();
    }

    DockingContext *DockingPanel::context() const {
        Q_D(const DockingPanel);
        return d->context;
    }

    DockingGroup *DockingPanel::group() const {
        return context() ? context()->groupForPanel(key()) : nullptr;
    }

    bool DockingPanel::isShown() const {
        auto g = group();
        return g && (g->leadingPanel() == this || g->trailingPanel() == this);
    }

    bool DockingPanel::isActive() const {
        return context() && context()->activePanel() == this;
    }
}

#include "moc_DockingPanel.cpp"
