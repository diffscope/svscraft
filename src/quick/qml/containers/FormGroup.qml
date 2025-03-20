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

import QtQml
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SVSCraft
import SVSCraft.UIComponents

Item {
    id: group
    property var label: ""
    property Item rowItem: null
    property Item columnItem: null
    property int horizontalSpacing: 0
    property int verticalSpacing: 0
    implicitHeight: column.height
    implicitWidth: column.row.width
    ThemedItem.foregroundLevel: SVS.FL_Secondary
    onLabelChanged: () => {
        if (group.label instanceof Item)
            return
        if (rowItem)
            rowItem.Accessible.name = group.label
        if (columnItem)
            columnItem.Accessible.name = group.label
    }
    onRowItemChanged: () => {
        if (!rowItem)
            return
        rowItem.Layout.alignment = Qt.AlignHCenter
        if (!(group.label instanceof Item))
            rowItem.Accessible.name = group.label
    }
    onColumnItemChanged: () => {
        if (!columnItem)
            return
        columnItem.Layout.fillWidth = true
        if (!(group.label instanceof Item))
            columnItem.Accessible.name = group.label
    }
    ColumnLayout {
        id: column
        spacing: group.verticalSpacing
        width: group.width
        readonly property Item row: RowLayout {
            spacing: group.horizontalSpacing
            visible: group.label || group.rowItem
            readonly property Label labelItem: Label {
                text: group.label instanceof Item ? "" : group.label
                visible: !(group.label instanceof Item)
                Layout.alignment: Qt.AlignHCenter
                ThemedItem.foregroundLevel: group.ThemedItem.foregroundLevel
                topPadding: 2
                bottomPadding: 2
            }
            readonly property Item realLabelItem: group.label instanceof Item ? group.label : labelItem
            readonly property Item stretch: Item { Layout.fillWidth: true }
            data: [
                realLabelItem,
                stretch,
                rowItem
            ]
        }
        data: [
            row,
            columnItem
        ]
    }
}