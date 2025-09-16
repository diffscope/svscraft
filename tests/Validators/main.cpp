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

#include <QMainWindow>
#include <QFormLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QApplication>
#include <QMetaProperty>
#include <QComboBox>

#include <SVSCraftGui/MusicPitchValidator.h>
#include <SVSCraftGui/LongTimeValidator.h>

using namespace SVS;

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QMainWindow w;
    auto mainWidget = new QWidget;
    auto mainLayout = new QFormLayout;
    auto bottomSpinBox = new QSpinBox;
    bottomSpinBox->setRange(-12, 127);
    bottomSpinBox->setValue(0);
    mainLayout->addRow("Bottom", bottomSpinBox);
    auto topSpinBox = new QSpinBox;
    topSpinBox->setRange(-12, 127);
    topSpinBox->setValue(127);
    mainLayout->addRow("Top", topSpinBox);
    auto accidentalComboBox = new QComboBox;
    accidentalComboBox->addItems({"Flat", "Sharp"});
    mainLayout->addRow("Accidental", accidentalComboBox);
    auto musicPitchValidator = new MusicPitchValidator;
    musicPitchValidator->setRange(0, 127);
    auto musicPitchLineEdit = new QLineEdit;
    musicPitchLineEdit->setValidator(musicPitchValidator);
    mainLayout->addRow("Music Pitch", musicPitchLineEdit);
    auto longTimeLineEdit = new QLineEdit;
    auto longTimeValidator = new LongTimeValidator;
    longTimeLineEdit->setValidator(longTimeValidator);
    mainLayout->addRow("Long Time", longTimeLineEdit);
    QObject::connect(bottomSpinBox, &QSpinBox::valueChanged, musicPitchValidator, &QIntValidator::setBottom);
    QObject::connect(bottomSpinBox, &QSpinBox::valueChanged, longTimeValidator, &QIntValidator::setBottom);
    QObject::connect(topSpinBox, &QSpinBox::valueChanged, musicPitchValidator, &QIntValidator::setTop);
    QObject::connect(topSpinBox, &QSpinBox::valueChanged, longTimeValidator, &QIntValidator::setTop);
    QObject::connect(accidentalComboBox, &QComboBox::currentIndexChanged, musicPitchValidator, [=](int index) {
        musicPitchValidator->setAccidental(static_cast<MusicPitch::Accidental>(index));
    });
    mainWidget->setLayout(mainLayout);
    w.setCentralWidget(mainWidget);
    w.show();
    return a.exec();
}
