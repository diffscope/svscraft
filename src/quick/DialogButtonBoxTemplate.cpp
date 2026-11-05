// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DialogButtonBoxTemplate_p.h"
#include "DialogButtonBoxTemplate_p_p.h"

#include <algorithm>
#include <utility>

#include <QKeyEvent>
#include <QQuickWindow>
#include <QScopedValueRollback>
#include <QtQml/qqml.h>

#include <QtQuick/private/qquickitem_p.h>
#include <QtQuickTemplates2/private/qquickdialogbuttonbox_p_p.h>
#include <QtQuickTemplates2/private/qquickpopupitem_p_p.h>

#include <SVSCraftQuick/private/DialogButton_p.h>

namespace SVS {

    namespace {

        DialogButton *buttonProperties(QObject *object) {
            return qobject_cast<DialogButton *>(qmlAttachedPropertiesObject<DialogButton>(object, true));
        }

        QQuickDialogButtonBoxAttached *behaviorProperties(QObject *object) {
            return qobject_cast<QQuickDialogButtonBoxAttached *>(qmlAttachedPropertiesObject<QQuickDialogButtonBox>(object, true));
        }

        int effectiveRole(QQuickAbstractButton *button) {
            const auto role = buttonProperties(button)->layoutRole();
            return role == DialogButton::AutoRole ? int(behaviorProperties(button)->buttonRole()) : int(role);
        }

        bool participatesInLayout(QQuickItem *item) {
            return QQuickItemPrivate::get(item)->explicitVisible;
        }

        QSizeF itemSize(QQuickItem *item) {
            const auto itemPrivate = QQuickItemPrivate::get(item);
            return {qMax(0.0, itemPrivate->widthValid() ? item->width() : item->implicitWidth()),
                    qMax(0.0, itemPrivate->heightValid() ? item->height() : item->implicitHeight())};
        }

        QList<QQuickItem *> visibleItems(const QList<QQuickItem *> &items) {
            QList<QQuickItem *> result;
            for (auto item : items) {
                if (participatesInLayout(item))
                    result.append(item);
            }
            return result;
        }

        qreal groupWidth(const QList<QQuickItem *> &items, qreal spacing) {
            qreal width = qMax(qsizetype(0), items.size() - 1) * spacing;
            for (auto item : items)
                width += itemSize(item).width();
            return width;
        }

    }

    void DialogButtonBoxTemplatePrivate::appendContent(QQmlListProperty<QQuickItem> *property, QQuickItem *item) {
        auto q = static_cast<DialogButtonBoxTemplate *>(property->object);
        auto d = q->d_func();
        auto items = static_cast<QList<QQuickItem *> *>(property->data);
        if (!item || items->contains(item))
            return;

        q->removeItem(item);
        auto otherItems = items == &d->leadingItems ? &d->accessoryItems : &d->leadingItems;
        if (otherItems->removeOne(item))
            d->contentChanged(otherItems);

        items->append(item);
        if (!item->parent())
            item->setParent(q);
        item->setParentItem(q->contentItem() ? q->contentItem() : q);
        d->watchItem(item);
        d->contentChanged(items);
    }

    qsizetype DialogButtonBoxTemplatePrivate::contentCount(QQmlListProperty<QQuickItem> *property) {
        return static_cast<QList<QQuickItem *> *>(property->data)->size();
    }

    QQuickItem *DialogButtonBoxTemplatePrivate::contentAt(QQmlListProperty<QQuickItem> *property, qsizetype index) {
        return static_cast<QList<QQuickItem *> *>(property->data)->value(index);
    }

    void DialogButtonBoxTemplatePrivate::clearContent(QQmlListProperty<QQuickItem> *property) {
        auto q = static_cast<DialogButtonBoxTemplate *>(property->object);
        auto d = q->d_func();
        auto items = static_cast<QList<QQuickItem *> *>(property->data);
        const auto previous = std::exchange(*items, {});
        for (auto item : previous) {
            d->unwatchItem(item);
            item->setParentItem(nullptr);
        }
        d->contentChanged(items);
    }

