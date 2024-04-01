#ifndef TITLELISTITEMDELEGATEPRIVATE_H
#define TITLELISTITEMDELEGATEPRIVATE_H

#include <QMWidgets/qfontinfoex.h>
#include <QMWidgets/qpeninfo.h>
#include <QMWidgets/qrectinfo.h>
#include <SVSCraftWidgets/titlelistitemdelegate.h>

namespace SVS {

    class TitleListItemDelegatePrivate {
        Q_DECLARE_PUBLIC(TitleListItemDelegate)
    public:
        TitleListItemDelegatePrivate();
        virtual ~TitleListItemDelegatePrivate();

        void init();

        TitleListItemDelegate *q_ptr;

        QRectInfo m_backgroundType;
        QPenInfo m_underline;

        QFontInfoEx m_fileType;
        QFontInfoEx m_locType;
        QFontInfoEx m_dateType;
        QFontInfoEx m_dateHighlightType;
        QRectInfo m_dateBackType;

        QMargins m_fileMargins;
        QMargins m_locMargins;
        QMargins m_dateMargins;
        QMargins m_margins;
        QMargins m_padding;

        QMargins m_iconMargins;
        QSize m_defaultIconSize;

        QCssValueMap styleValues_helper() const;
        void setStyleValues_helper(const QCssValueMap &map);
    };

}

#endif // TITLELISTITEMDELEGATEPRIVATE_H