import QtQml
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

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
    ThemedItem.foregroundLevel: Theme.FL_Secondary
    onLabelChanged: () => {
        if (typeof(group.label) !== "string")
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
        if (typeof(group.label) === "string")
            rowItem.Accessible.name = group.label
    }
    onColumnItemChanged: () => {
        if (!columnItem)
            return
        columnItem.Layout.fillWidth = true
        if (typeof(group.label) === "string")
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
                text: typeof(group.label) === "string" ? group.label : ""
                visible: typeof(group.label) === "string"
                Layout.alignment: Qt.AlignHCenter
                ThemedItem.foregroundLevel: group.ThemedItem.foregroundLevel
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