    void DialogButtonBoxTemplatePrivate::watchItem(QQuickItem *item) {
        Q_Q(DialogButtonBoxTemplate);
        if (connections.contains(item))
            return;

        auto &itemConnections = connections[item];
        const auto schedule = [this] { scheduleLayout(); };
        itemConnections.append(QObject::connect(item, &QQuickItem::implicitWidthChanged, q, schedule));
        itemConnections.append(QObject::connect(item, &QQuickItem::implicitHeightChanged, q, schedule));
        itemConnections.append(QObject::connect(item, &QQuickItem::widthChanged, q, schedule));
        itemConnections.append(QObject::connect(item, &QQuickItem::heightChanged, q, schedule));
        itemConnections.append(QObject::connect(item, &QQuickItem::visibleChanged, q, schedule));
        itemConnections.append(QObject::connect(item, &QObject::destroyed, q, [this, item] {
            if (leadingItems.removeOne(item))
                contentChanged(&leadingItems);
            if (accessoryItems.removeOne(item))
                contentChanged(&accessoryItems);
            connections.remove(item);
            scheduleLayout();
        }));

        if (auto button = qobject_cast<QQuickAbstractButton *>(item)) {
            itemConnections.append(QObject::connect(behaviorProperties(button), &QQuickDialogButtonBoxAttached::buttonRoleChanged, q, schedule));
            auto properties = buttonProperties(button);
            itemConnections.append(QObject::connect(properties, &DialogButton::layoutRoleChanged, q, schedule));
            itemConnections.append(QObject::connect(properties, &DialogButton::emphasisChanged, q, schedule));
        }
    }

    void DialogButtonBoxTemplatePrivate::unwatchItem(QQuickItem *item) {
        for (const auto &connection : connections.take(item))
            QObject::disconnect(connection);
    }

    void DialogButtonBoxTemplatePrivate::contentChanged(QList<QQuickItem *> *items) {
        Q_Q(DialogButtonBoxTemplate);
        if (items == &leadingItems)
            Q_EMIT q->leadingContentChanged();
        else
            Q_EMIT q->accessoryContentChanged();
        scheduleLayout();
    }

    void DialogButtonBoxTemplatePrivate::scheduleLayout() {
        Q_Q(DialogButtonBoxTemplate);
        if (!updatingLayout)
            q->polish();
    }

    QList<QQuickAbstractButton *> DialogButtonBoxTemplatePrivate::orderedButtons() const {
        Q_Q(const DialogButtonBoxTemplate);
        QList<int> roles;
        const auto layout = QPlatformDialogHelper::buttonLayout(Qt::Horizontal, q->buttonLayout());
        for (auto entry = layout; *entry != QPlatformDialogHelper::EOL; ++entry) {
            const auto role = *entry & ~QPlatformDialogHelper::Reverse;
            if (role != QPlatformDialogHelper::Stretch)
                roles.append(role);
        }
        auto orderedButtons = buttons;
        const auto rank = [&roles](QQuickAbstractButton *button) {
            const auto index = roles.indexOf(effectiveRole(button));
            return index < 0 ? roles.size() : index;
        };
        std::stable_sort(orderedButtons.begin(), orderedButtons.end(), [&rank](auto first, auto second) {
            return rank(first) < rank(second);
        });
        return orderedButtons;
    }

