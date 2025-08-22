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

#ifndef SVSCRAFT_COMMANDPALETTEPROXYMODEL_P_H
#define SVSCRAFT_COMMANDPALETTEPROXYMODEL_P_H

#include <QSortFilterProxyModel>
#include <qqmlintegration.h>

namespace SVS {

    class CommandPaletteProxyModel : public QSortFilterProxyModel {
        Q_OBJECT
        QML_NAMED_ELEMENT(CommandPaletteProxyModel)
        Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged)
        Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    public:
        explicit CommandPaletteProxyModel(QObject *parent = nullptr);
        ~CommandPaletteProxyModel() override;

        QString filterText() const;
        void setFilterText(const QString &filterText);

        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE inline int mapIndexToSource(int i) const {
            return mapToSource(index(i, 0)).row();
        }

        Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const {
            return QSortFilterProxyModel::data(index, role);
        }

    protected:
        bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    signals:
        void filterTextChanged();
        void countChanged();

    private:
        QString m_filterText;
    };

}

#endif // SVSCRAFT_COMMANDPALETTEPROXYMODEL_P_H
