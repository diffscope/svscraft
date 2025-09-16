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

#include <SVSCraftQuick/StatusTextContext.h>

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

        // Initialize status contexts from the current window
        if (parent && parent->window()) {
            d->statusContext = StatusTextContext::statusContext(parent->window());
            d->contextHelpContext = StatusTextContext::contextHelpContext(parent->window());
        }

        auto initialUpdateContext = [=] {
            if (!d->statusContext) {
                if (parent->window()) {
                    d->statusContext = StatusTextContext::statusContext(parent->window());
                }
            }
            if (!d->contextHelpContext) {
                if (parent->window()) {
                    d->contextHelpContext = StatusTextContext::contextHelpContext(parent->window());
                }
            }
        };

        // Connect statusTipChanged signal
        connect(this, &DescriptiveText::statusTipChanged, this, [=]() {
            initialUpdateContext();
            if (d->activated && !d->statusTip.isEmpty() && d->statusContext) {
                d->statusContext->update(this, d->statusTip);
            }
        });

        // Connect contextHelpTipChanged signal
        connect(this, &DescriptiveText::contextHelpTipChanged, this, [=]() {
            initialUpdateContext();
            if (d->activated && !d->contextHelpTip.isEmpty() && d->contextHelpContext) {
                d->contextHelpContext->update(this, d->contextHelpTip);
            }
        });

        // Connect activatedChanged signal
        connect(this, &DescriptiveText::activatedChanged, this, [=]() {
            initialUpdateContext();
            if (d->activated) {
                // Push status tip if not empty
                if (!d->statusTip.isEmpty() && d->statusContext) {
                    d->statusContext->push(this, d->statusTip);
                }
                // Push context help tip if not empty
                if (!d->contextHelpTip.isEmpty() && d->contextHelpContext) {
                    d->contextHelpContext->push(this, d->contextHelpTip);
                }
            } else {
                // Pop from both contexts
                if (d->statusContext) {
                    d->statusContext->pop(this);
                }
                if (d->contextHelpContext) {
                    d->contextHelpContext->pop(this);
                }
            }
        });

        // Connect to item's windowChanged signal
        connect(parent, &QQuickItem::windowChanged, this, [=](QQuickWindow *window) {
            // Store old contexts
            StatusTextContext *oldStatusContext = d->statusContext;
            StatusTextContext *oldContextHelpContext = d->contextHelpContext;
            
            // Get new contexts from the new window
            StatusTextContext *newStatusContext = window ? StatusTextContext::statusContext(window) : nullptr;
            StatusTextContext *newContextHelpContext = window ? StatusTextContext::contextHelpContext(window) : nullptr;
            
            // Update contexts
            d->statusContext = newStatusContext;
            d->contextHelpContext = newContextHelpContext;
            
            // If activated and contexts changed, update registrations
            if (d->activated) {
                // Pop from old contexts if they exist and are different
                if (oldStatusContext && oldStatusContext != newStatusContext) {
                    oldStatusContext->pop(this);
                }
                if (oldContextHelpContext && oldContextHelpContext != newContextHelpContext) {
                    oldContextHelpContext->pop(this);
                }
                
                // Push to new contexts if they exist and tips are not empty
                if (newStatusContext && !d->statusTip.isEmpty()) {
                    newStatusContext->push(this, d->statusTip);
                }
                if (newContextHelpContext && !d->contextHelpTip.isEmpty()) {
                    newContextHelpContext->push(this, d->contextHelpTip);
                }
            }
        });

    }
    DescriptiveText::~DescriptiveText() {
        Q_D(DescriptiveText);
        // Clean up status contexts when object is destroyed
        if (d->statusContext) {
            d->statusContext->pop(this);
        }
        if (d->contextHelpContext) {
            d->contextHelpContext->pop(this);
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

}
