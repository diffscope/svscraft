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

#include "TextMatcherItem_p.h"

#include <SVSCraftQml/private/TextMatcher_p.h>

namespace SVS {

    TextMatcherItem::TextMatcherItem(QObject *parent) : QObject(parent), m_enabled(true) {
    }

    TextMatcherItem::~TextMatcherItem() = default;

    TextMatcher *TextMatcherItem::matcher() const {
        return m_matcher;
    }

    void TextMatcherItem::setMatcher(TextMatcher *matcher) {
        if (m_matcher == matcher)
            return;

        if (m_matcher) {
            m_matcher->removeItem(this);
        }

        m_matcher = matcher;

        if (m_matcher) {
            m_matcher->addItem(this);
        }

        emit matcherChanged();
    }

    void TextMatcherItem::setTarget(const QQmlProperty &property) {
        m_property = property;
    }

    QString TextMatcherItem::text() const {
        return m_property.read().toString();
    }

    bool TextMatcherItem::enabled() const {
        return m_enabled;
    }
    
    void TextMatcherItem::setEnabled(bool enabled) {
        if (m_enabled == enabled)
            return;
            
        m_enabled = enabled;
        emit enabledChanged();
    }

}

#include "moc_TextMatcherItem_p.cpp"
