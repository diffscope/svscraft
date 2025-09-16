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

#include "ActionToolTipHelper_p.h"

#include <QQmlEngine>
#include <QQmlComponent>
#include <QtQuickTemplates2/private/qquicktooltip_p.h>

namespace SVS {
    ActionToolTipHelper::ActionToolTipHelper(QObject *parent)
        : QObject(parent) {
    }

    ActionToolTipHelper::~ActionToolTipHelper() = default;

    QQuickToolTip *ActionToolTipHelper::instance(bool create) const {
        QQmlEngine *engine = qmlEngine(parent());
        if (!engine)
            return nullptr;

        static const char *name = "_svscraft_ActionToolTip";

        QQuickToolTip *tip = engine->property(name).value<QQuickToolTip *>();
        if (!tip && create) {
            // FIXME: "There are still \"1\" items in the process of being created at engine destruction."
            QQmlComponent component(engine, "SVSCraft.UIComponents", "ActionToolTip");

            QObject *object = component.create();
            if (object)
                object->setParent(engine);

            tip = qobject_cast<QQuickToolTip *>(object);
            if (!tip)
                delete object;
            else
                engine->setProperty(name, QVariant::fromValue(object));
        }
        return tip;
    }

    QString ActionToolTipHelper::text() const {
        return m_text;
    }

    void ActionToolTipHelper::setText(const QString &text) {
        if (m_text == text)
            return;

        m_text = text;
        emit textChanged();

        if (isVisible()) {
            QQuickToolTip *tip = instance(true);
            if (tip)
                tip->setText(text);
        }
    }

    int ActionToolTipHelper::delay() const {
        return m_delay;
    }

    void ActionToolTipHelper::setDelay(int delay) {
        if (m_delay == delay)
            return;

        m_delay = delay;
        emit delayChanged();

        if (isVisible()) {
            QQuickToolTip *tip = instance(true);
            if (tip)
                tip->setDelay(delay);
        }
    }

    int ActionToolTipHelper::timeout() const {
        return m_timeout;
    }

    void ActionToolTipHelper::setTimeout(int timeout) {
        if (m_timeout == timeout)
            return;

        m_timeout = timeout;
        emit timeoutChanged();

        if (isVisible()) {
            QQuickToolTip *tip = instance(true);
            if (tip)
                tip->setTimeout(timeout);
        }
    }

    bool ActionToolTipHelper::isVisible() const {
        QQuickToolTip *tip = instance(false);
        if (!tip)
            return false;

        return tip->isVisible() && tip->parentItem() == parent();
    }

    void ActionToolTipHelper::setVisible(bool visible) {
        if (visible)
            show(m_text);
        else
            hide();
    }

    QString ActionToolTipHelper::shortcut() const {
        return m_shortcut;
    }

    void ActionToolTipHelper::setShortcut(const QString &shortcut) {
        if (m_shortcut == shortcut)
            return;
        m_shortcut = shortcut;
        emit shortcutChanged();
        QQuickToolTip *tip = instance(true);
        if (tip)
            tip->setProperty("shortcut", shortcut);
    }

    QQuickToolTip *ActionToolTipHelper::toolTip() const {
        return instance(true);
    }

    void ActionToolTipHelper::show(const QString &text, int ms) {
        QQuickToolTip *tip = instance(true);
        if (!tip)
            return;

        tip->resetWidth();
        tip->resetHeight();
        tip->setParentItem(qobject_cast<QQuickItem *>(parent()));
        tip->setDelay(m_delay);
        tip->setTimeout(ms >= 0 ? ms : m_timeout);
        tip->setProperty("shortcut", m_shortcut);
        tip->show(text);
    }

    void ActionToolTipHelper::hide() {
        QQuickToolTip *tip = instance(false);
        if (!tip)
            return;
        // check the parent item to prevent unexpectedly closing tooltip by new created invisible tooltip
        if (parent() == tip->parentItem())
            tip->hide();
    }
}

#include "moc_ActionToolTipHelper_p.cpp"
