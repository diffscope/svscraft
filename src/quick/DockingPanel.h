// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGPANEL_H
#define SVSCRAFT_DOCKINGPANEL_H

#include <QColor>
#include <QObject>
#include <QScopedPointer>
#include <QSizeF>
#include <QUrl>
#include <qqmlintegration.h>

#include <SVSCraftQuick/SVSCraftQuickGlobal.h>

class QQuickItem;

Q_MOC_INCLUDE(<SVSCraftQuick / DockingContext.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingGroup.h>)
Q_MOC_INCLUDE(<QQuickItem>)

namespace SVS {
    class DockingContext;
    class DockingGroup;
    class DockingPanelPrivate;

    class SVSCRAFT_QUICK_EXPORT DockingPanel : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DockingPanel)
        QML_ELEMENT
        Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged)
        Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY metadataChanged)
        Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY metadataChanged)
        Q_PROPERTY(QUrl iconSource READ iconSource WRITE setIconSource NOTIFY metadataChanged)
        Q_PROPERTY(QColor iconColor READ iconColor WRITE setIconColor NOTIFY metadataChanged)
        Q_PROPERTY(int badgeNumber READ badgeNumber WRITE setBadgeNumber NOTIFY metadataChanged)
        Q_PROPERTY(int badgeType READ badgeType WRITE setBadgeType NOTIFY metadataChanged)
        Q_PROPERTY(QSizeF minimumSize READ minimumSize WRITE setMinimumSize NOTIFY sizeHintsChanged)
        Q_PROPERTY(QSizeF preferredSize READ preferredSize WRITE setPreferredSize NOTIFY sizeHintsChanged)
        Q_PROPERTY(QSizeF effectivePreferredSize READ effectivePreferredSize NOTIFY sizeHintsChanged)
        Q_PROPERTY(QQuickItem *contentItem READ contentItem WRITE setContentItem NOTIFY contentItemChanged)
        Q_PROPERTY(QQuickItem *headerItem READ headerItem WRITE setHeaderItem NOTIFY headerItemChanged)
        Q_PROPERTY(DockingContext *context READ context NOTIFY placementChanged)
        Q_PROPERTY(DockingGroup *group READ group NOTIFY placementChanged)
        Q_PROPERTY(bool shown READ isShown NOTIFY placementChanged)
        Q_PROPERTY(bool active READ isActive NOTIFY placementChanged)
    public:
        explicit DockingPanel(QObject *parent = nullptr);
        ~DockingPanel() override;
        QString key() const;
        void setKey(const QString &key);
        QString title() const;
        void setTitle(const QString &title);
        QString description() const;
        void setDescription(const QString &description);
        QUrl iconSource() const;
        void setIconSource(const QUrl &source);
        QColor iconColor() const;
        void setIconColor(const QColor &color);
        int badgeNumber() const;
        void setBadgeNumber(int number);
        int badgeType() const;
        void setBadgeType(int type);
        QSizeF minimumSize() const;
        void setMinimumSize(const QSizeF &size);
        QSizeF preferredSize() const;
        void setPreferredSize(const QSizeF &size);
        QSizeF effectivePreferredSize() const;
        QQuickItem *contentItem() const;
        void setContentItem(QQuickItem *item);
        QQuickItem *headerItem() const;
        void setHeaderItem(QQuickItem *item);
        DockingContext *context() const;
        DockingGroup *group() const;
        bool isShown() const;
        bool isActive() const;
    Q_SIGNALS:
        void keyChanged();
        void metadataChanged();
        void sizeHintsChanged();
        void contentItemChanged();
        void headerItemChanged();
        void placementChanged();
        void activated();

    private:
        friend class DockingContext;
        QScopedPointer<DockingPanelPrivate> d_ptr;
    };
}

#endif
