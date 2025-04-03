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

#ifndef SVSCRAFT_DESCRIPTIVETEXT_P_P_H
#define SVSCRAFT_DESCRIPTIVETEXT_P_P_H

#include <SVSCraftQuick/private/DescriptiveText_p.h>

#include <QPointer>

#include <SVSCraftQuickImpl/private/StatusText_p.h>

namespace SVS {
    class DescriptiveTextAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(DescriptiveText)
        QML_ATTACHED(DescriptiveText)
    public:
        static DescriptiveText *qmlAttachedProperties(QObject *object);
    };;

    class DescriptiveTextPrivate {
        Q_DECLARE_PUBLIC(DescriptiveText)
    public:
        DescriptiveText *q_ptr;
        QQuickItem *item;
        bool activated = false;
        int contextHelpDelay = 0;
        QString toolTip;
        QString statusTip;
        QString contextHelpTip;

        QPointer<StatusText> statusText;
    };
}

#endif // SVSCRAFT_DESCRIPTIVETEXT_P_P_H
