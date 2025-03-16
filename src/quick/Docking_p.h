#ifndef SVSCRAFT_DOCKING_P_H
#define SVSCRAFT_DOCKING_P_H

#include <QObject>
#include <qqmlintegration.h>

class QQuickWindow;

namespace SVS {

    class DockingAttachedType;

    class DockingPrivate;

    class Docking : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(Docking)
        QML_ANONYMOUS

        Q_PROPERTY(QObject *dockingView READ dockingView NOTIFY dockingViewChanged)
        Q_PROPERTY(QQuickWindow *window READ window NOTIFY windowChanged)
    public:
        ~Docking() override;

        QObject *dockingView() const;
        QQuickWindow *window() const;

    signals:
        void dockingViewChanged();
        void windowChanged();

    private:
        friend class DockingAttachedType;
        explicit Docking(QObject *parent = nullptr);

        QScopedPointer<DockingPrivate> d_ptr;
    };

}

#endif // SVSCRAFT_DOCKING_P_H
