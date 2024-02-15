#ifndef TOOLTIPBUTTON_H
#define TOOLTIPBUTTON_H

#include <QMWidgets/ctoolbutton.h>
#include <QMWidgets/qmwidgetsmacros.h>

#include <SVSCraftWidgets/svscraftwidgetsglobal.h>

namespace SVS {

    class ToolTipButtonPrivate;

    class SVSCRAFT_WIDGETS_EXPORT ToolTipButton : public CToolButton {
        Q_OBJECT
    public:
        explicit ToolTipButton(QWidget *parent = nullptr);
        explicit ToolTipButton(Qt::Edge edge, QWidget *parent = nullptr);
        ~ToolTipButton();

    public:
        Qt::Edge toolTipEdge() const;
        void setToolTipEdge(Qt::Edge edge);

    protected:
        bool event(QEvent *event) override;

    protected:
        ToolTipButtonPrivate *d;
    };

}

#endif // TOOLTIPBUTTON_H
