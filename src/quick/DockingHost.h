// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGHOST_H
#define SVSCRAFT_DOCKINGHOST_H

#include <QObject>
#include <QScopedPointer>
#include <qqmlintegration.h>

#include <SVSCraftQuick/DockingTypes.h>

class QQuickWindow;

Q_MOC_INCLUDE(<SVSCraftQuick / DockingContext.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingLayout.h>)
Q_MOC_INCLUDE(<QQuickWindow>)

namespace SVS {
    class DockingContext;
    class DockingLayout;
    class DockingHostPrivate;

    class SVSCRAFT_QUICK_EXPORT DockingHost : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DockingHost)
        QML_ELEMENT
        Q_PROPERTY(QString hostId READ hostId WRITE setHostId NOTIFY hostIdChanged)
        Q_PROPERTY(Role role READ role WRITE setRole NOTIFY roleChanged)
        Q_PROPERTY(DockingContext *context READ context WRITE setContext NOTIFY contextChanged)
        Q_PROPERTY(QQuickWindow *window READ window WRITE setWindow NOTIFY windowChanged)
        Q_PROPERTY(DockingLayout *layout READ layout CONSTANT)
        Q_PROPERTY(bool empty READ isEmpty NOTIFY emptyChanged)
    public:
        enum Role {
            Embedded,
            Floating
        };
        Q_ENUM(Role)
        explicit DockingHost(QObject *parent = nullptr);
        ~DockingHost() override;
        QString hostId() const;
        void setHostId(const QString &id);
        Role role() const;
        void setRole(Role role);
        DockingContext *context() const;
        void setContext(DockingContext *context);
        QQuickWindow *window() const;
        void setWindow(QQuickWindow *window);
        DockingLayout *layout() const;
        bool isEmpty() const;
        Q_INVOKABLE DockingResult close();
    Q_SIGNALS:
        void hostIdChanged();
        void roleChanged();
        void contextChanged();
        void windowChanged();
        void emptyChanged();
        void closeRequested();

    private:
        friend class DockingContext;
        friend class DockingView;
        QScopedPointer<DockingHostPrivate> d_ptr;
    };
}

#endif
