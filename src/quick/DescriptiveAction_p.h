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

#ifndef SVSCRAFT_DESCRIPTIVEACTION_P_H
#define SVSCRAFT_DESCRIPTIVEACTION_P_H

#include <QObject>
#include <qqmlintegration.h>

namespace SVS {

    class DescriptiveActionAttachedType;
    class DescriptiveActionPrivate;

    class DescriptiveAction : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DescriptiveAction)
        QML_ANONYMOUS

        Q_PROPERTY(QString statusTip READ statusTip WRITE setStatusTip NOTIFY statusTipChanged)
        Q_PROPERTY(QString contextHelpTip READ contextHelpTip WRITE setContextHelpTip NOTIFY contextHelpTipChanged)
    public:
        ~DescriptiveAction() override;

        QString statusTip() const;
        void setStatusTip(const QString &value);

        QString contextHelpTip() const;
        void setContextHelpTip(const QString &value);

    signals:
        void statusTipChanged();
        void contextHelpTipChanged();

    private:
        friend class DescriptiveActionAttachedType;
        explicit DescriptiveAction(QObject *parent = nullptr);
        QScopedPointer<DescriptiveActionPrivate> d_ptr;
    };

}

#endif // SVSCRAFT_DESCRIPTIVEACTION_P_H
