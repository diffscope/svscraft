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

#ifndef SVSCRAFT_COMMANDPALETTEHELPER_P_H
#define SVSCRAFT_COMMANDPALETTEHELPER_P_H

#include <QObject>
#include <qqmlintegration.h>

class QAbstractItemModel;

namespace SVS {

    class CommandPaletteHelper : public QObject {
        Q_OBJECT
        QML_ELEMENT
        QML_SINGLETON
    public:
        explicit CommandPaletteHelper(QObject *parent = nullptr);
        ~CommandPaletteHelper() override;

        Q_INVOKABLE static QAbstractItemModel *createFilterModel(QAbstractItemModel *model, QObject *popup);
        Q_INVOKABLE static QString highlightString(const QString &s, const QString &t, const QColor &color);
    };
}

#endif // SVSCRAFT_COMMANDPALETTEHELPER_P_H
