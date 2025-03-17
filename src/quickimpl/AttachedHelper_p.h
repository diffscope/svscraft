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

#ifndef SVSCRAFT_ATTACHEDHELPER_P_H
#define SVSCRAFT_ATTACHEDHELPER_P_H

#include <QObject>
#include <QVariant>
#include <qqmlintegration.h>

namespace SVS {

    class AttachedHelper : public QObject {
        Q_OBJECT
        QML_ANONYMOUS
    public:
        explicit AttachedHelper(QObject *parent = nullptr);
        ~AttachedHelper() override;

        bool eventFilter(QObject *watched, QEvent *event) override;

        Q_INVOKABLE void installDoubleClickEventFilter();
        Q_INVOKABLE void removeDoubleClickEventFilter();
        Q_INVOKABLE void setProperty(const QString &key, const QVariant &value) const;

    signals:
        void doubleClicked();
    };

    class AttachedHelperAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(AttachedHelper)
        QML_ATTACHED(AttachedHelper)
    public:
        static AttachedHelper *qmlAttachedProperties(QObject *parent);
    };

}

#endif // SVSCRAFT_ATTACHEDHELPER_P_H
