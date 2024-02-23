#ifndef TITLELISTWIDGET_P_H
#define TITLELISTWIDGET_P_H

#include <SVSCraftWidgets/titlelistitemdelegate.h>
#include <SVSCraftWidgets/titlelistwidget.h>

namespace SVS {

    class TitleListWidgetPrivate : public QObject {
        Q_DECLARE_PUBLIC(TitleListWidget)
    public:
        TitleListWidgetPrivate();
        ~TitleListWidgetPrivate();

        void init();

        TitleListWidget *q_ptr;

        TitleListItemDelegate *m_delegate;
        QSize oldContentsSize;

    private:
        void _q_delegateClicked(const QModelIndex &index, int button);
    };

}

#endif // TITLELISTWIDGET_P_H
