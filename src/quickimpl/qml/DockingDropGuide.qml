// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick

import SVSCraft.UIComponents

DockingDropIndicatorTemplate {
    id: control
    required property var region
    property bool highlighted: false
    readonly property rect area: region?.rect ?? Qt.rect(0, 0, 0, 0)
    readonly property color fillColor: highlighted ? Theme.accentColor : Theme.foregroundPrimaryColor
    polygon: region?.polygon ?? []
    inset: 2
    color: Qt.rgba(fillColor.r, fillColor.g, fillColor.b, highlighted ? 0.25 : 0.16)
    Accessible.ignored: true
}
