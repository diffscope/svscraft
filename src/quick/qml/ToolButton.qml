import QtQml
import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl // NOTE: Qt Quick private API

import SVSCraft.UIComponents

T.ToolButton {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    padding: 4
    spacing: 4
    icon.width: 16
    icon.height: 16
    icon.color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
                control.down ? Theme.foregroundPressedColorChange.apply(Theme.foregroundPrimaryColor) :
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
               control.down ? Theme.foregroundPressedColorChange.apply(Theme.foregroundPrimaryColor) :
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
        implicitWidth: 24
        implicitHeight: 24
        property color _baseColor: control.checkable && control.checked ? Theme.accentColor : Theme.buttonColor
        color: !control.enabled ? control.checkable && control.checked ? Theme.controlDisabledColorChange.apply(Theme.accentColor) : "transparent" :
               control.down && control.enabled ? Theme.controlPressedColorChange.apply(_baseColor) :
               control.hovered && control.enabled ? Theme.controlHoveredColorChange.apply(_baseColor) :
               control.checkable && control.checked ? Theme.accentColor : "transparent"
        border.color: Theme.navigationColor
        border.width: control.visualFocus ? 2 : 0
        radius: 4

        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
    }
}