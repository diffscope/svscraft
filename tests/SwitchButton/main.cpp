#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDateTime>
#include <SVSCraftWidgets/SwitchButton.h>

using namespace SVS;

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    QDialog dlg;
    auto layout = new QVBoxLayout;
    auto button = new SwitchButton;
    layout->addWidget(button);
    auto button2 = new QPushButton;
    layout->addWidget(button2);
    QObject::connect(button, &SwitchButton::clicked, button2, [=](bool checked) {
        button2->setText(QString::number(checked));
    });
    QObject::connect(button2, &QPushButton::clicked, button, &SwitchButton::click);
    dlg.setLayout(layout);
    dlg.show();
    a.exec();
}


