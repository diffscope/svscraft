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

#ifndef SVSCRAFT_STATUSTEXT_P_H
#define SVSCRAFT_STATUSTEXT_P_H

#include <QObject>
#include <qqmlintegration.h>

#include <SVSCraftQuickImpl/SVSCraftQuickImplGlobal.h>

namespace SVS {

    class StatusTextAttachedType;

    class StatusTextPrivate;

    class SVSCRAFT_QUICK_IMPL_EXPORT StatusText : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(StatusText)
        QML_ANONYMOUS

    public:
        ~StatusText() override;

        void pushStatusText(QObject *contextObject, const QString &text);
        void popStatusText(QObject *contextObject);

        void pushContextHelpText(QObject *contextObject, const QString &text, int delay);
        void popContextHelpText(QObject *contextObject, int delay);

        QString statusText() const;
        QString contextHelpText() const;

    signals:
        void statusTextChanged();
        void contextHelpTextChanged();

    private:
        friend class StatusTextAttachedType;
        explicit StatusText(QObject *parent = nullptr);

        QScopedPointer<StatusTextPrivate> d_ptr;

    };

}

#endif // SVSCRAFT_STATUSTEXT_P_H
