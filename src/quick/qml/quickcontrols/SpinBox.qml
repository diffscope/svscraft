import QtQml
import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl

T.SpinBox {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    rightPadding: padding + Math.max((down.indicator ? down.indicator.width : 0), (up.indicator ? up.indicator.width : 0))

    editable: true

    validator: IntValidator {
        locale: control.locale.name
        bottom: Math.min(control.from, control.to)
        top: Math.max(control.from, control.to)
    }

    contentItem: TextInput {
        text: control.displayText
        padding: 6
        leftPadding: 8
        rightPadding: 8
        font: control.font

        color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
               Theme.foregroundPrimaryColor
        selectionColor: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.accentColor) :
                        Theme.accentColor
        selectedTextColor: color
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

        verticalAlignment: Qt.AlignVCenter
        readOnly: !control.editable
        validator: control.validator
        inputMethodHints: control.inputMethodHints
    }

    up.indicator: Rectangle {
        x: control.mirrored ? 3 : control.width - width - 3
        y: 3
        height: control.height / 2 - 3
        implicitWidth: 24
        implicitHeight: 12
        radius: 1
        property color _statusColor: control.up.pressed ? Theme.controlPressedColorChange.apply(Theme.buttonColor) :
                                     control.up.hovered ? Theme.controlHoveredColorChange.apply(Theme.buttonColor) :
                                     "transparent"
        color: !enabled ? "transparent" : _statusColor
        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
        ColorImage {
            anchors.centerIn: parent
            color: !enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
                   Theme.foregroundPrimaryColor
            source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/ChevronUp12Filled.svg"
            Behavior on color {
                ColorAnimation {
                    duration: Theme.colorAnimationDuration
                    easing.type: Easing.OutCubic
                }
            }
        }
    }

    down.indicator: Rectangle {
        x: control.mirrored ? 3 : control.width - width - 3
        y: control.height / 2
        height: control.height / 2 - 3
        implicitWidth: 24
        implicitHeight: 12
        radius: 1
        property color _statusColor: control.down.pressed ? Theme.controlPressedColorChange.apply(Theme.buttonColor) :
                                     control.down.hovered ? Theme.controlHoveredColorChange.apply(Theme.buttonColor) :
                                     "transparent"
        color: !enabled ? "transparent" : _statusColor
        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
        ColorImage {
            anchors.centerIn: parent
            color: !enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
                   Theme.foregroundPrimaryColor
            source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/ChevronDown12Filled.svg"
            Behavior on color {
                ColorAnimation {
                    duration: Theme.colorAnimationDuration
                    easing.type: Easing.OutCubic
                }
            }
        }
    }

    background: Rectangle {
        implicitWidth: 100
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
