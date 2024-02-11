#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <SVSCraftWidgets/longtimespinbox.h>
#include <QDebug>
#include <SVSCraftCore/musictimeline.h>
#include <SVSCraftWidgets/musictimespinbox.h>

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

    tempoLabel->setText(timeline.timeSignature(0).toString());

    QObject::connect(modifyTimeSigBtn, &QPushButton::clicked, [&] {
        if (timeline.timeSignature(0) == MusicTimeSignature(4, 4)) {
            timeline.addTimeSignature(0, {2, 4});
        } else {
            timeline.addTimeSignature(0, {4, 4});
        }
        tempoLabel->setText(timeline.timeSignature(0).toString());
    });

    dlg.show();

    QObject::connect(spinBox, &LongTimeSpinBox::valueChanged, [&](LongTime value) {
        QSignalBlocker blocker(mSpinBox);
        mSpinBox->setValue(timeline.create(value.totalMsec()));
    });

    QObject::connect(mSpinBox, &MusicTimeSpinBox::valueChanged, [=](const PersistentMusicTime& value) {
        QSpinBox blocker(spinBox);
        spinBox->setValue(LongTime(value.msec()));
    });

    QObject::connect(&timeline, &MusicTimeline::tempoChanged, [=] {
        QSpinBox blocker(spinBox);
        spinBox->setValue(LongTime(mSpinBox->value().msec()));
    });

    a.exec();
}
