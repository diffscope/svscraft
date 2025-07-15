import QtQml
import QtQuick
import QtQuick.Effects
import Qt5Compat.GraphicalEffects
import QtQuick.Templates as T

import SVSCraft
import SVSCraft.UIComponents

T.ProgressBar {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    contentItem: Item {
        id: barArea
        implicitWidth: 200
        implicitHeight: 6
        scale: control.mirrored ? -1 : 1
        Item {
            id: bar
            anchors.fill: parent
            visible: false
            readonly property color color: control.ThemedItem.controlType === SVS.CT_Normal ? Theme.foregroundSecondaryColor : Theme.controlColor(control.ThemedItem.controlType)
            Rectangle {
                x: -height
                height: parent.height
                width: height + control.position * parent.width
                radius: height / 2
                color: bar.color
            }
            Rectangle {
                id: indeterminateIndicator
                x: -height - width
                height: parent.height
                width: height + parent.width / 4
                radius: height / 2
                color: bar.color
                opacity: control.indeterminate ? 1 : 0
                NumberAnimation on x {
                    from: -indeterminateIndicator.height - indeterminateIndicator.width
                    to: bar.width
                    loops: Animation.Infinite
                    Component.onCompleted: running = Qt.binding(() => control.indeterminate && control.visible)
                    duration: 2000
                }
            }

        }
        Rectangle {
            id: mask
            anchors.fill: parent
            layer.enabled: true
            layer.smooth: true
            radius: height / 2
            visible: false
        }
        OpacityMask { // TODO: MultiEffect has problems with antialiasing.
            source: bar
            anchors.fill: bar
            maskSource: mask
        }
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 6
        height: control.height
        radius: height / 2
        color: Theme.buttonColor
    }
}