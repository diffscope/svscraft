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
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SVSCraft
import SVSCraft.UIComponents

Frame {
    id: card

    property bool atTop: true
    property bool atBottom: true
    property var image: ""
    property var title: ""
    property var subtitle: ""
    property Item toolBar: null

    implicitHeight: 60
    implicitWidth: 300
    padding: 6
    spacing: 6

    background: Rectangle {
        color: Theme.backgroundColor(card.ThemedItem.backgroundLevel)
        border.width: 1
        border.color: Theme.borderColor
        topLeftRadius: card.atTop ? 2 : 0
        topRightRadius: topLeftRadius
        bottomLeftRadius: card.atBottom ? 2 : 0
        bottomRightRadius: bottomLeftRadius
        Rectangle {
            color: parent.color
            width: parent.width - 2
            x: 1
            height: 1
            visible: !card.atTop
        }
    }

    RowLayout {
        id: row
        spacing: card.spacing
        anchors.fill: parent
        property Image imageItem: Image {
            source: card.image instanceof Item ? "" : card.image
        }
        property Item realImageItem: card.image instanceof Item ? card.image : imageItem
        property Item imageContainer: Item {
            implicitHeight: row.height
            implicitWidth: implicitHeight
            children: [row.realImageItem]
        }
        onRealImageItemChanged: realImageItem.anchors.fill = imageContainer
        property Item toolBar: card.toolBar
        onToolBarChanged: () => {
            if (toolBar)
                toolBar.Layout.alignment = Qt.AlignVCenter
        }
        property Label titleItem: Label {
            text: card.title instanceof Item ? "" : card.title
            font.weight: Font.DemiBold
        }
        property Item realTitleItem: card.title instanceof Item ? card.title : titleItem
        property Label subtitleItem: Label {
            text: card.subtitle instanceof Item ? "" : card.subtitle
        }
        property Item realSubtitleItem: card.subtitle instanceof Item ? card.subtitle : subtitleItem
        property Item columnContainer: Item {
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            height: column.height
            ColumnLayout {
                id: column
                spacing: card.spacing
                width: parent.width
                children: [row.realTitleItem, row.realSubtitleItem]
            }
        }
        children: [imageContainer, columnContainer, toolBar]
    }

}