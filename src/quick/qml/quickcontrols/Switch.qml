import QtQuick
import QtQuick.Templates as T
import QtQuick.Effects

import SVSCraft
import SVSCraft.UIComponents

T.Switch {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)

    padding: 6
    spacing: 6

    indicator: Rectangle {
        implicitWidth: 30
        implicitHeight: 16

        x: control.text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding + (control.availableWidth - width) / 2
        y: control.topPadding + (control.availableHeight - height) / 2

        property color _baseColor: control.checked ? Theme.accentColor : Theme.buttonColor
        color: !control.enabled ? Theme.controlDisabledColorChange.apply(_baseColor) :
               control.down ? Theme.controlPressedColorChange.apply(_baseColor) :
               control.hovered ? Theme.controlHoveredColorChange.apply(_baseColor) :
               _baseColor
        border.width: control.visualFocus ? 2 : 1
        border.color: control.visualFocus ? Theme.navigationColor : Theme.borderColor
        radius: height / 2

        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }

        Rectangle {
            width: height
            height: parent.height - 4
            anchors.verticalCenter: parent.verticalCenter
            radius: height / 2
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
            x: control.checked !== control.mirrored ? parent.width - width - 2 : 2
            Behavior on x {
                NumberAnimation {
                    duration: Theme.visualEffectAnimationDuration
                    easing.type: Easing.OutCubic
                }
            }
        }
    }

    contentItem: Text {
        leftPadding: control.indicator && !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.indicator && control.mirrored ? control.indicator.width + control.spacing : 0

        text: control.text
        font: control.font
        color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
            Theme.foregroundPrimaryColor

        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
    }
}
