#include <QMainWindow>
#include <QFormLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QApplication>
#include <QMetaProperty>
#include <QComboBox>

#include <SVSCraftGui/musicpitchvalidator.h>
#include <SVSCraftGui/longtimevalidator.h>

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