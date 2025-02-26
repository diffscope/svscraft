import QtQml
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.impl

import SVSCraft.UIComponents

Frame {
    id: annotation
    property QtObject icon: QtObject {
        property string name: ""
        property url source: annotation.ThemedItem.controlType === SVS.CT_Normal ?
                "qrc:/qt/qml/SVSCraft/UIComponents/assets/Info16Regular.svg" :
            annotation.ThemedItem.controlType === SVS.CT_Accent ?
                "qrc:/qt/qml/SVSCraft/UIComponents/assets/CheckmarkCircle16Regular.svg" :
            annotation.ThemedItem.controlType === SVS.CT_Warning ?
                "qrc:/qt/qml/SVSCraft/UIComponents/assets/ErrorCircle16Regular.svg" :
            annotation.ThemedItem.controlType === SVS.CT_Error ?
                "qrc:/qt/qml/SVSCraft/UIComponents/assets/DismissCircle16Regular.svg" : ""
        property int width: 16
        property int height: 16
        property color color: rowLayout._color
        property bool cache: true
    }
    property string text: ""
    property bool closable: false
    property Action action: null

    spacing: 4
    padding: 4
    implicitWidth: 200
    implicitHeight: Math.max(32, label.height + annotation.topPadding + annotation.bottomPadding)

    background: Rectangle {
        color: "transparent"
        border.color: rowLayout._color
        radius: 2
    }
    RowLayout {
        id: rowLayout
        property color _color: annotation.ThemedItem.controlType === SVS.CT_Normal ? Theme.foregroundSecondaryColor : Theme.controlColor(annotation.ThemedItem.controlType)
        spacing: annotation.spacing
        anchors.fill: parent
        IconLabel {
            width: 16
            height: 16
            icon.name: annotation.icon.name
            icon.source: annotation.icon.source
            icon.color: annotation.icon.color
            icon.width: annotation.icon.width
            icon.height: annotation.icon.height
            icon.cache: annotation.icon.cache
            Layout.alignment: Qt.AlignVCenter
        }
        Label {
            id: label
            text: annotation.text
            color: rowLayout._color
            wrapMode: Text.Wrap
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
        }
        ToolButton {
            visible: annotation.action !== null
            action: annotation.action
            display: AbstractButton.IconOnly
            ThemedItem.foregroundLevel: SVS.FL_Secondary
            DescriptiveText.toolTip: annotation.action?.text ?? ""
            DescriptiveText.activated: hovered
            Layout.alignment: Qt.AlignVCenter
        }
        ToolButton {
            visible: annotation.closable
            icon.source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/Dismiss12Filled.svg"
            icon.width: 12
            icon.height: 12
            ThemedItem.foregroundLevel: SVS.FL_Secondary
            DescriptiveText.toolTip: qsTr("Close")
            DescriptiveText.activated: hovered
            Layout.alignment: Qt.AlignVCenter
            onClicked: annotation.visible = false
        }
    }

}