    void DialogButtonBoxTemplatePrivate::updateLayout() {
        Q_Q(DialogButtonBoxTemplate);
        if (updatingLayout || !q->contentItem())
            return;
        QScopedValueRollback guard(updatingLayout, true);
        const auto sortedButtons = orderedButtons();

        QQuickAbstractButton *primary = nullptr;
        QQuickAbstractButton *automaticPrimary = nullptr;
        qsizetype affirmativeCount = 0;
        for (auto button : std::as_const(buttons)) {
            if (!participatesInLayout(button))
                continue;
            const auto emphasis = buttonProperties(button)->emphasis();
            if (!primary && emphasis == DialogButton::Primary)
                primary = button;
            const auto role = effectiveRole(button);
            if (emphasis == DialogButton::Auto && (role == QPlatformDialogHelper::AcceptRole || role == QPlatformDialogHelper::YesRole)) {
                automaticPrimary = button;
                ++affirmativeCount;
            }
        }
        if (!primary && affirmativeCount == 1)
            primary = automaticPrimary;
        for (auto button : std::as_const(buttons))
            buttonProperties(button)->setManaged(true, button == primary);

        auto leading = visibleItems(leadingItems);
        auto trailing = visibleItems(accessoryItems);
        for (auto button : sortedButtons) {
            if (participatesInLayout(button))
                trailing.append(button);
        }

        const auto spacing = q->spacing();
        const auto leadingWidth = groupWidth(leading, spacing);
        const auto trailingWidth = groupWidth(trailing, spacing);
        const auto groupSpacing = !leading.isEmpty() && !trailing.isEmpty() ? spacing : 0;
        const auto width = leadingWidth + groupSpacing + trailingWidth;
        qreal height = 0;
        const auto items = leading + trailing;
        for (auto item : items)
            height = qMax(height, itemSize(item).height());
        const auto populated = !items.isEmpty();
        if (!qFuzzyCompare(implicitWidth, width) || !qFuzzyCompare(implicitHeight, height) || hasContent != populated) {
            implicitWidth = width;
            implicitHeight = height;
            hasContent = populated;
            Q_EMIT q->layoutChanged();
        }

        auto content = q->contentItem();
        content->setPosition({q->leftPadding(), q->topPadding()});
        content->setSize({q->availableWidth(), q->availableHeight()});
        const auto availableWidth = q->availableWidth();
        const auto availableHeight = q->availableHeight();
        qreal trailingStart = availableWidth - trailingWidth;
        if (q->alignment().testFlag(Qt::AlignHCenter))
            trailingStart = (availableWidth - trailingWidth) / 2;
        else if (q->alignment().testFlag(Qt::AlignLeft))
            trailingStart = 0;
        trailingStart = qMax(leadingWidth + groupSpacing, trailingStart);

        const auto positionGroup = [q, spacing, availableWidth, availableHeight](const QList<QQuickItem *> &group, qreal x) {
            for (auto item : group) {
                const auto size = itemSize(item);
                auto itemPrivate = QQuickItemPrivate::get(item);
                if (!itemPrivate->widthValid()) {
                    item->setWidth(size.width());
                    itemPrivate->widthValidFlag = false;
                }
                if (!itemPrivate->heightValid()) {
                    item->setHeight(size.height());
                    itemPrivate->heightValidFlag = false;
                }
                item->setPosition({q->isMirrored() ? availableWidth - x - size.width() : x,
                                   qMax(0.0, (availableHeight - size.height()) / 2)});
                x += size.width() + spacing;
            }
        };
        positionGroup(leading, 0);
        positionGroup(trailing, trailingStart);

        QQuickItem *previous = nullptr;
        for (auto item : items) {
            if (previous)
                item->stackAfter(previous);
            previous = item;
        }
    }

    void DialogButtonBoxTemplatePrivate::updateKeyTarget() {
        Q_Q(DialogButtonBoxTemplate);
        QQuickItem *target = q->window() ? q->window()->contentItem() : nullptr;
        for (auto parent = q->parentItem(); parent; parent = parent->parentItem()) {
            if (qobject_cast<QQuickPopupItem *>(parent)) {
                target = parent;
                break;
            }
        }
        if (keyTarget == target)
            return;
        if (keyTarget)
            keyTarget->removeEventFilter(q);
        keyTarget = target;
        if (keyTarget)
            keyTarget->installEventFilter(q);
    }

    QQuickAbstractButton *DialogButtonBoxTemplatePrivate::effectiveDefaultButton() const {
        Q_Q(const DialogButtonBoxTemplate);
        auto button = q->defaultButton();
        if (!button)
            button = q->standardButton(q->defaultStandardButton());
        return buttons.contains(button) ? button : nullptr;
    }

