import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

import SVSCraft.UIComponents

T.MenuItem {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)

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
        readonly property real arrowPadding: control.arrow ? control.arrow.width + control.spacing : 0
        readonly property real indicatorPadding: control.icon.source.toString() && !control.checkable ? 0 : 20
        leftPadding: !control.mirrored ? indicatorPadding : arrowPadding
        rightPadding: control.mirrored ? indicatorPadding : arrowPadding

        spacing: control.spacing
        mirrored: control.mirrored
        display: control.checkable ? IconLabel.TextOnly : control.display
        alignment: Qt.AlignLeft

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

    indicator: ColorImage {
        x: control.mirrored ? control.width - width - control.rightPadding - 2 : control.leftPadding + 2
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 12
        height: 12
        visible: control.checked
        source: control.checkable ? "qrc:/qt/qml/SVSCraft/UIComponents/assets/Checkmark12Filled.svg" : ""
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

    arrow: Item {
        x: control.mirrored ? control.leftPadding : control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: control.subMenu ? arrowItem.width : keySequenceItem.width
        height: control.subMenu ? arrowItem.height : keySequenceItem.height

        Text {
            id: keySequenceItem
            text: control.action?.shortcut ?? ""
            visible: !control.subMenu
            color: arrowItem.color
        }

        ColorImage {
            id: arrowItem
            visible: control.subMenu
            mirror: control.mirrored
            width: 12
            height: 12
            source: control.subMenu ? "qrc:/qt/qml/SVSCraft/UIComponents/assets/ChevronRight12Filled.svg" : ""
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

    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 24
        width: control.width
        height: control.height
        color: !control.enabled ? Theme.buttonColor :
               control.down ? Theme.controlPressedColorChange.apply(Theme.accentColor) :
               control.hovered ? Theme.controlHoveredColorChange.apply(Theme.accentColor) :
               Theme.buttonColor
        border.color: Theme.navigationColor
        border.width: control.visualFocus ? 2 : 0
        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
        radius: 2
    }
}
