// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGVIEW_P_H
#define SVSCRAFT_DOCKINGVIEW_P_H

#include <SVSCraftQuick/DockingView.h>

#include <QByteArray>
#include <QHash>
#include <QPointer>
#include <QSet>

#include <SVSCraftQuick/DockingHost.h>

namespace SVS {
    class DockingNode;

    class DockingViewFrame : public QObject {
        Q_OBJECT
        QML_ANONYMOUS
        Q_PROPERTY(QObject *node READ node CONSTANT)
        Q_PROPERTY(QRectF rect READ rect NOTIFY rectChanged)
        Q_PROPERTY(int index READ index CONSTANT)
        Q_PROPERTY(bool horizontal READ horizontal CONSTANT)
    public:
        DockingViewFrame(QObject *node, int index, bool horizontal, QObject *parent)
            : QObject(parent), m_node(node), m_index(index), m_horizontal(horizontal) {
        }

        QObject *node() const {
            return m_node;
        }

        QRectF rect() const {
            return m_rect;
        }

        int index() const {
            return m_index;
        }

        bool horizontal() const {
            return m_horizontal;
        }

        void setRect(const QRectF &rect) {
            if (m_rect == rect)
                return;
            m_rect = rect;
            Q_EMIT rectChanged();
        }

    Q_SIGNALS:
        void rectChanged();

    private:
        QPointer<QObject> m_node;
        QRectF m_rect;
        int m_index;
        bool m_horizontal;
    };

    class DockingViewPrivate {
        Q_DECLARE_PUBLIC(DockingView)
    public:
        DockingView *q_ptr = nullptr;
        QPointer<DockingHost> host;
        qreal padding = 0;
        QVariantList groups;
        QVariantList handles;
        QHash<DockingNode *, QRectF> rectangles;
        QHash<QString, DockingViewFrame *> frames;
        QSet<QString> usedFrames;
        mutable QVariantMap previewKey;
        mutable QHash<QByteArray, QRectF> previewRects;
        QList<QMetaObject::Connection> connections;
        void arrange(DockingNode *node, const QRectF &rect);
        DockingViewFrame *frame(DockingNode *node, int index, bool horizontal, const QRectF &rect);
    };
}

#endif
