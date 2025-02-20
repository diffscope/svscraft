import QtQuick
import QtQuick.Templates as T

import SVSCraft.UIComponents

T.ScrollIndicator {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    padding: 2

    contentItem: Rectangle {
        implicitWidth: 2
        implicitHeight: 2

        radius: width / 2
        color: Theme.scrollBarColor
        opacity: (control.active && control.size < 1.0) ? 1 : 0

        Behavior on opacity {
            NumberAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
    }
}
