// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGDRAGSOURCE_P_H
#define SVSCRAFT_DOCKINGDRAGSOURCE_P_H

#include <QObject>
#include <QPointF>
#include <QPointer>
#include <qqmlintegration.h>

class QQuickDragAttached;
class QQuickItem;

Q_MOC_INCLUDE(<SVSCraftQuick / DockingContext.h>)

namespace SVS {
    class DockingContext;
    class DockingPanel;

    class DockingDragSource : public QObject {
        Q_OBJECT
        QML_ANONYMOUS
        Q_PROPERTY(QString panelKey READ panelKey CONSTANT)
        Q_PROPERTY(DockingContext *dockingContext READ dockingContext CONSTANT)
    public:
        static void start(DockingPanel *panel);
        QString panelKey() const;
        DockingContext *dockingContext() const;

    protected:
        bool eventFilter(QObject *watched, QEvent *event) override;

    private:
        explicit DockingDragSource(DockingPanel *panel);
        void updatePosition(const QPointF &position);
        void finish(bool drop);
        QString m_panelKey;
        QPointer<DockingContext> m_context;
        QPointer<QQuickItem> m_proxy;
        QPointer<QQuickDragAttached> m_drag;
        bool m_finishing = false;
        bool m_cursorOverridden = false;
    };
}

#endif
