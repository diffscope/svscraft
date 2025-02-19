import QtQuick
import QtQuick.Templates as T

import SVSCraft.UIComponents

T.Label {
    id: control

    color: Theme.foregroundColor(control.ThemedItem.foregroundLevel)
    linkColor: Theme.linkColor

    HoverHandler {
        enabled: parent.hoveredLink
        cursorShape: Qt.PointingHandCursor
    }
}