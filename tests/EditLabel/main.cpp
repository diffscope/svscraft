#include <QApplication>
#include <QDialog>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <SVSCraftWidgets/editlabel.h>
#include <SVSCraftWidgets/expressiondoublespinbox.h>

using namespace SVS;

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    QDialog dlg;
    auto layout = new QFormLayout;
    auto editLabel = new EditLabel;
    auto spinBox = new ExpressionDoubleSpinBox;
    // qDebug() << spinBox->sizePolicy();
    // qDebug() << editLabel->sizePolicy();
    editLabel->setSuffix(" dB");
    editLabel->setEditWidget(spinBox);
    layout->addRow("&Gain", editLabel);
    auto editLabel2 = new EditLabel;
    auto lineEdit = new QLineEdit;
    editLabel2->setEditWidget(lineEdit);
    layout->addRow("&Text", editLabel2);
    auto button = new QPushButton("Toggle");
    layout->addRow(button);
    dlg.setLayout(layout);
    dlg.show();

    spinBox->setValue(3);
    lineEdit->setText("This is a test");

    a.exec();
}
