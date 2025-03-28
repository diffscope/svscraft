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
                disconnect(d->statusText, nullptr, this, nullptr);
            }
            d->statusText = nullptr;
            if (parent->window()) {
                d->statusText = qobject_cast<StatusText *>(qmlAttachedPropertiesObject<StatusTextAttachedType>(parent->window()));
                connect(d->statusText, &StatusText::displayTextChanged, this, &DescriptiveText::statusTextChanged);
            }
            emit statusTextChanged();
        });
        d->statusText = qobject_cast<StatusText *>(qmlAttachedPropertiesObject<StatusTextAttachedType>(parent->window()));
        if (d->statusText) {
            connect(d->statusText, &StatusText::displayTextChanged, this, &DescriptiveText::statusTextChanged);
            connect(d->statusText, &StatusText::defaultTextChanged, this, &DescriptiveText::defaultStatusTextChanged);
        }
        connect(this, &DescriptiveText::activatedChanged, [=] {
            if (d->activated) {
                if (d->statusText)
                    d->statusText->setText(d->statusTip);
                    d->statusText->setContextObject(this);
            } else {
                if (d->statusText && d->statusText->contextObject() == this) {
                    d->statusText->setText({});
                    d->statusText->setContextObject(nullptr);
                }
            }
        });
        connect(this, &DescriptiveText::statusTipChanged, [=] {
            if (d->activated) {
                if (d->statusText) {
                    d->statusText->setText(d->statusTip);
                    d->statusText->setContextObject(this);
                }
            }
        });
    }
    DescriptiveText::~DescriptiveText() {
        Q_D(DescriptiveText);
        if (d->statusText && d->statusText->contextObject() == this) {
            d->statusText->setText({});
            d->statusText->setContextObject(nullptr);
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
    QString DescriptiveText::statusText() const {
        Q_D(const DescriptiveText);
        return d->statusText ? d->statusText->displayText() : QString();
    }
    QString DescriptiveText::defaultStatusText() const {
        Q_D(const DescriptiveText);
        return d->statusText ? d->statusText->defaultText() : QString();
    }
    void DescriptiveText::setDefaultStatusText(const QString &defaultStatusText) {
        Q_D(DescriptiveText);
        d->statusText->setDefaultText(defaultStatusText);
    }
}