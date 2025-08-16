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

#ifndef SVSCRAFT_DESCRIPTIVEACTION_P_P_H
#define SVSCRAFT_DESCRIPTIVEACTION_P_P_H

#include <SVSCraftQuick/private/DescriptiveAction_p.h>

namespace SVS {

    class DescriptiveActionAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(DescriptiveAction)
        QML_ATTACHED(DescriptiveAction)
    public:
        static DescriptiveAction *qmlAttachedProperties(QObject *object);
    };

    class DescriptiveActionPrivate {
        Q_DECLARE_PUBLIC(DescriptiveAction)
    public:
        DescriptiveAction *q_ptr;
        QString statusTip;
        QString contextHelpTip;
    };
}

#endif // SVSCRAFT_DESCRIPTIVEACTION_P_P_H
