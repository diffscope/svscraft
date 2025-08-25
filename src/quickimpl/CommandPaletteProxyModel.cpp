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

#include "CommandPaletteProxyModel_p.h"

#include <SVSCraftCore/SVSCraftNamespace.h>

namespace SVS {

    CommandPaletteProxyModel::CommandPaletteProxyModel(QObject *parent) : QSortFilterProxyModel(parent) {
        connect(this, &QAbstractItemModel::modelReset, this, &CommandPaletteProxyModel::countChanged);
        connect(this, &QAbstractItemModel::rowsInserted, this, &CommandPaletteProxyModel::countChanged);
        connect(this, &QAbstractItemModel::rowsRemoved, this, &CommandPaletteProxyModel::countChanged);
    }

    CommandPaletteProxyModel::~CommandPaletteProxyModel() = default;

    QString CommandPaletteProxyModel::filterText() const {
        return m_filterText;
    }

    void CommandPaletteProxyModel::setFilterText(const QString &filterText) {
        if (m_filterText != filterText) {
            m_filterText = filterText;
            invalidateFilter();
            emit filterTextChanged();
        }
    }

    QHash<int, QByteArray> CommandPaletteProxyModel::roleNames() const {
        static const QHash<int, QByteArray> roles {
            {SVSCraft::CP_TitleRole, "title"},
            {SVSCraft::CP_SubtitleRole, "subtitle"},
            {SVSCraft::CP_KeywordRole, "keyword"},
            {SVSCraft::CP_DescriptionRole, "description"},
            {SVSCraft::CP_KeySequenceRole, "keySequence"},
            {SVSCraft::CP_TagRole, "tag"},
            {SVSCraft::CP_IsSeparatorRole, "isSeparator"},
        };
        return roles;
    }

    bool CommandPaletteProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const {
        if (m_filterText.isEmpty())
            return true;

        auto index = sourceModel()->index(source_row, 0, source_parent);

        // Check title
        if (index.data(SVSCraft::CP_TitleRole).toString().contains(m_filterText, Qt::CaseInsensitive))
            return true;

        // Check subtitle  
        if (index.data(SVSCraft::CP_SubtitleRole).toString().contains(m_filterText, Qt::CaseInsensitive))
            return true;

        // Check keyword
        if (index.data(SVSCraft::CP_KeywordRole).toString().contains(m_filterText, Qt::CaseInsensitive))
            return true;

        return false;
    }

}

#include "moc_CommandPaletteProxyModel_p.cpp"