    DialogButtonBoxTemplate::DialogButtonBoxTemplate(QQuickItem *parent) : QQuickDialogButtonBox(parent), d_ptr(new DialogButtonBoxTemplatePrivate) {
        Q_D(DialogButtonBoxTemplate);
        d->q_ptr = this;
        const auto schedule = [d] { d->scheduleLayout(); };
        connect(this, &QQuickControl::spacingChanged, this, schedule);
        connect(this, &QQuickControl::paddingChanged, this, schedule);
        connect(this, &QQuickControl::leftPaddingChanged, this, schedule);
        connect(this, &QQuickControl::rightPaddingChanged, this, schedule);
        connect(this, &QQuickControl::topPaddingChanged, this, schedule);
        connect(this, &QQuickControl::bottomPaddingChanged, this, schedule);
        connect(this, &QQuickControl::mirroredChanged, this, schedule);
        connect(this, &QQuickDialogButtonBox::alignmentChanged, this, schedule);
        connect(this, &QQuickDialogButtonBox::buttonLayoutChanged, this, schedule);
        connect(this, &QQuickItem::parentChanged, this, [d] { d->updateKeyTarget(); });
        connect(this, &QQuickItem::windowChanged, this, [d] { d->updateKeyTarget(); });
    }

    DialogButtonBoxTemplate::~DialogButtonBoxTemplate() {
        Q_D(DialogButtonBoxTemplate);
        if (d->keyTarget)
            d->keyTarget->removeEventFilter(this);
        for (const auto &connections : std::as_const(d->connections)) {
            for (const auto &connection : connections)
                disconnect(connection);
        }
    }

    QQmlListProperty<QQuickItem> DialogButtonBoxTemplate::leadingContent() {
        Q_D(DialogButtonBoxTemplate);
        return {this, &d->leadingItems, d->appendContent, d->contentCount, d->contentAt, d->clearContent};
    }

    QQmlListProperty<QQuickItem> DialogButtonBoxTemplate::accessoryContent() {
        Q_D(DialogButtonBoxTemplate);
        return {this, &d->accessoryItems, d->appendContent, d->contentCount, d->contentAt, d->clearContent};
    }

    qreal DialogButtonBoxTemplate::layoutImplicitWidth() const {
        Q_D(const DialogButtonBoxTemplate);
        return d->implicitWidth;
    }

    qreal DialogButtonBoxTemplate::layoutImplicitHeight() const {
        Q_D(const DialogButtonBoxTemplate);
        return d->implicitHeight;
    }

    bool DialogButtonBoxTemplate::hasContent() const {
        Q_D(const DialogButtonBoxTemplate);
        return d->hasContent;
    }

    QPlatformDialogHelper::StandardButton DialogButtonBoxTemplate::standardButtonId(QQuickAbstractButton *button) const {
        if (!button)
            return QPlatformDialogHelper::NoButton;
        const auto attached = qobject_cast<QQuickDialogButtonBoxAttached *>(qmlAttachedPropertiesObject<QQuickDialogButtonBox>(button, false));
        return attached ? QQuickDialogButtonBoxAttachedPrivate::get(attached)->standardButton : QPlatformDialogHelper::NoButton;
    }

    QList<QQuickAbstractButton *> DialogButtonBoxTemplate::buttonsInLayoutOrder() const {
        Q_D(const DialogButtonBoxTemplate);
        auto buttons = d->orderedButtons();
        buttons.removeIf([](auto button) { return !participatesInLayout(button); });
        return buttons;
    }

    void DialogButtonBoxTemplate::selectDefaultButton(QQuickAbstractButton *button) {
        if (defaultButton() == button)
            return;
#if QT_VERSION >= QT_VERSION_CHECK(6, 11, 0)
        QQuickDialogButtonBoxPrivate::get(this)->defaultButton = nullptr;
        if (button)
            QQuickDialogButtonBox::setDefaultButton(button);
        else
            Q_EMIT defaultButtonChanged();
#else
        setDefaultButton(button);
#endif
    }

#if QT_VERSION < QT_VERSION_CHECK(6, 11, 0)
    QQuickAbstractButton *DialogButtonBoxTemplate::defaultButton() const {
        Q_D(const DialogButtonBoxTemplate);
        return d->defaultButton;
    }

    void DialogButtonBoxTemplate::setDefaultButton(QQuickAbstractButton *button) {
        Q_D(DialogButtonBoxTemplate);
        if (d->defaultButton == button)
            return;
        d->defaultButton = button;
        if (button && !d->buttons.contains(button))
            addItem(button);
        Q_EMIT defaultButtonChanged();
    }

    QPlatformDialogHelper::StandardButton DialogButtonBoxTemplate::defaultStandardButton() const {
        Q_D(const DialogButtonBoxTemplate);
        return d->defaultStandardButton;
    }

