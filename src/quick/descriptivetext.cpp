#include "descriptivetext_p.h"
#include "descriptivetext_p_p.h"

#include <QQuickItem>
#include <QQuickWindow>
#include <QQmlEngine>
#include <QQmlInfo>

#include <SVSCraftQuick/private/statustext_p_p.h>

namespace SVS {

    DescriptiveText *DescriptiveTextAttachedType::qmlAttachedProperties(QObject *object) {
        QQuickItem *item = qobject_cast<QQuickItem *>(object);
        if (!item)
            qmlWarning(object) << "DescriptiveText should be attached to an Item";
        auto descriptiveText = new DescriptiveText(item);
        auto engine = qmlEngine(item);
        QQmlComponent component(engine);
        component.setData(R"qml(
            import QtQml
            import QtQuick
            import QtQuick.Controls
            import SVSCraft.UIComponents

            Item {
                id: wrapper
                required property QtObject descriptiveText
                required property Item attachee

                Binding {
                    wrapper.attachee.ToolTip.visible: wrapper.descriptiveText.activated
                    wrapper.attachee.ToolTip.text: wrapper.descriptiveText.toolTip
                    wrapper.attachee.ToolTip.delay: Theme.toolTipDelay
                    wrapper.attachee.ToolTip.timeout: Theme.toolTipTimeout
                    wrapper.attachee.Accessible.description: wrapper.descriptiveText.toolTip && wrapper.descriptiveText.statusTip ? wrapper.descriptiveText.toolTip + "\n" + wrapper.descriptiveText.statusTip : (wrapper.descriptiveText.toolTip || wrapper.descriptiveText.statusTip)
                }
            }
        )qml", {});
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
        if (d->statusText)
            connect(d->statusText, &StatusText::displayTextChanged, this, &DescriptiveText::statusTextChanged);
        connect(this, &DescriptiveText::activatedChanged, [=] {
            if (d->activated) {
                if (d->statusText)
                    d->statusText->setText(d->statusTip);
            } else {
                if (d->statusText)
                    d->statusText->setText({});
            }
        });
        connect(this, &DescriptiveText::statusTipChanged, [=] {
            if (d->activated) {
                if (d->statusText)
                    d->statusText->setText(d->statusTip);
            }
        });
    }
    DescriptiveText::~DescriptiveText() = default;


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
        return d->statusText ? d->statusText->displayText() : "";
    }
}