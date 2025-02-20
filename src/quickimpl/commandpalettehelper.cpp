#include "commandpalettehelper_p.h"

#include <algorithm>

#include <QSortFilterProxyModel>
#include <QColor>

namespace SVS {

    class CommandPaletteFilterProxyModel : public QSortFilterProxyModel {
        Q_OBJECT
        Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged)
    public:
        QObject *m_popup = nullptr;
        QString m_filterText;
        QString filterText() const {
            return m_filterText;
        }
        void setFilterText(const QString& filterText) {
            if (m_filterText != filterText) {
                m_filterText = filterText;
                invalidateFilter();
                emit filterTextChanged();
            }
        }

        Q_INVOKABLE QString descriptionAt(int row) {
            int descriptionRole = -1;
            auto m = roleNames();
            std::for_each(m.keyValueBegin(), m.keyValueEnd(), [&](const auto &p) {
                if (p.second == m_popup->property("descriptionRole").toString())
                    descriptionRole = p.first;
            });
            return index(row, 0).data(descriptionRole).toString();
        }

        bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override {
            if (!m_popup)
                return true;
            int titleRole = -1;
            int subtitleRole = -1;
            int keywordRole = -1;
            auto m = roleNames();
            std::for_each(m.keyValueBegin(), m.keyValueEnd(), [&](const auto &p) {
                if (p.second == m_popup->property("titleRole").toString())
                    titleRole = p.first;
                else if (p.second == m_popup->property("subtitleRole").toString())
                    subtitleRole = p.first;
                else if (p.second == m_popup->property("keywordRole").toString())
                    keywordRole = p.first;
            });
            if (titleRole == -1 && subtitleRole == -1 && keywordRole == -1)
                return true;
            if (titleRole != -1 && sourceModel()->index(source_row, 0, source_parent).data(titleRole).toString().contains(m_filterText, Qt::CaseInsensitive))
                return true;
            if (subtitleRole != -1 && sourceModel()->index(source_row, 0, source_parent).data(subtitleRole).toString().contains(m_filterText, Qt::CaseInsensitive))
                return true;
            if (keywordRole != -1 && sourceModel()->index(source_row, 0, source_parent).data(keywordRole).toString().contains(m_filterText, Qt::CaseInsensitive))
                return true;
            return false;
        }

    signals:
        void filterTextChanged();
    };

    CommandPaletteHelper::CommandPaletteHelper(QObject *parent) : QObject(parent) {
    }
    CommandPaletteHelper::~CommandPaletteHelper() = default;
    QAbstractItemModel *CommandPaletteHelper::createFilterModel(QAbstractItemModel *model,
                                                                QObject *popup) {
        auto proxyModel = new CommandPaletteFilterProxyModel;
        proxyModel->m_popup = popup;
        proxyModel->setSourceModel(model);
        return proxyModel;
    }
    QString CommandPaletteHelper::highlightString(const QString &s, const QString &t, const QColor &c) {
        if (t.isEmpty()) {
            return s.toHtmlEscaped();
        }
        QString result;
        qsizetype pos = 0;
        auto matchPos = s.indexOf(t, pos, Qt::CaseInsensitive);
        while (matchPos != -1) {
            result += s.mid(pos, matchPos - pos).toHtmlEscaped();
            result += QStringLiteral("<span style='background-color: rgba(%1, %2, %3, %4);'>").arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alphaF());
            result += s.mid(matchPos, t.length()).toHtmlEscaped();
            result += QStringLiteral("</span>");
            pos = matchPos + t.length();
            matchPos = s.indexOf(t, pos);
        }
        result += s.mid(pos).toHtmlEscaped();
        return result;
    }
}

#include "commandpalettehelper.moc"
#include "moc_commandpalettehelper_p.cpp"