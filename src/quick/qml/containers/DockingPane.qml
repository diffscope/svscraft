import QtQml
import QtQuick
import QtQuick.Controls

Pane {
    id: pane
    property Item header: null
    property string title: ""
    property string description: ""
    property url iconSource: ""
    property bool locked: false
    property bool dock: true
    property Menu menu: null
}