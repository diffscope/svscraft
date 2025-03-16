#ifndef SVSCRAFT_CONFIGURABLEDIALOG_H
#define SVSCRAFT_CONFIGURABLEDIALOG_H

#include <QDialog>

#include <SVSCraftWidgets/svscraftwidgetsglobal.h>

namespace SVS {

    class ConfigurableDialogPrivate;

    class SVSCRAFT_WIDGETS_EXPORT ConfigurableDialog : public QDialog {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ConfigurableDialog)
    public:
        explicit ConfigurableDialog(QWidget *parent = nullptr);
        ~ConfigurableDialog();

        void done(int r) override;

    public:
        QWidget *widget() const;
        QWidget *takeWidget();
        void setWidget(QWidget *w);

        bool applyButtonVisible() const;
        void setApplyButtonVisible(bool visible);

    protected:
        virtual void apply();
        virtual void finish();

    Q_SIGNALS:
        void applied();
        void finished();

    protected:
        ConfigurableDialog(ConfigurableDialogPrivate &d, QWidget *parent = nullptr);

        QScopedPointer<ConfigurableDialogPrivate> d_ptr;
    };

}

#endif // SVSCRAFT_CONFIGURABLEDIALOG_H
