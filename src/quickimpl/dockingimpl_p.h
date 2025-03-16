#ifndef SVSCRAFT_DOCKINGIMPL_P_H
#define SVSCRAFT_DOCKINGIMPL_P_H

#include <QObject>
#include <QPointer>
#include <qqmlintegration.h>

class QQuickWindow;

namespace SVS {

    class DockingImpl;

    class DockingImplAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(DockingImpl)
        QML_ATTACHED(DockingImpl)
    public:
        static DockingImpl *qmlAttachedProperties(QObject *object);
    };

    class DockingImpl : public QObject {
        Q_OBJECT
        QML_ANONYMOUS

        Q_PROPERTY(QObject *dockingView READ dockingView WRITE setDockingView NOTIFY dockingViewChanged)
        Q_PROPERTY(QQuickWindow *window READ window WRITE setWindow NOTIFY windowChanged)

    public:
        ~DockingImpl() override;

        QObject *dockingView() const;
        void setDockingView(QObject *dockingView);

        QQuickWindow *window() const;
        void setWindow(QQuickWindow *window);

    signals:
        void dockingViewChanged();
        void windowChanged();

    private:
        friend class DockingImplAttachedType;
        explicit DockingImpl(QObject *parent = nullptr);

        QPointer<QObject> m_dockingView;
        QPointer<QQuickWindow> m_window;
    };
}

#endif //DOCKINGIMPL_P_H
