import QtQuick
import QtQuick.Templates as T

import SVSCraft
import SVSCraft.UIComponents

T.Slider {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitHandleWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitHandleHeight + topPadding + bottomPadding)

    padding: 6

    Rectangle {
        width: control.width
        height: control.height
        border.width: control.visualFocus ? 2 : 0
        color: "transparent"
        border.color: Theme.navigationColor
        radius: 4
    }

    handle: Rectangle {
        x: control.leftPadding + (control.horizontal ? control.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : control.visualPosition * (control.availableHeight - height))
        implicitWidth: 12
        implicitHeight: 12
        radius: width / 2
        width: implicitWidth * (!control.enabled ? 1.1 : control.pressed ? 1 : control.hovered ? 1.25 : 1.1)
        height: width
        color: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
               control.pressed ? Theme.controlPressedColorChange.apply(Theme.foregroundPrimaryColor) :
               control.hovered ? Theme.controlHoveredColorChange.apply(Theme.foregroundPrimaryColor) :
               Theme.foregroundPrimaryColor
        Behavior on width {
            NumberAnimation {
                duration: Theme.visualEffectAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
    }

    background: Rectangle {
        x: control.leftPadding + (control.horizontal ? 0 : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : 0)
        implicitWidth: control.horizontal ? 200 : 4
        implicitHeight: control.horizontal ? 4 : 200
        width: control.horizontal ? control.availableWidth : implicitWidth
        height: control.horizontal ? implicitHeight : control.availableHeight
        radius: 2
        color: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.buttonColor) :
               Theme.buttonColor
        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
        scale: control.horizontal && control.mirrored ? -1 : 1

        Rectangle {
            x: control.horizontal ? Math.min(control.ThemedItem.trackStart, control.position) * parent.width : 0
            y: control.horizontal ? 0 : Math.min(control.visualPosition, 1 - control.ThemedItem.trackStart) * parent.height
            width: control.horizontal ? Math.abs(control.position - control.ThemedItem.trackStart) * parent.width : 4
            height: control.horizontal ? 4 : Math.abs(control.position - control.ThemedItem.trackStart) * parent.height

            radius: 2
            color: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.accentColor) :
                   Theme.accentColor
            Behavior on color {
                ColorAnimation {
                    duration: Theme.colorAnimationDuration
                    easing.type: Easing.OutCubic
                }
            }
        }
    }
}
