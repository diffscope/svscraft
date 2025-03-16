import QtQuick
import QtQuick.Templates as T

import SVSCraft
import SVSCraft.UIComponents

T.TextField {
    id: control

    implicitWidth: implicitBackgroundWidth + leftInset + rightInset || Math.max(contentWidth, placeholder.implicitWidth) + leftPadding + rightPadding
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding, placeholder.implicitHeight + topPadding + bottomPadding)

    padding: 6
    leftPadding: 8
    rightPadding: 8

    color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
           Theme.foregroundPrimaryColor
    selectionColor: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.accentColor) :
                    Theme.accentColor
    selectedTextColor: color
    placeholderTextColor: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundSecondaryColor) :
                          Theme.foregroundSecondaryColor
    verticalAlignment: Text.AlignVCenter

    Behavior on color {
        ColorAnimation {
            duration: Theme.colorAnimationDuration
            easing.type: Easing.OutCubic
        }
    }
    Behavior on selectionColor {
        ColorAnimation {
            duration: Theme.colorAnimationDuration
            easing.type: Easing.OutCubic
        }
    }
    Behavior on placeholderTextColor {
        ColorAnimation {
            duration: Theme.colorAnimationDuration
            easing.type: Easing.OutCubic
        }
    }

    Text {
        id: placeholder
        x: control.leftPadding
        y: control.topPadding
        width: control.width - (control.leftPadding + control.rightPadding)
        height: control.height - (control.topPadding + control.bottomPadding)

        text: control.placeholderText
        font: control.font
        color: control.placeholderTextColor
        verticalAlignment: control.verticalAlignment
        visible: !control.length && !control.preeditText && (!control.activeFocus || control.horizontalAlignment !== Qt.AlignHCenter)
        elide: Text.ElideRight
        renderType: control.renderType
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 28
        border.width: control.activeFocus ? 2 : 1
        color: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.textFieldColor) :
               Theme.textFieldColor
        border.color: control.activeFocus ? Theme.accentColor : Theme.borderColor
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
