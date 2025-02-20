import QtQuick
import QtQuick.Templates as T

import SVSCraft.UIComponents

T.ToolBar {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding)

    padding: 4

    background: Rectangle {
        implicitHeight: 32
        color: Theme.backgroundColor(control.ThemedItem.backgroundLevel)
        Rectangle {
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
            color: Theme.dividerStrokeColor(control.ThemedItem.dividerStroke)
        }
    }
}