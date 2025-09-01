/******************************************************************************
 * Copyright (c) 2025 OpenVPI                                                 *
 *                                                                            *
 * This file is part of SVSCraft                                              *
 *                                                                            *
 * SVSCraft is free software: you can redistribute it and/or modify it under  *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License, or (at your     *
 * option) any later version.                                                 *
 *                                                                            *
 * SVSCraft is distributed in the hope that it will be useful, but WITHOUT    *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public        *
 * License for more details.                                                  *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with SVSCraft. If not, see <https://www.gnu.org/licenses/>.          *
 ******************************************************************************/

import QtQml
import QtQml.Models
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Popup {
    id: popup

    property var model: null
    property string placeholderText: ""
    property string emptyText: ""
    property string filterText: ""
    property int currentIndex: 0

    CommandPaletteProxyModel {
        id: proxyModel
        sourceModel: popup.model
    }

    onCurrentIndexChanged: () => {
        if (!popup.model || popup.currentIndex < 0) {
            listView.currentIndex = 0
            return
        }
        let sourceIndex = popup.model.index(popup.currentIndex, 0)
        let proxyIndex = proxyModel.mapFromSource(sourceIndex)
        listView.currentIndex = proxyIndex.row
    }

    Binding {
        popup.filterText: textField.text
        popup.currentIndex: {
            if (!popup.model || !listView.currentItem || listView.currentIndex < 0) return -1
            let proxyIndex = proxyModel.index(listView.currentIndex, 0)
            let sourceIndex = proxyModel.mapToSource(proxyIndex)
            return sourceIndex.row
        }
        listView.currentIndex: {
            if (!popup.model || popup.currentIndex < 0) return 0
            let sourceIndex = popup.model.index(popup.currentIndex, 0)
            let proxyIndex = proxyModel.mapFromSource(sourceIndex)
            return proxyIndex.row
        }
    }
    onFilterTextChanged: () => {
        proxyModel.filterText = filterText
        listView.contentY = 0
        listView.DescriptiveText.statusTip = Qt.binding(() => {
            let index = proxyModel.index(listView._descriptionIndex, 0)
            return index.valid ? proxyModel.data(index, SVS.CP_DescriptionRole) || " " : " "
        })
    }

    x: parent ? (parent.width - implicitWidth) / 2 : 0
    implicitWidth: 600
    implicitHeight: Math.min(400, textField.height + 4 + listView.contentHeight + topPadding + bottomPadding)

    onAboutToShow: textField.forceActiveFocus()

    signal accepted()
    signal rejected()
    signal finished(int index)

    property bool isAccepting: false
    
    function accept() {
        isAccepting = true
        close();
    }

    onAboutToHide: () => {
        if (isAccepting) {
            accepted()
            finished(currentIndex)
            isAccepting = false
        } else {
            rejected()
            finished(-1)
        }
    }

    component CommandPaletteItemDelegate: ItemDelegate {
        id: itemDelegate
        required property var model
        required property int index
        width: listView.width
        ThemedItem.flat: true
        highlighted: listView.currentIndex === index
        padding: 4
        leftPadding: 6
        rightPadding: 6
        onHoveredChanged: () => {
            if (hovered)
                listView._hoveredIndex = index
            else if (listView._hoveredIndex === index)
                listView._hoveredIndex = -1
        }
        onPressedChanged: () => {
            if (pressed)
                listView.currentIndex = index
        }
        onClicked: () => {
            popup.accept()
        }
        property color _titleColor: itemDelegate.down ? Theme.foregroundPressedColorChange.apply(Theme.foregroundPrimaryColor) :
            itemDelegate.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundPrimaryColor) :
                Theme.foregroundPrimaryColor
        property color _subtitleColor: itemDelegate.down ? Theme.foregroundPressedColorChange.apply(Theme.foregroundSecondaryColor) :
            itemDelegate.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundSecondaryColor) :
                Theme.foregroundSecondaryColor
        property color _tagColor: itemDelegate.highlighted ? Theme.foregroundPrimaryColor : Theme.accentColor
        Behavior on _titleColor {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
        Behavior on _subtitleColor {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
        Behavior on _tagColor {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }

        Accessible.name: model.title || ""
        Accessible.description: [model.subtitle || "", model.tag || "", model.description || ""].join("\n")

        contentItem: ColumnLayout {
            spacing: 4
            RowLayout {
                Layout.fillWidth: true
                spacing: 4
                Text {
                    Layout.fillWidth: true
                    font: popup.font
                    text: CommandPaletteHelper.highlightString(itemDelegate.model.title || "", popup.filterText, Theme.highlightColor)
                    color: itemDelegate._titleColor
                    textFormat: Qt.RichText
                }
                Repeater {
                    model: typeof(itemDelegate.model.keySequence) === 'string' ? [itemDelegate.model.keySequence] : itemDelegate.model.keySequence
                    Rectangle {
                        required property string modelData
                        width: shortcutText.width + 2 * 4
                        height: shortcutText.height + 2 * 2
                        color: Theme.buttonColor
                        border.color: Theme.borderColor
                        radius: 2
                        visible: shortcutText.text.length !== 0
                        Text {
                            id: shortcutText
                            anchors.centerIn: parent
                            font: popup.font
                            text: modelData
                            color: itemDelegate._titleColor
                        }
                    }
                }
                Text {
                    visible: text !== ""
                    font: popup.font
                    text: itemDelegate.model.tag ?? ""
                    color: itemDelegate._tagColor
                }
            }
            Text {
                font: popup.font
                visible: text !== ""
                text: CommandPaletteHelper.highlightString(itemDelegate.model.subtitle || "", popup.filterText, Theme.highlightColor)
                color: itemDelegate._subtitleColor
                textFormat: Qt.RichText
            }
        }
    }

    ColumnLayout {
        spacing: 4
        anchors.fill: parent
        TextField {
            id: textField
            Layout.fillWidth: true
            placeholderText: popup.placeholderText
            text: popup.filterText
            Keys.onUpPressed: () => {
                let targetIndex = listView.currentIndex - 1
                while (targetIndex >= 0 && proxyModel.index(targetIndex, 0).data(SVS.CP_IsSeparatorRole)) {
                    targetIndex--
                }
                if (targetIndex >= 0)
                    listView.currentIndex = targetIndex
                if (listView.currentItem) {
                    Accessible.announce(qsTr("Current action: ") + listView.currentItem.Accessible.name + "\n" + listView.currentItem.Accessible.description, Accessible.Polite)
                }

            }
            Keys.onDownPressed: () => {
                let targetIndex = listView.currentIndex + 1
                while (targetIndex < listView.count && proxyModel.index(targetIndex, 0).data(SVS.CP_IsSeparatorRole)) {
                    targetIndex++
                }
                if (targetIndex < listView.count)
                    listView.currentIndex = targetIndex
                if (listView.currentItem) {
                    Accessible.announce(qsTr("Current action: ") + listView.currentItem.Accessible.name + "\n" + listView.currentItem.Accessible.description, Accessible.Polite)
                }
            }
            Keys.onReturnPressed: () => {
                if (popup.currentIndex === -1)
                    return
                popup.accept()
            }
        }
        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            property int _hoveredIndex: -1
            property int _descriptionIndex: _hoveredIndex !== -1 ? _hoveredIndex : currentIndex
            DescriptiveText.statusTip: {
                let index = proxyModel.index(_descriptionIndex, 0)
                return index.valid ? proxyModel.data(index, SVS.CP_DescriptionRole) || " " : " "
            }
            DescriptiveText.activated: popup.opened
            ScrollBar.vertical: ScrollBar {}
            clip: true
            model: proxyModel
            delegate: DelegateChooser {
                role: "isSeparator"
                DelegateChoice {
                    roleValue: true
                    Item {
                        Accessible.role: Accessible.Separator
                        width: listView.width
                        height: 5
                        Rectangle {
                            width: parent.width
                            height: 1
                            anchors.verticalCenter: parent.verticalCenter
                            color: Theme.borderColor
                        }
                    }
                }
                DelegateChoice {
                    roleValue: false
                    CommandPaletteItemDelegate {}
                }
                DelegateChoice {
                    roleValue: undefined
                    CommandPaletteItemDelegate {}
                }
            }
            Label {
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                visible: listView.count === 0
                ThemedItem.foregroundLevel: SVS.FL_Secondary
                text: popup.emptyText
            }
        }
    }
}