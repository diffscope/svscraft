import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SVSCraft.UIComponents

Item {
    id: view
    default property list<QtObject> contentData: []
    property int edge: Qt.LeftEdge
    Pane {
        id: tabBar
        implicitWidth: 40
        implicitHeight: 40
        width: view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge ? parent.width : undefined
        height: view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge ? parent.height: undefined
        GridLayout {
            rowSpacing: 6
            columnSpacing: 6
            anchors.fill: parent
            anchors.margins: 6
            columns: view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge ? 1 : -1
            rows: view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge ? 1 : -1
            Repeater {
                model: view.contentData
                ToolButton {
                    required property QtObject modelData
                    required property int index
                    enabled: modelData instanceof DockingPane
                    opacity: enabled ? 1 : 0
                    Layout.fillWidth: (view.edge === Qt.TopEdge || view.edge === Qt.BottomEdge) && !enabled
                    Layout.fillHeight: (view.edge === Qt.LeftEdge || view.edge === Qt.RightEdge) && !enabled
                    icon.source: enabled ? modelData.iconSource : ""
                    implicitWidth: 28
                    implicitHeight: 28
                    checkable: true
                    autoExclusive: true
                    DescriptiveText.activated: hovered && enabled
                    DescriptiveText.toolTip: modelData.title ?? ""
                    DescriptiveText.statusTip: modelData.description ?? ""
                }
            }
        }
    }
}