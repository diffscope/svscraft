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

#ifndef SVSCRAFT_STATUSTEXTCONTEXT_H
#define SVSCRAFT_STATUSTEXTCONTEXT_H

#include <QObject>
#include <qqmlintegration.h>

#include <SVSCraftQuick/SVSCraftQuickGlobal.h>

class QQuickWindow;

namespace SVS {

    class StatusTextContextPrivate;

    class SVSCRAFT_QUICK_EXPORT StatusTextContext : public QObject {
        Q_OBJECT
        QML_ELEMENT
        Q_DECLARE_PRIVATE(StatusTextContext)

        Q_PROPERTY(QString text READ text NOTIFY textChanged)

    public:
        explicit StatusTextContext(QObject *parent = nullptr);
        ~StatusTextContext() override;

        QString text() const;

        Q_INVOKABLE void push(QObject *contextObject, const QString &text);
        Q_INVOKABLE void update(QObject *contextObject, const QString &text);
        Q_INVOKABLE void pop(QObject *contextObject);

        static void setStatusContext(QQuickWindow *window, StatusTextContext *context);
        static StatusTextContext *statusContext(QQuickWindow *window);

        static void setContextHelpContext(QQuickWindow *window, StatusTextContext *context);
        static StatusTextContext *contextHelpContext(QQuickWindow *window);

    Q_SIGNALS:
        void textChanged();

    private:
        QScopedPointer<StatusTextContextPrivate> d_ptr;
    };

}

#endif // SVSCRAFT_STATUSTEXTCONTEXT_H
