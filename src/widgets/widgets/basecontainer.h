#ifndef BASECONTAINER_H
#define BASECONTAINER_H

#include <QFrame>
#include <QWidget>

#include <QMWidgets/QMWidgetsMacros.h>
#include <SVSWidgets/CkSVSWidgetsGlobal.h>

namespace SVS {

    class CKSVSWIDGETS_API BaseContainer : public QFrame {
        Q_OBJECT
        Q_PROPERTY_DECLARE(QMargins, margins, Margins)
        Q_PROPERTY_DECLARE(QColor, borderColor, BorderColor)
        Q_PROPERTY_DECLARE(QColor, wallpaper, Wallpaper)
    public:
        explicit BaseContainer(QWidget *parent = nullptr);
        ~BaseContainer();

    protected:
        void paintEvent(QPaintEvent *event) override;
    };

}

#endif // BASECONTAINER_H
