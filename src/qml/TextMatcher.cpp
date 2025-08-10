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

#include "TextMatcher_p.h"

#include <algorithm>
#include <ranges>

#include <QSet>
#include <QString>

#include <SVSCraftQml/private/TextMatcherItem_p.h>

namespace SVS {

    TextMatcher::TextMatcher(QObject *parent) : QObject(parent) {
    }

    TextMatcher::~TextMatcher() = default;

    void TextMatcher::addItem(TextMatcherItem *item) {
        if (!item || m_items.contains(item))
            return;
        m_items.insert(item);
        connect(item, &QObject::destroyed, this, [this, item]() {
            removeItem(item);
        });
    }

    void TextMatcher::removeItem(TextMatcherItem *item) {
        if (!item || !m_items.contains(item))
            return;
        m_items.remove(item);
        disconnect(item, &QObject::destroyed, this, nullptr);
    }

    bool TextMatcher::matches(const QString &text, Qt::CaseSensitivity caseSensitivity) const {
        return std::ranges::any_of(m_items, [&text, caseSensitivity](const TextMatcherItem *item) {
            return item->enabled() && item->text().contains(text, caseSensitivity);
        });
    }

}