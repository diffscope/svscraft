#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <SVSCraftWidgets/seekbar.h>
#include <QAccessibleWidget>

using namespace SVS;

class MySeekBar : public SeekBar {
protected:
    double displayValueFromActualValue(double value) const override {
        return std::log(value);
    }
};

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    QDialog dlg;
    auto layout = new QVBoxLayout;
    auto slider = new MySeekBar;
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

