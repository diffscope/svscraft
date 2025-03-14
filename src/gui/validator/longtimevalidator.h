#ifndef LONGTIMEVALIDATOR_H
#define LONGTIMEVALIDATOR_H

#include <QIntValidator>

#include <SVSCraftGui/svscraftguiglobal.h>

namespace SVS {

    class SVSCRAFT_GUI_EXPORT LongTimeValidator : public QIntValidator {
        Q_OBJECT

    public:
        explicit LongTimeValidator(QObject *parent = nullptr);
        LongTimeValidator(int minimum, int maximum, QObject *parent = nullptr);

        State validate(QString &, int &) const override;
        void fixup(QString &) const override;

    };

}

#endif //LONGTIMEVALIDATOR_H