    void DialogButtonBoxTemplate::setDefaultStandardButton(QPlatformDialogHelper::StandardButton button) {
        Q_D(DialogButtonBoxTemplate);
        if (d->defaultStandardButton == button)
            return;
        d->defaultStandardButton = button;
        Q_EMIT defaultStandardButtonChanged();
    }
#endif

    void DialogButtonBoxTemplate::componentComplete() {
        QQuickContainer::componentComplete();
        Q_D(DialogButtonBoxTemplate);
        d->updateKeyTarget();
        d->updateLayout();
    }

    void DialogButtonBoxTemplate::updatePolish() {
        QQuickContainer::updatePolish();
        Q_D(DialogButtonBoxTemplate);
        d->updateLayout();
    }

    void DialogButtonBoxTemplate::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
        QQuickContainer::geometryChange(newGeometry, oldGeometry);
        Q_D(DialogButtonBoxTemplate);
        d->scheduleLayout();
    }

    void DialogButtonBoxTemplate::contentItemChange(QQuickItem *newItem, QQuickItem *oldItem) {
        QQuickDialogButtonBox::contentItemChange(newItem, oldItem);
        Q_D(DialogButtonBoxTemplate);
        if (newItem) {
            for (auto item : d->leadingItems + d->accessoryItems)
                item->setParentItem(newItem);
            for (auto button : std::as_const(d->buttons))
                button->setParentItem(newItem);
        }
        d->scheduleLayout();
    }

    void DialogButtonBoxTemplate::itemChange(ItemChange change, const ItemChangeData &data) {
        QQuickContainer::itemChange(change, data);
    }

    bool DialogButtonBoxTemplate::isContent(QQuickItem *item) const {
        Q_D(const DialogButtonBoxTemplate);
        return !d->leadingItems.contains(item) && !d->accessoryItems.contains(item) && QQuickDialogButtonBox::isContent(item);
    }

    void DialogButtonBoxTemplate::itemAdded(int index, QQuickItem *item) {
        QQuickDialogButtonBox::itemAdded(index, item);
        Q_D(DialogButtonBoxTemplate);
        if (auto button = qobject_cast<QQuickAbstractButton *>(item)) {
            d->buttons.insert(qBound(0, index, int(d->buttons.size())), button);
            d->watchItem(button);
        }
        d->scheduleLayout();
    }

    void DialogButtonBoxTemplate::itemRemoved(int index, QQuickItem *item) {
        QQuickDialogButtonBox::itemRemoved(index, item);
        Q_D(DialogButtonBoxTemplate);
        const auto iterator = std::ranges::find(d->buttons, item);
        if (iterator != d->buttons.end()) {
            auto button = *iterator;
            d->buttons.erase(iterator);
            if (!QQuickItemPrivate::get(item)->inDestructor)
                buttonProperties(button)->setManaged(false);
            if (defaultButton() == button)
                selectDefaultButton(nullptr);
        }
        d->unwatchItem(item);
        d->scheduleLayout();
    }

    bool DialogButtonBoxTemplate::eventFilter(QObject *watched, QEvent *event) {
        Q_D(DialogButtonBoxTemplate);
        if (watched != d->keyTarget || !isVisible() || !isEnabled() || !window())
            return QQuickDialogButtonBox::eventFilter(watched, event);
        if (event->type() != QEvent::KeyPress && event->type() != QEvent::KeyRelease)
            return false;
        auto keyEvent = static_cast<QKeyEvent *>(event);
        if ((keyEvent->key() != Qt::Key_Return && keyEvent->key() != Qt::Key_Enter) || (keyEvent->modifiers() & ~Qt::KeypadModifier))
            return false;

        for (auto item = window()->activeFocusItem(); item && item != d->keyTarget; item = item->parentItem()) {
            if (qobject_cast<QQuickPopupItem *>(item))
                return false;
        }
        auto button = d->effectiveDefaultButton();
        if (!button || !button->isVisible() || !button->isEnabled())
            return false;
        keyEvent->accept();
        if (event->type() == QEvent::KeyPress && !keyEvent->isAutoRepeat())
            button->animateClick();
        return true;
    }

}
