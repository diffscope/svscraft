#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <SVSCraftWidgets/seekbar.h>
#include <QAccessibleWidget>

#include <cmath>

using namespace SVS;

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    QDialog dlg;
    auto layout = new QVBoxLayout;
    auto slider = new SeekBar;
    slider->setDisplayValueConverter(QOverload<double>::of<double>(&std::log));
    layout->addWidget(slider);
    auto valueLabel = new QLabel;
    layout->addWidget(valueLabel);
    auto sliderPositionLabel = new QLabel;
    layout->addWidget(sliderPositionLabel);
    QObject::connect(slider, &SeekBar::valueChanged, valueLabel, [=](double value) {
        valueLabel->setText("value = " + QString::number(value));
        sliderPositionLabel->setText({});
    });
    QObject::connect(slider, &SeekBar::sliderMoved, sliderPositionLabel, [=](double value) {
        sliderPositionLabel->setText("sliderPosition = " + QString::number(value));
    });
    slider->setTracking(false);
    slider->setInterval(0);
    dlg.setLayout(layout);
    dlg.show();
    slider->setFocus();
    qDebug() << slider->focusPolicy() << slider->hasFocus();

    a.exec();
}

