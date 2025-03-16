#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <SVSCraftWidgets/LongTimeSpinBox.h>
#include <QDebug>
#include <SVSCraftCore/MusicTimeline.h>
#include <SVSCraftWidgets/MusicTimeSpinBox.h>

using namespace SVS;

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    MusicTimeline timeline;

    QDialog dlg;
    auto layout = new QVBoxLayout;
    auto spinBox = new LongTimeSpinBox;
    auto mSpinBox = new MusicTimeSpinBox(&timeline);
    layout->addWidget(spinBox);
    layout->addWidget(mSpinBox);
    auto modifyTimeSigBtn = new QPushButton("Modify Time Sig");
    layout->addWidget(modifyTimeSigBtn);
    auto tempoLabel = new QLabel;
    layout->addWidget(tempoLabel);
    dlg.setLayout(layout);

    tempoLabel->setText(timeline.timeSignatureAt(0).toString());

    QObject::connect(modifyTimeSigBtn, &QPushButton::clicked, [&] {
        if (timeline.timeSignatureAt(0) == MusicTimeSignature(4, 4)) {
            timeline.setTimeSignature(0, {2, 4});
        } else {
            timeline.setTimeSignature(0, {4, 4});
        }
        tempoLabel->setText(timeline.timeSignatureAt(0).toString());
    });

    dlg.show();

    QObject::connect(spinBox, &LongTimeSpinBox::valueChanged, [&](LongTime value) {
        QSignalBlocker blocker(mSpinBox);
        mSpinBox->setValue(timeline.create(value.totalMillisecond()));
    });

    QObject::connect(mSpinBox, &MusicTimeSpinBox::valueChanged, [=](const PersistentMusicTime& value) {
        QSpinBox blocker(spinBox);
        spinBox->setValue(LongTime(value.millisecond()));
    });

    QObject::connect(&timeline, &MusicTimeline::tempiChanged, [=] {
        QSpinBox blocker(spinBox);
        spinBox->setValue(LongTime(mSpinBox->value().millisecond()));
    });

    a.exec();
}
