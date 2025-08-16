import QtQml
import QtQuick

Item {
    property color color: "white"
    Image {
        anchors.fill: parent
        fillMode: Image.Tile
        horizontalAlignment: Image.AlignLeft
        verticalAlignment: Image.AlignTop
        source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/transparent_tile.svg"
    }
    Rectangle {
        anchors.fill: parent
        color: parent.color
    }
}