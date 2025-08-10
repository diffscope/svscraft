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

#ifndef SVSCRAFT_TEXTMATCHERITEM_P_H
#define SVSCRAFT_TEXTMATCHERITEM_P_H

#include <QObject>
#include <QPointer>
#include <QQmlPropertyValueSource>
#include <QQmlProperty>
#include <qqmlintegration.h>

namespace SVS {

    class TextMatcher;

    class TextMatcherItem : public QObject, public QQmlPropertyValueSource {
        Q_OBJECT
        Q_INTERFACES(QQmlPropertyValueSource)
        QML_ELEMENT
        Q_PROPERTY(TextMatcher *matcher READ matcher WRITE setMatcher NOTIFY matcherChanged)
        Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    public:
        explicit TextMatcherItem(QObject *parent = nullptr);
        ~TextMatcherItem() override;

        TextMatcher *matcher() const;
        void setMatcher(TextMatcher *matcher);

        void setTarget(const QQmlProperty &) override;

        QString text() const;
        
        bool enabled() const;
        void setEnabled(bool enabled);

    signals:
        void matcherChanged();
        void enabledChanged();

    private:
        QPointer<TextMatcher> m_matcher;
        QQmlProperty m_property;
        bool m_enabled = true;

    };

}

#endif //SVSCRAFT_TEXTMATCHERITEM_P_H
