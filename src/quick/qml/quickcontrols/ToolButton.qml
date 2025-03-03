import QtQml
import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl // NOTE: Qt Quick private API

import SVSCraft
import SVSCraft.UIComponents

T.ToolButton {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    padding: 4
    spacing: 4

    flat: true

    icon.width: 16
    icon.height: 16
    icon.color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundColor(control.ThemedItem.foregroundLevel)) :
                control.down ? Theme.foregroundPressedColorChange.apply(Theme.foregroundColor(control.ThemedItem.foregroundLevel)) :
                control.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundColor(control.ThemedItem.foregroundLevel)) :
                Theme.foregroundColor(control.ThemedItem.foregroundLevel)
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

        color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundColor(control.ThemedItem.foregroundLevel)) :
               control.down ? Theme.foregroundPressedColorChange.apply(Theme.foregroundColor(control.ThemedItem.foregroundLevel)) :
               control.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundColor(control.ThemedItem.foregroundLevel)) :
               Theme.foregroundColor(control.ThemedItem.foregroundLevel)

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
        property color _checkedColor: control.flat ? control.ThemedItem.controlType === SVS.CT_Normal ? Theme.controlCheckedColorChange.apply(Theme.buttonColor) : Theme.controlColor(control.ThemedItem.controlType) : control.ThemedItem.controlType === SVS.CT_Normal ? Theme.accentColor : Theme.controlColor(control.ThemedItem.controlType)
        property color _baseColor: control.checkable || control.highlighted ? control.checked || control.highlighted ? _checkedColor : Theme.buttonColor : Theme.controlColor(control.ThemedItem.controlType)
        property color _unactiveColor: control.checkable || control.highlighted ? control.checked || control.highlighted ? _checkedColor : control.flat ? "transparent" : Theme.buttonColor : control.flat ? "transparent" : Theme.controlColor(control.ThemedItem.controlType)
        property color _statusColor: control.down ? Theme.controlPressedColorChange.apply(_baseColor) :
                                     control.hovered? Theme.controlHoveredColorChange.apply(_baseColor) :
                                     _unactiveColor
        color: !control.enabled ? _unactiveColor.a ? Theme.controlDisabledColorChange.apply(_unactiveColor) : "transparent" :
               _statusColor
        border.color: control.visualFocus ? Theme.navigationColor : Theme.borderColor
        border.width: control.visualFocus ? 2 : control.flat || !control.checkable && control.ThemedItem.controlType !== SVS.CT_Normal || control.checkable && control.checked ? 0 : 1
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