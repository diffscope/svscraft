// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGGROUP_H
#define SVSCRAFT_DOCKINGGROUP_H

#include <SVSCraftQuick/DockingNode.h>
#include <SVSCraftQuick/DockingTypes.h>

Q_MOC_INCLUDE(<SVSCraftQuick / DockingPanel.h>)

namespace SVS {
    class DockingPanel;
    class DockingGroupPrivate;

    class SVSCRAFT_QUICK_EXPORT DockingGroup : public DockingNode {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DockingGroup)
        QML_ELEMENT
        QML_UNCREATABLE("Groups belong to a DockingLayout")
        Q_PROPERTY(Qt::Edge tabBarEdge READ tabBarEdge WRITE setTabBarEdge NOTIFY changed)
        Q_PROPERTY(bool autoHide READ autoHide WRITE setAutoHide NOTIFY changed)
        Q_PROPERTY(bool showPanelNames READ showPanelNames WRITE setShowPanelNames NOTIFY changed)
        Q_PROPERTY(bool compactLayout READ compactLayout WRITE setCompactLayout NOTIFY changed)
        Q_PROPERTY(bool widthLocked READ widthLocked WRITE setWidthLocked NOTIFY changed)
        Q_PROPERTY(bool heightLocked READ heightLocked WRITE setHeightLocked NOTIFY changed)
        Q_PROPERTY(QSizeF lockedSize READ lockedSize WRITE setLockedSize NOTIFY changed)
        Q_PROPERTY(QSizeF expandedSize READ expandedSize WRITE setExpandedSize NOTIFY changed)
        Q_PROPERTY(qreal contentRatio READ contentRatio WRITE setContentRatio NOTIFY changed)
        Q_PROPERTY(bool collapsible READ isCollapsible NOTIFY changed)
        Q_PROPERTY(bool collapsed READ isCollapsed NOTIFY changed)
        Q_PROPERTY(bool barHidden READ isBarHidden NOTIFY changed)
        Q_PROPERTY(QVariantList leadingPanels READ leadingPanels NOTIFY changed)
        Q_PROPERTY(QVariantList trailingPanels READ trailingPanels NOTIFY changed)
        Q_PROPERTY(DockingPanel *leadingPanel READ leadingPanel NOTIFY changed)
        Q_PROPERTY(DockingPanel *trailingPanel READ trailingPanel NOTIFY changed)
    public:
        enum Side {
            Leading,
            Trailing
        };
        Q_ENUM(Side)
        ~DockingGroup() override;
        Qt::Edge tabBarEdge() const;
        void setTabBarEdge(Qt::Edge edge);
        bool autoHide() const;
        void setAutoHide(bool hide);
        bool showPanelNames() const;
        void setShowPanelNames(bool show);
        bool compactLayout() const;
        void setCompactLayout(bool compact);
        bool widthLocked() const override;
        void setWidthLocked(bool locked);
        bool heightLocked() const override;
        void setHeightLocked(bool locked);
        QSizeF lockedSize() const override;
        void setLockedSize(const QSizeF &size);
        QSizeF expandedSize() const;
        void setExpandedSize(const QSizeF &size);
        qreal contentRatio() const;
        void setContentRatio(qreal ratio);
        bool isCollapsible() const;
        bool isCollapsed() const;
        bool isBarHidden() const;
        QVariantList leadingPanels() const;
        QVariantList trailingPanels() const;
        DockingPanel *leadingPanel() const;
        DockingPanel *trailingPanel() const;
        QList<DockingPanel *> panels(Side side) const;
        DockingPanel *selectedPanel(Side side) const;
        Q_INVOKABLE int sideOf(DockingPanel *panel) const;
        Q_INVOKABLE DockingResult select(DockingPanel *panel);
        Q_INVOKABLE bool canHide(DockingPanel *panel) const;
        Q_INVOKABLE DockingResult hide(DockingPanel *panel);
        Q_INVOKABLE DockingResult insertPanel(DockingPanel *panel, Side side, const QString &anchor = {}, bool after = true);
        Q_INVOKABLE DockingResult removePanel(DockingPanel *panel);
        QSizeF minimumSize() const override;
        QSizeF preferredSize() const override;

    private:
        friend class DockingLayout;
        friend class DockingContext;
        explicit DockingGroup(const QString &id, DockingLayout *layout);
        QScopedPointer<DockingGroupPrivate> d_ptr;
    };
}

#endif
