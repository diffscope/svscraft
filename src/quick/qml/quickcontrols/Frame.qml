import QtQuick
import QtQuick.Templates as T

import SVSCraft.UIComponents

T.Pane {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding)

    background: Rectangle {
        color: Theme.backgroundColor(control.ThemedItem.backgroundLevel)
        border.width: 1
        border.color: Theme.borderColor
    }
}