#ifndef SVSCRAFT_WORKFLOWDIALOG_H
#define SVSCRAFT_WORKFLOWDIALOG_H

#include <QDialog>

#include <SVSCraftWidgets/workflowpage.h>

namespace SVS {

    class WorkflowDialogPrivate;

    class SVSCRAFT_WIDGETS_EXPORT WorkflowDialog : public QDialog {
        Q_OBJECT
        Q_DECLARE_PRIVATE(WorkflowDialog)
    public:
        explicit WorkflowDialog(QWidget *parent = nullptr);
        ~WorkflowDialog();

    public:
        WorkflowPage *page() const;
        WorkflowPage *takePage();
        void setPage(WorkflowPage *w);

    protected:
        virtual bool prev();
        virtual bool next();
        virtual bool finish();

    Q_SIGNALS:
        void aboutToPrev();
        void aboutToNext();
        void finished();

        void helpRequested();

    protected:
        WorkflowDialog(WorkflowDialogPrivate &d, QWidget *parent = nullptr);

        QScopedPointer<WorkflowDialogPrivate> d_ptr;
    };

}

#endif // SVSCRAFT_WORKFLOWDIALOG_H
