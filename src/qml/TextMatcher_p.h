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

#ifndef SVSCRAFT_TEXTMATCHER_P_H
#define SVSCRAFT_TEXTMATCHER_P_H

#include <QObject>
#include <qqmlintegration.h>

#include <QSet>

namespace SVS {

    class TextMatcherItem;

    class TextMatcher : public QObject {
        Q_OBJECT
        QML_ELEMENT
    public:
        explicit TextMatcher(QObject *parent = nullptr);
        ~TextMatcher() override;

        void addItem(TextMatcherItem *item);
        void removeItem(TextMatcherItem *item);

        Q_INVOKABLE bool matches(const QString &text, Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;

    private:
        QSet<TextMatcherItem *> m_items;
    };

}

#endif //SVSCRAFT_TEXTMATCHER_P_H
