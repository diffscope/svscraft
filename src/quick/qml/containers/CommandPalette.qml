import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Popup {
    id: popup

    property var model: null
    property string titleRole: "title"
    property string subtitleRole: "subtitle"
    property string keywordRole: "keyword"
    property string descriptionRole: "description"
    property string keySequenceRole: "keySequence"
    property string recentlyUsedRole: "recentlyUsed"
    property string placeholderText: ""
    property string emptyText: ""
    property string recentlyUsedText: ""
    property string filterText: ""
    property int currentIndex: 0
    Binding {
        popup.filterText: textField.text
        popup.currentIndex: popup.model ? listView.model.mapToSource(listView.model.index(listView.currentIndex, 0)).row : 0
        listView.currentIndex: popup.model ? listView.model.mapFromSource(model.index(popup.currentIndex, 0)).row : 0
    }
    onFilterTextChanged: () => {
        listView.model.filterText = filterText
        listView.currentIndex = 0
        popup.currentIndex = Qt.binding(() => popup.model ? listView.model.mapToSource(listView.model.index(listView.currentIndex, 0)).row : 0)
        listView.contentY = 0
        listView.DescriptiveText.statusTip = Qt.binding(() => listView.model.descriptionAt(listView._descriptionIndex))
    }

    x: parent ? (parent.width - implicitWidth) / 2 : 0
    implicitWidth: 600
    implicitHeight: 400

    onAboutToShow: textField.forceActiveFocus()

    signal accepted()

    ColumnLayout {
        spacing: 4
        anchors.fill: parent
        TextField {
            id: textField
            Layout.fillWidth: true
            placeholderText: popup.placeholderText
            text: popup.filterText
            Keys.onUpPressed: listView.currentIndex = Math.max(0, listView.currentIndex - 1)
            Keys.onDownPressed: listView.currentIndex = Math.min(listView.count - 1, listView.currentIndex + 1)
            Keys.onReturnPressed: () => {
                if (popup.currentIndex === -1)
                    return
                popup.accepted()
                popup.close()
            }
        }
        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            property int _hoveredIndex: -1
            property int _descriptionIndex: _hoveredIndex !== -1 ? _hoveredIndex : currentIndex
            DescriptiveText.statusTip: model.descriptionAt(_descriptionIndex) || " "
            DescriptiveText.activated: popup.opened
            ScrollBar.vertical: ScrollBar {}
            clip: true
            model: CommandPaletteHelper.createFilterModel(popup.model, popup)
            delegate: ItemDelegate {
                id: itemDelegate
                required property var model
                required property int index
                width: ListView.view.width
                ThemedItem.flat: true
                highlighted: ListView.view.currentIndex === index
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
                    popup.accepted()
                    popup.close()
                }
                property color _titleColor: itemDelegate.down ? Theme.foregroundPressedColorChange.apply(Theme.foregroundPrimaryColor) :
                                            itemDelegate.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundPrimaryColor) :
                                            Theme.foregroundPrimaryColor
                property color _subtitleColor: itemDelegate.down ? Theme.foregroundPressedColorChange.apply(Theme.foregroundSecondaryColor) :
                                               itemDelegate.hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundSecondaryColor) :
                                               Theme.foregroundSecondaryColor
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

                contentItem: ColumnLayout {
                    spacing: 4
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 4
                        Text {
                            Layout.fillWidth: true
                            text: CommandPaletteHelper.highlightString(itemDelegate.model[popup.titleRole], popup.filterText, Theme.highlightColor)
                            color: itemDelegate._titleColor
                            textFormat: Qt.RichText
                        }
                        Rectangle {
                            width: shortcutText.width + 2 * 4
                            height: shortcutText.height + 2 * 2
                            color: Theme.buttonColor
                            border.color: Theme.borderColor
                            radius: 2
                            visible: shortcutText.text.length !== 0
                            Text {
                                id: shortcutText
                                anchors.centerIn: parent
                                text: itemDelegate.model[popup.keySequenceRole] ?? ""
                                color: itemDelegate._titleColor
                            }
                        }
                        Text {
                            visible: itemDelegate.model[popup.recentlyUsedRole] ?? false
                            text: popup.recentlyUsedText
                            color: itemDelegate._titleColor
                        }
                    }
                    Text {
                        text: CommandPaletteHelper.highlightString(itemDelegate.model[popup.subtitleRole], popup.filterText, Theme.highlightColor)
                        color: itemDelegate._subtitleColor
                        textFormat: Qt.RichText
                    }
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