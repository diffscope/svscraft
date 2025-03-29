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
    height: 500
    visible: true

    required property QtObject helper

    FileDialog {
        id: fileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: main.helper.load(selectedFile, levelComboBox.currentIndex, sampleTypeComboBox.currentIndex, useRmsCheckBox.checked)
    }
    FileDialog {
        id: loadMipmapFileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: main.helper.loadMipmap(selectedFile)
    }
    FileDialog {
        id: saveMipmapFileDialog
        fileMode: FileDialog.SaveFile
        onAccepted: main.helper.saveMipmap(selectedFile)
    }

    Pane {
        anchors.fill: parent
        padding: 8
        ColumnLayout {
            anchors.fill: parent
            GroupBox {
                Layout.fillWidth: true
                title: "Audio Data"
                ColumnLayout {
                    RowLayout {
                        ComboBox {
                            id: levelComboBox
                            model: ["Downscale", "Original"]
                        }
                        ComboBox {
                            id: sampleTypeComboBox
                            model: ["Int8", "Int16"]
                        }
                        CheckBox {
                            id: useRmsCheckBox
                            text: "RMS"
                        }
                    }
                    Button {
                        text: "Load 32-bit IEEE 754 Float Mono Channel Raw Data"
                        onClicked: fileDialog.open()
                    }
                }
            }
            GroupBox {
                Layout.fillWidth: true
                title: "Mipmap"
                RowLayout {
                    Button {
                        text: "Load Mipmap"
                        onClicked: loadMipmapFileDialog.open()
                    }
                    Button {
                        text: "Save Mipmap"
                        onClicked: saveMipmapFileDialog.open()
                        enabled: main.helper.waveformMipmap.valid
                    }
                }
            }
            GroupBox {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: "Waveform View"
                ColumnLayout {
                    anchors.fill: parent
                    RowLayout {
                        Layout.fillWidth: true
                        SpinBox {
                            from: rangeSlider.from
                            to: rangeSlider.second.value
                            value: rangeSlider.first.value
                            onValueModified: rangeSlider.first.value = value
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
                        SpinBox {
                            from: rangeSlider.first.value
                            to: rangeSlider.to
                            value: rangeSlider.second.value
                            onValueModified: rangeSlider.second.value = value
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
    }
}