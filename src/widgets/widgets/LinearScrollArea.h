#ifndef SVSCRAFT_LINEARSCROLLAREA_H
#define SVSCRAFT_LINEARSCROLLAREA_H

#include <QScrollArea>

#include <SVSCraftWidgets/SVSCraftWidgetsGlobal.h>

namespace SVS {

    class SVSCRAFT_WIDGETS_EXPORT LinearScrollArea : public QScrollArea {
        Q_OBJECT
    public:
        explicit LinearScrollArea(Qt::Orientation ori, QWidget *parent = nullptr);
        ~LinearScrollArea();

    public:
        Qt::Orientation orient() const;

    protected:
        Qt::Orientation m_orient;
        
        void resizeEvent(QResizeEvent *event) override;
    };

}

#endif // SVSCRAFT_LINEARSCROLLAREA_H
