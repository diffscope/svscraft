#ifndef DOCKING_P_P_H
#define DOCKING_P_P_H

#include <SVSCraftQuick/private/docking_p.h>

namespace SVS {

    class DockingImpl;

    class DockingAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(Docking)
        QML_ATTACHED(Docking)
    public:
        static Docking *qmlAttachedProperties(QObject *object);
    };

    class DockingPrivate {
        Q_DECLARE_PUBLIC(Docking)
    public:
        Docking *q_ptr;
        DockingImpl *dockingImpl;
    };
}

#endif //DOCKING_P_P_H
