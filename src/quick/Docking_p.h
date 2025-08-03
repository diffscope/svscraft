/******************************************************************************
 * Copyright (c) 2025 OpenVPI                                                 *
 *                                                                            *
 * This file is part of SVSCraft                                              *
 *                                                                            *
 * SVSCraft is free software: you can redistribute it and/or modify it under  *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License, or (at your     *
 * option) any later version.                                                 *
 *                                                                            *
 * SVSCraft is distributed in the hope that it will be useful, but WITHOUT    *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public        *
 * License for more details.                                                  *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with SVSCraft. If not, see <https://www.gnu.org/licenses/>.          *
 ******************************************************************************/

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
        Q_PROPERTY(QRect windowGeometryHint READ windowGeometryHint WRITE setWindowGeometryHint NOTIFY windowGeometryHintChanged)
    public:
        ~Docking() override;

        QObject *dockingView() const;
        QQuickWindow *window() const;

        QRect windowGeometryHint() const;
        void setWindowGeometryHint(const QRect &geometryHint);

    signals:
        void dockingViewChanged();
        void windowChanged();
        void windowGeometryHintChanged();

    private:
        friend class DockingAttachedType;
        explicit Docking(QObject *parent = nullptr);

        QScopedPointer<DockingPrivate> d_ptr;
    };

}

#endif // SVSCRAFT_DOCKING_P_H
