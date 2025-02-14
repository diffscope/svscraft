import QtQml
import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl

import SVSCraft.UIComponents

T.Button {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    padding: 6
    horizontalPadding: 8
    spacing: 4
    icon.width: 12
    icon.height: 12
    icon.color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
                control.pressed ? Theme.foregroundPressedColorChange.apply(Theme.foregroundPrimaryColor) :
                control.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundPrimaryColor) :
                Theme.foregroundPrimaryColor
    Behavior on icon.color {
        ColorAnimation {
            duration: Theme.colorAnimationDuration
            easing.type: Easing.OutCubic
        }
    }

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font

        color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
               control.pressed ? Theme.foregroundPressedColorChange.apply(Theme.foregroundPrimaryColor) :
               control.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundPrimaryColor) :
               Theme.foregroundPrimaryColor

        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
    }

    background: Rectangle {
        implicitWidth: 64
        implicitHeight: 20
        color: !control.enabled && !control.flat ? Theme.controlDisabledColorChange.apply(Theme.controlColor(control.ThemedItem.controlType)) :
               control.pressed ? Theme.controlPressedColorChange.apply(Theme.controlColor(control.ThemedItem.controlType)) :
               control.hovered ? Theme.controlHoveredColorChange.apply(Theme.controlColor(control.ThemedItem.controlType)) :
               !control.flat ? Theme.controlColor(control.ThemedItem.controlType) : "transparent"
        border.color: control.visualFocus ? Theme.navigationColor : Theme.borderColor
        border.width: control.visualFocus ? 2 : control.flat || control.ThemedItem.controlType !== Theme.CT_Normal ? 0 : 1
        radius: 4

        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }

        Behavior on border.color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
    }
}