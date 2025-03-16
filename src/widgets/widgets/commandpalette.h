#ifndef SVSCRAFT_COMMANDPALETTE_H
#define SVSCRAFT_COMMANDPALETTE_H

#include <QListWidget>
#include <QVBoxLayout>

#include <QMWidgets/qmwidgetsmacros.h>
#include <SVSCraftWidgets/basecontainer.h>

namespace SVS {

    class CommandPalettePrivate;

    class SVSCRAFT_WIDGETS_EXPORT CommandPalette : public BaseContainer {
        Q_OBJECT
        Q_DECLARE_PRIVATE(CommandPalette)
        Q_LAYOUT_PROPERTY_DELCARE
    public:
        explicit CommandPalette(QWidget *parent = nullptr);
        ~CommandPalette();

        inline void addItem(QListWidgetItem *item);
        void insertItem(int index, QListWidgetItem *item);
        int count() const;
        void clear();
        QListWidgetItem *itemAt(int index) const;

        QListWidgetItem *currentItem() const;
        void setCurrentItem(QListWidgetItem *item);

        int currentRow() const;
        void setCurrentRow(int row);

        QString filterHint() const;
        void setFilterHint(const QString &hint);

        QString filterKeyword() const;
        void setFilterKeyword(const QString &keyword);

    public Q_SLOTS:
        void start();
        void activate(int index);
        void abandon();

    Q_SIGNALS:
        void activated(int index);
        void finished(QListWidgetItem *item);
        void tabPressed();

        void currentRowChanged(int role);
        void currentItemChanged(QListWidgetItem *item);
        void filterKeywordChanged(const QString &text);

        void styleValuesChanged();

    protected:
        bool event(QEvent *event) override;

    protected:
        CommandPalette(CommandPalettePrivate &d, QWidget *parent = nullptr);

        QScopedPointer<CommandPalettePrivate> d_ptr;
    };

    inline void CommandPalette::addItem(QListWidgetItem *item) {
        insertItem(count(), item);
    }

}

#endif // SVSCRAFT_COMMANDPALETTE_H