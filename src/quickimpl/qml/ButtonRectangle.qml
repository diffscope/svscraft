import QtQml
import QtQuick
import QtQuick.Templates as T

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Rectangle {
    required property Item control

    property bool flat: control.flat || control.ThemedItem.flat
    property bool checked: control.checked || control.checkState === Qt.PartiallyChecked
    property bool visualFocus: control.visualFocus
    property bool down: control.down
    property bool hovered: control.hovered
    function transparentIf(condition, color) {
        return condition ? Qt.rgba(color.r, color.g, color.b, 0) : color
    }
    property color _checkedColor: flat ? control.ThemedItem.controlType === SVS.CT_Normal ? Theme.controlCheckedColorChange.apply(Theme.buttonColor) : Theme.controlColor(control.ThemedItem.controlType) : control.ThemedItem.controlType === SVS.CT_Normal ? Theme.accentColor : Theme.controlColor(control.ThemedItem.controlType)
    property color _baseColor: control.checkable || control.highlighted ? checked || control.highlighted ? _checkedColor : Theme.buttonColor : Theme.controlColor(control.ThemedItem.controlType)
    property color _unactiveColor: control.checkable || control.highlighted ? checked || control.highlighted ? _checkedColor : transparentIf(flat, Theme.buttonColor) : transparentIf(flat, Theme.controlColor(control.ThemedItem.controlType))
    property color _statusColor: down ? Theme.controlPressedColorChange.apply(_baseColor) :
                                 control.enabled && hovered ? Theme.controlHoveredColorChange.apply(_baseColor) :
                                 _unactiveColor
    color: !control.enabled ? transparentIf(flat, Theme.controlDisabledColorChange.apply(_unactiveColor)) : _statusColor
    border.color: visualFocus ? Theme.navigationColor :
            Theme.flatButtonHighContrastBorderColor.a === 0 || ((!control.enabled || !hovered) && !down && !checked && !control.highlighted) ? Theme.borderColor :
            Theme.flatButtonHighContrastBorderColor
    border.width: visualFocus ? 2 : flat && (((!control.enabled || !hovered) && !down && !checked && !control.highlighted) || Theme.flatButtonHighContrastBorderColor.a === 0) ? 0 : 1

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