#ifndef SVSCRAFT_TITLELISTWIDGET_H
#define SVSCRAFT_TITLELISTWIDGET_H

#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QListWidget>
#include <QMenu>

#include <QMWidgets/qcssvaluemap.h>
#include <SVSCraftWidgets/svscraftwidgetsglobal.h>

namespace SVS {

    class TitleListWidgetPrivate;

    class SVSCRAFT_WIDGETS_EXPORT TitleListWidget : public QListWidget {
        Q_OBJECT
        Q_DECLARE_PRIVATE(TitleListWidget)
        Q_PROPERTY(QCssValueMap styleValues READ styleValues WRITE setStyleValues)
    public:
        explicit TitleListWidget(QWidget *parent = nullptr);
        ~TitleListWidget();

    public:
        QCssValueMap styleValues() const;
        void setStyleValues(const QCssValueMap &map);

        QSize contentsSize() const;

    protected:
        QScopedPointer<TitleListWidgetPrivate> d_ptr;

        void updateGeometries() override;
        void mouseReleaseEvent(QMouseEvent *event) override;

    Q_SIGNALS:
        void itemClickedEx(const QModelIndex &index, int button);
        void contentsSizeUpdated(const QSize &size);
    };

}

#endif // SVSCRAFT_TITLELISTWIDGET_H
