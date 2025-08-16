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

#include "DescriptiveAction_p.h"
#include "DescriptiveAction_p_p.h"

namespace SVS {

    DescriptiveAction *DescriptiveActionAttachedType::qmlAttachedProperties(QObject *object) {
        return new DescriptiveAction(object);
    }

    DescriptiveAction::DescriptiveAction(QObject *parent)
        : QObject(parent), d_ptr(new DescriptiveActionPrivate) {
        Q_D(DescriptiveAction);
        d->q_ptr = this;
    }
    DescriptiveAction::~DescriptiveAction() = default;

    QString DescriptiveAction::statusTip() const {
        Q_D(const DescriptiveAction);
        return d->statusTip;
    }
    void DescriptiveAction::setStatusTip(const QString &value) {
        Q_D(DescriptiveAction);
        if (d->statusTip != value) {
            d->statusTip = value;
            emit statusTipChanged();
        }
    }
    QString DescriptiveAction::contextHelpTip() const {
        Q_D(const DescriptiveAction);
        return d->contextHelpTip;
    }
    void DescriptiveAction::setContextHelpTip(const QString &value) {
        Q_D(DescriptiveAction);
        if (d->contextHelpTip != value) {
            d->contextHelpTip = value;
            emit contextHelpTipChanged();
        }
    }

}
