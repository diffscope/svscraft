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

        QString text() const;
        void setText(const QString &text);

        void setContextObject(QObject *object);
        QObject *contextObject() const;

        QString defaultText() const;
        void setDefaultText(const QString &defaultText);

        QString displayText() const;

    signals:
        void textChanged();
        void defaultTextChanged();
        void displayTextChanged();

    private:
        friend class StatusTextAttachedType;
        explicit StatusText(QObject *parent = nullptr);

        QScopedPointer<StatusTextPrivate> d_ptr;

    };

}

#endif // SVSCRAFT_STATUSTEXT_P_H
