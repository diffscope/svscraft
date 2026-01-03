/******************************************************************************
 * Copyright (c) 2026 OpenVPI                                                 *
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

#ifndef SVSCRAFT_CURSORBINDING_P_H
#define SVSCRAFT_CURSORBINDING_P_H

#include <QObject>
#include <qqmlintegration.h>
#include <QtGui/QCursor>

class QQuickItem;

namespace SVS {

    class CursorBindingAttachedType;
    class CursorBindingPrivate;

    class CursorBinding : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(CursorBinding)
        QML_ANONYMOUS

        Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
        Q_PROPERTY(Qt::CursorShape cursorShape READ cursorShape WRITE setCursorShape NOTIFY cursorShapeChanged)
    public:
        ~CursorBinding() override;

        bool enabled() const;
        void setEnabled(bool enabled);

        Qt::CursorShape cursorShape() const;
        void setCursorShape(Qt::CursorShape cursorShape);

    Q_SIGNALS:
        void enabledChanged();
        void cursorShapeChanged();

    private:
        friend class CursorBindingAttachedType;
        explicit CursorBinding(QQuickItem *parent = nullptr);

        QScopedPointer<CursorBindingPrivate> d_ptr;
    };

}

#endif // SVSCRAFT_CURSORBINDING_P_H
