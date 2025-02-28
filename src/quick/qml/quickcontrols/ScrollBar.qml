import QtQuick
import QtQuick.Templates as T

import SVSCraft
import SVSCraft.UIComponents

T.ScrollBar {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    padding: 2
    visible: control.policy !== T.ScrollBar.AlwaysOff
    minimumSize: orientation === Qt.Horizontal ? height / width : width / height

    contentItem: Rectangle {
        implicitWidth: control.interactive ? 6 : 2
        implicitHeight: control.interactive ? 6 : 2

        radius: width / 2
        color: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.scrollBarColor) :
               control.pressed ? Theme.controlPressedColorChange.apply(Theme.scrollBarColor) :
               control.hovered ? Theme.controlHoveredColorChange.apply(Theme.scrollBarColor) :
               Theme.scrollBarColor
        opacity: control.policy === T.ScrollBar.AlwaysOn || (control.active && control.size < 1.0) ? 1 : 0

        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }

        Behavior on opacity {
            NumberAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
    }
}
