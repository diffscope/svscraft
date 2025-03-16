#ifndef SVSCRAFT_FULLICONBUTTON_H
#define SVSCRAFT_FULLICONBUTTON_H

#include <QMWidgets/cpushbutton.h>
#include <QMWidgets/qmwidgetsmacros.h>
#include <SVSCraftWidgets/svscraftwidgetsglobal.h>

namespace SVS {

    class SVSCRAFT_WIDGETS_EXPORT FullIconButton : public CPushButton {
        Q_OBJECT
        Q_PROPERTY_DECLARE(QSize, iconMargin, IconMargin)
    public:
        explicit FullIconButton(QWidget *parent = nullptr);
        ~FullIconButton();

    protected:
        void paintEvent(QPaintEvent *event) override;
    };

}

#endif // SVSCRAFT_FULLICONBUTTON_H
