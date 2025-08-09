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

#ifndef SVSCRAFT_ACTIONTOOLTIPHELPER_P_H
#define SVSCRAFT_ACTIONTOOLTIPHELPER_P_H

#include <QObject>
#include <qqmlintegration.h>

class QQuickToolTip;

namespace SVS {
    class ActionToolTipHelper : public QObject {
        Q_OBJECT
        QML_ELEMENT
        Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged FINAL)
        Q_PROPERTY(int delay READ delay WRITE setDelay NOTIFY delayChanged FINAL)
        Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged FINAL)
        Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged FINAL)
        Q_PROPERTY(QString shortcut READ shortcut WRITE setShortcut NOTIFY shortcutChanged FINAL)
        Q_PROPERTY(QQuickToolTip *toolTip READ toolTip CONSTANT FINAL)

    public:
        explicit ActionToolTipHelper(QObject *parent = nullptr);
        ~ActionToolTipHelper() override;

        QString text() const;
        void setText(const QString &text);

        int delay() const;
        void setDelay(int delay);

        int timeout() const;
        void setTimeout(int timeout);

        bool isVisible() const;
        void setVisible(bool visible);

        QString shortcut() const;
        void setShortcut(const QString &shortcut);

        QQuickToolTip *toolTip() const;

    public Q_SLOTS:
        void show(const QString &text, int ms = -1);
        void hide();

    Q_SIGNALS:
        void textChanged();
        void delayChanged();
        void timeoutChanged();
        void visibleChanged();
        void shortcutChanged();

    private:
        QQuickToolTip *instance(bool create) const;

        int m_delay = 0;
        int m_timeout = -1;
        QString m_text;
        QString m_shortcut;

    };
}


#endif //SVSCRAFT_ACTIONTOOLTIPHELPER_P_H
