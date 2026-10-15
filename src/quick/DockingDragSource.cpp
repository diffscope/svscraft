// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingDragSource_p.h"

#include <QCoreApplication>
#include <QCursor>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QTouchEvent>

#include <QtQuick/private/qquickdrag_p.h>

#include <SVSCraftQuick/DockingContext.h>
#include <SVSCraftQuick/DockingGroup.h>
#include <SVSCraftQuick/DockingPanel.h>

namespace SVS {
    DockingDragSource::DockingDragSource(DockingPanel *panel) : m_panelKey(panel->key()), m_context(panel->context()) {}

    QString DockingDragSource::panelKey() const {
        return m_panelKey;
    }

    DockingContext *DockingDragSource::dockingContext() const {
        return m_context;
    }

    void DockingDragSource::start(DockingPanel *panel) {
        if (!panel || !panel->context() || panel->context()->isDragging())
            return;
        auto source = new DockingDragSource(panel);
        QQmlEngine::setObjectOwnership(source, QQmlEngine::CppOwnership);
        source->m_proxy = new QQuickItem;
        source->m_proxy->setParent(source);
        source->m_drag = new QQuickDragAttached(source->m_proxy);
        source->m_drag->setSource(source);
        source->m_drag->setKeys({"application/x-svscraft-panel"});
        source->m_drag->setSupportedActions(Qt::MoveAction);
        connect(source->m_context, &QObject::destroyed, source, [source] { source->finish(false); });
        connect(panel, &QObject::destroyed, source, [source] { source->finish(false); });
        if (panel->group())
            connect(panel->group(), &QObject::destroyed, source, [source] { source->finish(false); });
        if (auto window = QGuiApplication::focusWindow())
            connect(window, &QObject::destroyed, source, [source] { source->finish(false); });
        connect(source->m_context, &DockingContext::dragChanged, source, [source] {
            if (!source->m_context || !source->m_context->isDragging())
                source->finish(false);
        });
        connect(source->m_drag, &QQuickDragAttached::targetChanged, source, [source] {
            if (source->m_cursorOverridden)
                QGuiApplication::changeOverrideCursor(source->m_drag->target() ? Qt::DragMoveCursor : Qt::ForbiddenCursor);
        });
        source->m_context->beginDrag(panel, source);
        if (!source->m_context || !source->m_context->isDragging() || source->m_finishing) {
            source->finish(false);
            return;
        }
        qGuiApp->installEventFilter(source);
        QGuiApplication::setOverrideCursor(Qt::ForbiddenCursor);
        source->m_cursorOverridden = true;
        source->updatePosition(QCursor::pos());
        source->m_drag->setActive(true);
    }

    void DockingDragSource::updatePosition(const QPointF &position) {
        if (!m_proxy || m_finishing)
            return;
        auto window = qobject_cast<QQuickWindow *>(QGuiApplication::topLevelAt(position.toPoint()));
        m_proxy->setParentItem(window ? window->contentItem() : nullptr);
        if (window)
            m_proxy->setPosition(window->mapFromGlobal(position.toPoint()));
    }

    void DockingDragSource::finish(bool drop) {
        if (m_finishing)
            return;
        m_finishing = true;
        qGuiApp->removeEventFilter(this);
        int action = Qt::IgnoreAction;
        if (m_drag) {
            if (drop) {
                QCoreApplication::sendPostedEvents(m_drag, QEvent::User);
                action = m_drag->drop();
            } else
                m_drag->cancel();
        }
        if (m_cursorOverridden) {
            QGuiApplication::restoreOverrideCursor();
            m_cursorOverridden = false;
        }
        if (m_context)
            m_context->endDrag(action != Qt::IgnoreAction);
        if (m_proxy)
            m_proxy->setParentItem(nullptr);
        deleteLater();
    }

    bool DockingDragSource::eventFilter(QObject *watched, QEvent *event) {
        switch (event->type()) {
            case QEvent::MouseMove:
                updatePosition(static_cast<QMouseEvent *>(event)->globalPosition());
                break;
            case QEvent::MouseButtonRelease: {
                auto mouse = static_cast<QMouseEvent *>(event);
                if (mouse->button() == Qt::LeftButton) {
                    updatePosition(mouse->globalPosition());
                    finish(true);
                }
                break;
            }
            case QEvent::TouchUpdate:
            case QEvent::TouchEnd: {
                const auto points = static_cast<QTouchEvent *>(event)->points();
                if (!points.isEmpty())
                    updatePosition(points.first().globalPosition());
                if (event->type() == QEvent::TouchEnd)
                    finish(true);
                break;
            }
            case QEvent::TouchCancel:
                finish(false);
                break;
            case QEvent::ShortcutOverride:
                if (static_cast<QKeyEvent *>(event)->key() == Qt::Key_Escape) {
                    event->accept();
                    return true;
                }
                break;
            case QEvent::KeyPress:
                if (static_cast<QKeyEvent *>(event)->key() == Qt::Key_Escape) {
                    finish(false);
                    return true;
                }
                break;
            case QEvent::ApplicationStateChange:
                if (QGuiApplication::applicationState() != Qt::ApplicationActive)
                    finish(false);
                break;
            default:
                break;
        }
        return QObject::eventFilter(watched, event);
    }
}

#include "moc_DockingDragSource_p.cpp"
