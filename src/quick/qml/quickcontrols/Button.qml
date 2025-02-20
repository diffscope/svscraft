import QtQml
import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl // NOTE: Qt Quick private API

import SVSCraft.UIComponents

T.Button {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    padding: 6
    horizontalPadding: control.text ? 8 : 6
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
        implicitWidth: control.text ? 64 : 28
        implicitHeight: 28
        property color _checkedColor: control.flat ? control.ThemedItem.controlType === Theme.CT_Normal ? Theme.controlCheckedColorChange.apply(Theme.buttonColor) : Theme.controlColor(control.ThemedItem.controlType) : control.ThemedItem.controlType === Theme.CT_Normal ? Theme.accentColor : Theme.controlColor(control.ThemedItem.controlType)
        property color _baseColor: control.checkable ? control.checked ? _checkedColor : Theme.buttonColor : Theme.controlColor(control.ThemedItem.controlType)
        property color _unactiveColor: control.checkable ? control.checked ? _checkedColor : control.flat ? "transparent" : Theme.buttonColor : control.flat ? "transparent" : Theme.controlColor(control.ThemedItem.controlType)
        property color _statusColor: control.down ? Theme.controlPressedColorChange.apply(_baseColor) :
                                     control.hovered? Theme.controlHoveredColorChange.apply(_baseColor) :
                                     _unactiveColor
        color: !control.enabled ? _unactiveColor.a ? Theme.controlDisabledColorChange.apply(_unactiveColor) : "transparent" :
               _statusColor
        border.color: control.visualFocus ? Theme.navigationColor : Theme.borderColor
        border.width: control.visualFocus ? 2 : control.flat || !control.checkable && control.ThemedItem.controlType !== Theme.CT_Normal || control.checkable && control.checked ? 0 : 1
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