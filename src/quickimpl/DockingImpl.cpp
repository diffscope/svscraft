#include "DockingImpl_p.h"

#include <QQmlEngine>
#include <QQuickWindow>

namespace SVS {

    DockingImpl *DockingImplAttachedType::qmlAttachedProperties(QObject *object) {
        return new DockingImpl(object);
    }


    DockingImpl::DockingImpl(QObject *parent)
        : QObject(parent), m_dockingView(nullptr), m_window(nullptr) {
    }
    DockingImpl::~DockingImpl() = default;
    QObject *DockingImpl::dockingView() const {
        return m_dockingView;
    }
    void DockingImpl::setDockingView(QObject *dockingView) {
        if (m_dockingView != dockingView) {
            m_dockingView = dockingView;
            emit dockingViewChanged();
        }
    }
    QQuickWindow *DockingImpl::window() const {
        return m_window;
    }
    void DockingImpl::setWindow(QQuickWindow *window) {
        if (m_window != window) {
            m_window = window;
            emit windowChanged();
        }
    }
}

#include "moc_DockingImpl_p.cpp"