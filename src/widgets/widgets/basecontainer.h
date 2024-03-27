#ifndef BASECONTAINER_H
#define BASECONTAINER_H

#include <QFrame>
#include <QWidget>

#include <QMWidgets/cbasicframe.h>
#include <SVSCraftWidgets/svscraftwidgetsglobal.h>

namespace SVS {

    class SVSCRAFT_WIDGETS_EXPORT BaseContainer : public CBasicFrame {
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
