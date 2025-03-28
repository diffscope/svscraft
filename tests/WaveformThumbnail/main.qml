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
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import SVSCraft
import SVSCraft.UIComponents

ApplicationWindow {
    id: main
    width: 800
    height: 400
    visible: true

    required property QtObject helper

    FileDialog {
        id: fileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: main.helper.load(selectedFile, levelComboBox.currentIndex, sampleTypeComboBox.currentIndex, useRmsCheckBox.checked)
    }

    Pane {
        anchors.fill: parent
        padding: 8
        ColumnLayout {
            anchors.fill: parent
            Button {
                Layout.fillWidth: true
                text: "Load 32-bit IEEE 754 Float Mono Channel Raw Data"
                onClicked: fileDialog.open()
            }
            ComboBox {
                id: levelComboBox
                Layout.fillWidth: true
                model: ["Downscale", "Original", "Upscale"]
            }
            ComboBox {
                id: sampleTypeComboBox
                Layout.fillWidth: true
                model: ["Int8", "Int16"]
            }
            CheckBox {
                id: useRmsCheckBox
                text: "RMS"
            }
            RangeSlider {
                id: rangeSlider
                Layout.fillWidth: true
                from: 0
                to: main.helper.size
                Connections {
                    target: main.helper
                    function onWaveformMipmapChanged() {
                        rangeSlider.first.value = 0
                        rangeSlider.second.value = main.helper.size
                    }
                }
            }
            WaveformThumbnail {
                Layout.fillWidth: true
                Layout.fillHeight: true
                waveformMipmap: main.helper.waveformMipmap
                waveformOffset: rangeSlider.first.value
                waveformLength: rangeSlider.second.value - rangeSlider.first.value
                color: Theme.accentColor
                rmsColor: Qt.rgba(1, 1, 1, 0.5)
            }
        }
    }
}