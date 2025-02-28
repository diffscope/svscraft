import QtQml
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Effects

import SVSCraft
import SVSCraft.UIComponents

Popup {
    id: popup
    property QtObject icon: QtObject {
        property string name: ""
        property url source: popup.ThemedItem.controlType === SVS.CT_Normal ?
                "qrc:/qt/qml/SVSCraft/UIComponents/assets/Info16Regular.svg" :
            popup.ThemedItem.controlType === SVS.CT_Accent ?
                "qrc:/qt/qml/SVSCraft/UIComponents/assets/CheckmarkCircle16Regular.svg" :
            popup.ThemedItem.controlType === SVS.CT_Warning ?
                "qrc:/qt/qml/SVSCraft/UIComponents/assets/ErrorCircle16Regular.svg" :
            popup.ThemedItem.controlType === SVS.CT_Error ?
                "qrc:/qt/qml/SVSCraft/UIComponents/assets/DismissCircle16Regular.svg" : ""
        property int width: 16
        property int height: 16
        property color color: Theme.foregroundPrimaryColor
        property bool cache: true
    }
    property var title: ""
    property var content: ""
    property bool closable: false
    property int timeout: -1
    property Action action: null

    spacing: 4
    padding: 6
    implicitWidth: 240
    implicitHeight: Math.max(80, columnLayout.height + topPadding + bottomPadding)
    closePolicy: Popup.NoAutoClose
    exit: Transition {
        NumberAnimation {
            property: "opacity"
            from: 1.0
            to: 0.0
            duration: Theme.visualEffectAnimationDuration
        }
    }

    background: Rectangle {
        color: Theme.annotationPopupContentColorChange.apply(columnLayout._color)
        border.color: columnLayout._color
        radius: 4
        Item {
            id: roundedRectangleMask
            visible: false
            layer.enabled: true
            anchors.bottom: parent.bottom
            width: parent.width
            height: 8
            clip: true
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width * columnLayout._size
                height: 4

            }
        }
        Rectangle {
            id: progressBarSource
            visible: false
            color: columnLayout._color
            anchors.bottom: parent.bottom
            width: parent.width
            height: 8
            radius: 4
        }
        MultiEffect {
            anchors.bottom: parent.bottom
            width: parent.width
            height: 8
            visible: popup.timeout >= 0
            source: progressBarSource
            maskEnabled: true
            maskSource: roundedRectangleMask
        }
    }
    ColumnLayout {
        id: columnLayout
        spacing: 2 * popup.spacing
        width: parent.width
        property double _size: 1
        property color _color: popup.ThemedItem.controlType === SVS.CT_Normal ? Theme.foregroundSecondaryColor : Theme.controlColor(popup.ThemedItem.controlType)
        NumberAnimation on _size {
            id: progressBarAnimation
            from: 1
            to: 0
            duration: popup.timeout >= 0 ? popup.timeout : 0
            onFinished: popup.close()
            running: false
        }
        Behavior on _size {
            id: progressBarStopBehavior
            enabled: !progressBarAnimation.running
            NumberAnimation {
                duration: Theme.visualEffectAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
        Rectangle {
            Layout.fillWidth: true
            implicitHeight: Math.max(32, rowLayout.height + 2 * popup.spacing)
            color: Theme.annotationPopupTitleColorChange.apply(columnLayout._color)
            radius: 2
            RowLayout {
                id: rowLayout
                spacing: popup.spacing
                width: parent.width - 2 * spacing
                anchors.centerIn: parent
                IconLabel {
                    width: 16
                    height: 16
                    icon.name: popup.icon.name
                    icon.source: popup.icon.source
                    icon.color: popup.icon.color
                    icon.width: popup.icon.width
                    icon.height: popup.icon.height
                    icon.cache: popup.icon.cache
                    Layout.alignment: Qt.AlignVCenter
                }
                RowLayout {
                    id: labelContainer
                    Layout.alignment: Qt.AlignVCenter
                    Layout.fillWidth: true
                    property Label labelItem: Label {
                        id: label
                        text: typeof(popup.title) === "string" ? popup.title : ""
                        wrapMode: Text.Wrap
                    }
                    property Label realLabelItem: typeof(popup.title) === "string" ? labelItem : popup.title instanceof Label ? popup.title : null
                    data: [realLabelItem]
                    onRealLabelItemChanged: () => {
                        if (realLabelItem) {
                            realLabelItem.Layout.alignment = Qt.AlignVCenter
                            realLabelItem.Layout.fillWidth = true
                        }
                    }
                }
                ToolButton {
                    visible: popup.action !== null
                    action: popup.action
                    display: AbstractButton.IconOnly
                    ThemedItem.foregroundLevel: SVS.FL_Secondary
                    DescriptiveText.toolTip: popup.action?.text ?? ""
                    DescriptiveText.activated: hovered
                    Layout.alignment: Qt.AlignVCenter
                }
                ToolButton {
                    visible: popup.closable
                    icon.source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/Dismiss12Filled.svg"
                    icon.width: 12
                    icon.height: 12
                    ThemedItem.foregroundLevel: SVS.FL_Secondary
                    DescriptiveText.toolTip: qsTr("Close")
                    DescriptiveText.activated: hovered
                    Layout.alignment: Qt.AlignVCenter
                    onClicked: popup.visible = false
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            property Label labelItem: Label {
                text: typeof(popup.content) === "string" ? popup.content : ""
                wrapMode: Text.Wrap
            }
            property Label realLabelItem: typeof(popup.content) === "string" ? labelItem : popup.content instanceof Label ? popup.content : null
            onRealLabelItemChanged: () => {
                if (realLabelItem) {
                    realLabelItem.Layout.fillWidth = true
                    realLabelItem.Layout.leftMargin = Qt.binding(() => popup.spacing)
                    realLabelItem.Layout.rightMargin = Qt.binding(() => popup.spacing)
                }
            }
            data: [realLabelItem]
        }
    }

    HoverHandler {
        onHoveredChanged: () => {
            if (hovered) {
                progressBarAnimation.stop()
                columnLayout._size = 0
            }
        }
    }

    onOpened: () => {
        if (timeout >= 0) {
            progressBarAnimation.start()
        }
    }

    onClosed: () => {
        progressBarAnimation.stop()
    }

}