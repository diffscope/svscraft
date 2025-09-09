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

#ifndef SVSCRAFT_STATUSTEXTCONTEXT_P_H
#define SVSCRAFT_STATUSTEXTCONTEXT_P_H

#include <SVSCraftQuick/StatusTextContext.h>

#include <QList>
#include <QPair>

namespace SVS {

    class StatusTextContextWindowContext : public QObject {
        Q_OBJECT
        Q_PROPERTY(StatusTextContext *statusContext READ statusContext WRITE setStatusContext NOTIFY statusContextChanged)
        Q_PROPERTY(StatusTextContext *contextHelpContext READ contextHelpContext WRITE setContextHelpContext NOTIFY contextHelpContextChanged)
    public:
        explicit StatusTextContextWindowContext(QObject *parent = nullptr) : QObject(parent) {
        }
        ~StatusTextContextWindowContext() override = default;

        StatusTextContext *statusContext() const {
            return m_statusContext;
        }

        void setStatusContext(StatusTextContext *statusContext) {
            if (m_statusContext != statusContext) {
                m_statusContext = statusContext;
                emit statusContextChanged();
            }
        }

        StatusTextContext *contextHelpContext() const {
            return m_contextHelpContext;
        }

        void setContextHelpContext(StatusTextContext *contextHelpContext) {
            if (m_contextHelpContext != contextHelpContext) {
                m_contextHelpContext = contextHelpContext;
                emit contextHelpContextChanged();
            }
        }

    signals:
        void statusContextChanged();
        void contextHelpContextChanged();

    private:
        StatusTextContext *m_statusContext{};
        StatusTextContext *m_contextHelpContext{};
    };

    class StatusTextContextAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(StatusTextContext)
        QML_ATTACHED(StatusTextContextWindowContext)
    public:
        static StatusTextContextWindowContext *qmlAttachedProperties(QObject *object);
    };

    class StatusTextContextPrivate {
        Q_DECLARE_PUBLIC(StatusTextContext)
    public:
        StatusTextContext *q_ptr;
        QList<QPair<QObject *, QString>> contextList;

        void updateTextProperty();
    };

}

#endif // SVSCRAFT_STATUSTEXTCONTEXT_P_H
