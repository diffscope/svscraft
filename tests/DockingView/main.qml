import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SVSCraft.UIComponents

ApplicationWindow {
    visible: true
    width: 1024
    height: 800

    DockingView {
        height: parent.height
        DockingPane {
            title: "Test 1"
            iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon.svg"
        }
        DockingPane {
            title: "Test 2"
            iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon.svg"
        }
        DockingStretch {
        }
        DockingPane {
            title: "Test 3"
            iconSource: "qrc:/qt/qml/SVSCraft/Test/DockingView/icon.svg"
        }
    }

}