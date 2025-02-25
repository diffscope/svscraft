#include "docking_p.h"
#include "docking_p_p.h"

#include <QQmlEngine>
#include <QQuickWindow>

#include <SVSCraftQuickImpl/private/dockingimpl_p.h>

namespace SVS {

    Docking *DockingAttachedType::qmlAttachedProperties(QObject *object) {
        auto o = new Docking(object);
        return o;
    }


    Docking::Docking(QObject *parent) : QObject(parent), d_ptr(new DockingPrivate) {
        Q_D(Docking);
        d->q_ptr = this;
        d->dockingImpl = qobject_cast<DockingImpl *>(qmlAttachedPropertiesObject<DockingImplAttachedType>(parent));
        connect(d->dockingImpl, &DockingImpl::dockingViewChanged, this, &Docking::dockingViewChanged);
        connect(d->dockingImpl, &DockingImpl::windowChanged, this, &Docking::windowChanged);
    }
    Docking::~Docking() = default;
    QObject *Docking::dockingView() const {
        Q_D(const Docking);
        return d->dockingImpl->dockingView();
    }
    QQuickWindow *Docking::window() const {
        Q_D(const Docking);
        return d->dockingImpl->window();
    }
}

#include "moc_docking_p.cpp"