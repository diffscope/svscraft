#include "expressionspinbox.h"

#include <tinyexpr.h>

namespace SVS {

    ExpressionSpinBox::ExpressionSpinBox(QWidget *parent) : QSpinBox(parent) {
    }

    ExpressionSpinBox::~ExpressionSpinBox() = default;

    QValidator::State ExpressionSpinBox::validate(QString &input, int &pos) const {
        if (QString::number(input.toInt()) == input)
            return QValidator::Acceptable;
        else
            return QValidator::Intermediate;
    }

    void ExpressionSpinBox::fixup(QString &str) const {
        int err;
        auto s = str;
        for (auto &c : s) {
            if (c.unicode() >= 0xff01 && c.unicode() <= 0xff5e) {
                c.unicode() -= 0xfee0;
            }
        }
        double ret = te_interp(s.toLatin1(), &err);
        if (err == 0) {
            str = QString::number(int(ret));
        }
    }
}