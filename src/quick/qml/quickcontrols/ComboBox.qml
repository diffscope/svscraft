pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl

import SVSCraft.UIComponents

T.ComboBox {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)

    leftPadding: padding + (!control.mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)
    rightPadding: padding + (control.mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)

    spacing: 4
    hoverEnabled: true

    delegate: ItemDelegate {
        required property var model
        required property int index

        width: ListView.view.width
        text: model[control.textRole]
        highlighted: control.currentIndex === index
        hoverEnabled: control.hoverEnabled
    }

    indicator: Item {
        x: control.mirrored ? control.padding : control.width - width - control.padding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: image.width + 8
        ColorImage {
            id: image
            anchors.centerIn: parent
            color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
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

    contentItem: TextField {
        leftPadding: !control.mirrored ? 8 : 0
        rightPadding: control.mirrored ? 8 : 0

        text: control.editable ? control.editText : control.displayText
        color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
               Theme.foregroundPrimaryColor
        selectionColor: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.accentColor) :
                        Theme.accentColor
        selectedTextColor: color
        placeholderTextColor: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundSecondaryColor) :
                              Theme.foregroundSecondaryColor

        enabled: control.editable
        autoScroll: control.editable
        readOnly: control.down
        inputMethodHints: control.inputMethodHints
        validator: control.validator
        selectByMouse: control.selectTextByMouse

        verticalAlignment: Text.AlignVCenter

        background: Rectangle {
            color: "transparent"
        }
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 28

        color: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.textFieldColor) :
               Theme.textFieldColor
        border.color: control.editable && control.contentItem.activeFocus ? Theme.accentColor : control.visualFocus ? Theme.navigationColor : Theme.borderColor
        border.width: control.visualFocus || control.editable && control.contentItem.activeFocus ? 2 : 1
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

    popup: T.Popup {
        y: control.height
        width: control.width
        height: Math.min(contentItem.implicitHeight, control.Window.height - topMargin - bottomMargin)
        topMargin: 6
        bottomMargin: 6
        popupType: T.Popup.Window

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.delegateModel
            currentIndex: control.currentIndex
            highlightMoveDuration: 0

            Rectangle {
                z: 10
                width: parent.width
                height: parent.height
                color: "transparent"
                border.color: Theme.borderColor
                radius: 4
            }

            T.ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            id: backgroundArea
            color: Theme.buttonColor
            radius: 4
        }
    }
}
