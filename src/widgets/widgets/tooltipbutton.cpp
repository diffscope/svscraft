#include "tooltipbutton.h"

#include <QMouseEvent>
#include <QStyle>
#include <QBasicTimer>
#include <QApplication>

// #include "tooltip.h"

namespace SVS {

    class ToolTipButtonPrivate {
    public:
        ToolTipButtonPrivate(ToolTipButton *q, Qt::Edge edge) : q(q), edge(edge) {
        }

        ~ToolTipButtonPrivate() {
        }

        ToolTipButton *q;
        Qt::Edge edge;

        void showToolTip() {
            // TODO
        }

        void hideToolTip() {
            // TODO
        }
    };

    ToolTipButton::ToolTipButton(Qt::Edge edge, QWidget *parent)
        : CToolButton(parent), d(new ToolTipButtonPrivate(this, edge)) {
    }

    ToolTipButton::ToolTipButton(QWidget *parent) : ToolTipButton(Qt::LeftEdge) {
    }

    ToolTipButton::~ToolTipButton() {
        delete d;
    }

    Qt::Edge ToolTipButton::toolTipEdge() const {
        return d->edge;
    }

    void ToolTipButton::setToolTipEdge(Qt::Edge edge) {
        d->edge = edge;
    }

    bool ToolTipButton::event(QEvent *event) {
        switch (event->type()) {
            case QEvent::ToolTip:
                return true;
            case QEvent::Enter: {
                if (auto text = this->toolTip(); !text.isEmpty()) {
                    d->showToolTip();
                }
                break;
            }
            case QEvent::Leave: {
                d->hideToolTip();
                break;
            }
            default:
                break;
        }
        return CToolButton::event(event);
    }

}