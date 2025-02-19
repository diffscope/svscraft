import QtQml
import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl // NOTE: Qt Quick private API

import SVSCraft.UIComponents

T.TabButton {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    padding: 6
    horizontalPadding: 8
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
        implicitWidth: 64
        implicitHeight: 20
        property color _baseColor: control.checked && control.ThemedItem.tabIndicator === Theme.TI_Fill ? Theme.accentColor : "transparent"
        color: !control.enabled && _baseColor.a ? Theme.controlDisabledColorChange.apply(_baseColor) :
               control.down && control.enabled ? Theme.controlPressedColorChange.apply(_baseColor) :
               control.hovered && control.enabled ? Theme.controlHoveredColorChange.apply(_baseColor) :
               _baseColor
        border.color: Theme.navigationColor
        border.width: control.visualFocus ? 2 : 0
        radius: 4

        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }

        Rectangle {
            visible: control.ThemedItem.tabIndicator !== Theme.TI_Fill
            anchors.top: control.ThemedItem.tabIndicator === Theme.TI_Top ? parent.top : undefined
            anchors.bottom: control.ThemedItem.tabIndicator === Theme.TI_Bottom ? parent.bottom : undefined
            anchors.left: control.ThemedItem.tabIndicator === Theme.TI_Left && !control.mirrored || control.ThemedItem.tabIndicator === Theme.TI_Right && control.mirrored ? parent.left : undefined
            anchors.right: control.ThemedItem.tabIndicator === Theme.TI_Left && control.mirrored || control.ThemedItem.tabIndicator === Theme.TI_Right && !control.mirrored ? parent.right : undefined
            anchors.verticalCenter: control.ThemedItem.tabIndicator === Theme.TI_Left || control.ThemedItem.tabIndicator === Theme.TI_Right ? parent.verticalCenter : undefined
            anchors.horizontalCenter: control.ThemedItem.tabIndicator === Theme.TI_Top || control.ThemedItem.tabIndicator === Theme.TI_Bottom ? parent.horizontalCenter : undefined
            width: control.ThemedItem.tabIndicator === Theme.TI_Top || control.ThemedItem.tabIndicator === Theme.TI_Bottom ? parent.width * 0.75 : 2
            height: control.ThemedItem.tabIndicator === Theme.TI_Left || control.ThemedItem.tabIndicator === Theme.TI_Right ? parent.height * 0.75 : 2
            radius: 1
            property color _baseColor: control.checked ? Theme.accentColor : "transparent"
            color: !control.enabled && _baseColor.a ? Theme.controlDisabledColorChange.apply(_baseColor) : _baseColor
            Behavior on color {
                ColorAnimation {
                    duration: Theme.colorAnimationDuration
                    easing.type: Easing.OutCubic
                }
            }
        }

    }
}