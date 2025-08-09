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

#include "DescriptiveText_p.h"
#include "DescriptiveText_p_p.h"

#include <QQuickItem>
#include <QQuickWindow>
#include <QQmlEngine>
#include <QQmlInfo>

#include <SVSCraftQuickImpl/private/StatusText_p_p.h>

namespace SVS {

    DescriptiveText *DescriptiveTextAttachedType::qmlAttachedProperties(QObject *object) {
        QQuickItem *item = qobject_cast<QQuickItem *>(object);
        if (!item)
            qmlWarning(object) << "DescriptiveText should be attached to an Item";
        auto descriptiveText = new DescriptiveText(item);
        auto engine = qmlEngine(item);
        QQmlComponent component(engine);
        component.setData("import SVSCraft.UIComponents.impl;DescriptiveTextWrapper{}", {});
        if (component.isError())
            qmlWarning(object) << component.errorString();
        auto *wrapper = component.createWithInitialProperties({
            {"descriptiveText", QVariant::fromValue(descriptiveText)},
            {"attachee", QVariant::fromValue(item)},
        });
        if (wrapper) {
            wrapper->setParent(descriptiveText);
        }
        return descriptiveText;
    }

    DescriptiveText::DescriptiveText(QQuickItem *parent) : QObject(parent), d_ptr(new DescriptiveTextPrivate) {
        Q_D(DescriptiveText);
        d->q_ptr = this;
        d->item = parent;
        connect(parent, &QQuickItem::windowChanged, this, [=] {
            if (d->statusText) {
                d->statusText->popStatusText(this);
                d->statusText->popContextHelpText(this, d->contextHelpDelay);
                disconnect(d->statusText, nullptr, this, nullptr);
            }
            d->statusText = nullptr;
            auto *window = parent->window();
            if (window) {
                d->statusText = qobject_cast<StatusText *>(qmlAttachedPropertiesObject<StatusTextAttachedType>(parent->window()));
                connect(d->statusText, &StatusText::statusTextChanged, this, &DescriptiveText::statusTextChanged);
                connect(d->statusText, &StatusText::contextHelpTextChanged, this, &DescriptiveText::contextHelpTextChanged);
            }
            emit statusTextChanged();
        });
        d->statusText = qobject_cast<StatusText *>(qmlAttachedPropertiesObject<StatusTextAttachedType>(parent->window()));
        if (d->statusText) {
            connect(d->statusText, &StatusText::statusTextChanged, this, &DescriptiveText::statusTextChanged);
            connect(d->statusText, &StatusText::contextHelpTextChanged, this, &DescriptiveText::contextHelpTextChanged);
        }
        connect(this, &DescriptiveText::activatedChanged, [=] {
            if (!d->statusText)
                return;
            if (d->activated) {
                d->statusText->pushStatusText(this, d->statusTip);
                d->statusText->pushContextHelpText(this, d->contextHelpTip, d->contextHelpDelay);
            } else {
                d->statusText->popStatusText(this);
                d->statusText->popContextHelpText(this, d->contextHelpDelay);
            }
        });
        connect(this, &DescriptiveText::statusTipChanged, [=] {
            if (!d->statusText)
                return;
            if (d->activated) {
                d->statusText->pushStatusText(this, d->statusTip);
            }
        });
        connect(this, &DescriptiveText::contextHelpTipChanged, [=] {
            if (!d->statusText)
                return;
            if (d->activated) {
                d->statusText->pushContextHelpText(this, d->contextHelpTip, d->contextHelpDelay);
            }
        });
    }
    DescriptiveText::~DescriptiveText() {
        Q_D(DescriptiveText);
        if (d->statusText) {
            d->statusText->popStatusText(this);
            d->statusText->popContextHelpText(this, d->contextHelpDelay);
        }
    }

    bool DescriptiveText::activated() const {
        Q_D(const DescriptiveText);
        return d->activated;
    }
    void DescriptiveText::setActivated(bool activated) {
        Q_D(DescriptiveText);
        if (d->activated != activated) {
            d->activated = activated;
            emit activatedChanged();
        }
    }

    QString DescriptiveText::toolTip() const {
        Q_D(const DescriptiveText);
        return d->toolTip;
    }
    void DescriptiveText::setToolTip(const QString &toolTip) {
        Q_D(DescriptiveText);
        if (d->toolTip != toolTip) {
            d->toolTip = toolTip;
            emit toolTipChanged();
        }
    }
    QString DescriptiveText::statusTip() const {
        Q_D(const DescriptiveText);
        return d->statusTip;
    }
    void DescriptiveText::setStatusTip(const QString &statusTip) {
        Q_D(DescriptiveText);
        if (d->statusTip != statusTip) {
            d->statusTip = statusTip;
            emit statusTipChanged();
        }
    }
    QString DescriptiveText::contextHelpTip() const {
        Q_D(const DescriptiveText);
        return d->contextHelpTip;
    }
    void DescriptiveText::setContextHelpTip(const QString &contextHelpTip) {
        Q_D(DescriptiveText);
        if (d->contextHelpTip != contextHelpTip) {
            d->contextHelpTip = contextHelpTip;
            emit contextHelpTipChanged();
        }
    }
    int DescriptiveText::contextHelpDelay() const {
        Q_D(const DescriptiveText);
        return d->contextHelpDelay;
    }
    void DescriptiveText::setContextHelpDelay(int contextHelpDelay) {
        Q_D(DescriptiveText);
        if (d->contextHelpDelay != contextHelpDelay) {
            d->contextHelpDelay = contextHelpDelay;
            emit contextHelpDelayChanged();
        }
    }
    bool DescriptiveText::bindAccessibleDescription() const {
        Q_D(const DescriptiveText);
        return d->bindAccessibleDescription;
    }
    void DescriptiveText::setBindAccessibleDescription(bool bindAccessibleDescription) {
        Q_D(DescriptiveText);
        if (d->bindAccessibleDescription != bindAccessibleDescription) {
            d->bindAccessibleDescription = bindAccessibleDescription;
            emit bindAccessibleDescriptionChanged();
        }
    }
    QString DescriptiveText::statusText() const {
        Q_D(const DescriptiveText);
        return d->statusText ? d->statusText->statusText() : QString();
    }
    QString DescriptiveText::contextHelpText() const {
        Q_D(const DescriptiveText);
        return d->statusText ? d->statusText->contextHelpText() : QString();
    }

}