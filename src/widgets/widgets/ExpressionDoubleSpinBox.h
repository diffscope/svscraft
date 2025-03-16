#ifndef SVSCRAFT_EXPRESSIONDOUBLESPINBOX_H
#define SVSCRAFT_EXPRESSIONDOUBLESPINBOX_H

#include <QDoubleSpinBox>

#include <SVSCraftWidgets/SVSCraftWidgetsGlobal.h>

namespace SVS {

    class SVSCRAFT_WIDGETS_EXPORT ExpressionDoubleSpinBox : public QDoubleSpinBox {
        Q_OBJECT
    public:
        explicit ExpressionDoubleSpinBox(QWidget *parent = nullptr);
        ~ExpressionDoubleSpinBox() override;

        QString cleanText() const = delete;

        QString prefix() const = delete;
        void setPrefix(const QString &) = delete;
        QString suffix() const = delete;
        void setSuffix(const QString &) = delete;

        QValidator::State validate(QString &input, int &pos) const override;
        void fixup(QString &str) const override;
    };

}

#endif // SVSCRAFT_EXPRESSIONDOUBLESPINBOX_H
