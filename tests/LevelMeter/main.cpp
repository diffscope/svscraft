#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QTimer>
#include <SVSCraftWidgets/levelmeter.h>

using namespace SVS;

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    QDialog dlg;
    auto layout = new QVBoxLayout;
    auto lm = new LevelMeter;
    layout->addWidget(lm);
    dlg.setLayout(layout);

    srand(QDateTime::currentSecsSinceEpoch());

    QTimer timer;
    timer.setInterval(100);
    QObject::connect(&timer, &QTimer::timeout, [=] {
        lm->setValue(1 - rand() % 128, 1 - rand() % 128);
    });
    timer.start();
    dlg.show();
    a.exec();
}

