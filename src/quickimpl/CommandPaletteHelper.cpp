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

#include "CommandPaletteHelper_p.h"

#include <QColor>

namespace SVS {

    CommandPaletteHelper::CommandPaletteHelper(QObject *parent) : QObject(parent) {
    }
    
    CommandPaletteHelper::~CommandPaletteHelper() = default;
    
    QString CommandPaletteHelper::highlightString(const QString &s, const QString &t, const QColor &c) {
        if (t.isEmpty()) {
            return s.toHtmlEscaped();
        }
        QString result;
        qsizetype pos = 0;
        auto matchPos = s.indexOf(t, pos, Qt::CaseInsensitive);
        while (matchPos != -1) {
            result += s.mid(pos, matchPos - pos).toHtmlEscaped();
            result += QStringLiteral("<span style='background-color: rgba(%1, %2, %3, %4);'>").arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alphaF());
            result += s.mid(matchPos, t.length()).toHtmlEscaped();
            result += QStringLiteral("</span>");
            pos = matchPos + t.length();
            matchPos = s.indexOf(t, pos);
        }
        result += s.mid(pos).toHtmlEscaped();
        return result;
    }
}

#include "moc_CommandPaletteHelper_p.cpp"
