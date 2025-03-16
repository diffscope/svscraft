import QtQuick
import QtQuick.Templates as T

import SVSCraft
import SVSCraft.UIComponents

T.GroupBox {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentWidth + leftPadding + rightPadding, implicitLabelWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding)

    spacing: 6
    padding: 6
    topPadding: padding + (implicitLabelWidth > 0 ? implicitLabelHeight + spacing : 0)

    label: Text {
        x: control.leftPadding
        width: control.availableWidth
        text: control.title
        font.family: control.font.family
        font.weight: Font.DemiBold
        font.pointSize: 1.25 * control.font.pointSize
        color: Theme.foregroundPrimaryColor
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter

        Button {
            id: foldButton
            property bool _folded: control.ThemedItem.folded
            property double _rotation: control.ThemedItem.folded ? 0 : 180
            visible: control.ThemedItem.foldable
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: control.rightPadding
            width: 20
            height: 20
            padding: 2
            horizontalPadding: 2
            icon.source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/ChevronDown12Filled.svg"
            icon.name: control.ThemedItem.folded ? qsTr("Expand") : qsTr("Collapse")
            Accessible.name: icon.name
            flat: true
            onClicked: () => {
                _folded = !_folded
            }
            Binding {
                control.ThemedItem.folded: foldButton._folded
                foldButton._folded: control.ThemedItem.folded
                foldButton.contentItem.rotation: foldButton._rotation
            }
            Behavior on _rotation {
                NumberAnimation {
                    duration: Theme.visualEffectAnimationDuration
                    easing.type: Easing.OutCubic
                }
            }
        }
    }

    background: Rectangle {
        y: control.topPadding - control.bottomPadding
        width: parent.width
        height: parent.height - control.topPadding + control.bottomPadding

        color: "transparent"
    }
}
