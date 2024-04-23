#include "configurabledialog.h"

#include <QMWidgets/ctabbutton.h>
#include <QMWidgets/qmdecoratorv2.h>
#include <QMWidgets/qmequalboxlayout.h>

namespace SVS {

    class ConfigurableDialogPrivate : public QObject {
        Q_DECLARE_PUBLIC(ConfigurableDialog)
    public:
        ConfigurableDialogPrivate();
        virtual ~ConfigurableDialogPrivate();

        void init();

        void reloadStrings();

        ConfigurableDialog *q_ptr;

        CTabButton *okButton, *cancelButton, *applyButton;
        QMEqualBoxLayout *buttonsLayout;

        QWidget *widget;

        QVBoxLayout *mainLayout;

        void apply_helper();
    };


    ConfigurableDialogPrivate::ConfigurableDialogPrivate() {
        widget = nullptr;
    }

    ConfigurableDialogPrivate::~ConfigurableDialogPrivate() {
    }

    void ConfigurableDialogPrivate::init() {
        Q_Q(ConfigurableDialog);

        q->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        q->setWindowFlag(Qt::WindowCloseButtonHint, true);

        okButton = new CTabButton();
        cancelButton = new CTabButton();
        applyButton = new CTabButton();

        buttonsLayout = new QMEqualBoxLayout(QBoxLayout::LeftToRight);
        buttonsLayout->setAlignment(Qt::AlignRight);
        buttonsLayout->addWidget2(okButton);
        buttonsLayout->addWidget2(cancelButton);
        buttonsLayout->addWidget2(applyButton);

        mainLayout = new QVBoxLayout();
        mainLayout->addLayout(buttonsLayout);

        q->setLayout(mainLayout);

        connect(okButton, &QPushButton::clicked, q, &ConfigurableDialog::accept);
        connect(cancelButton, &QPushButton::clicked, q, &ConfigurableDialog::reject);
        connect(applyButton, &QPushButton::clicked, this, &ConfigurableDialogPrivate::apply_helper);

        qIDec->installLocale(this);
    }

    void ConfigurableDialogPrivate::reloadStrings() {
        okButton->setText(ConfigurableDialog::tr("OK"));
        cancelButton->setText(ConfigurableDialog::tr("Cancel"));
        applyButton->setText(ConfigurableDialog::tr("Apply"));
    }

    void ConfigurableDialogPrivate::apply_helper() {
        Q_Q(ConfigurableDialog);
        q->apply();
        Q_EMIT q->applied();
    }

    ConfigurableDialog::ConfigurableDialog(QWidget *parent)
        : ConfigurableDialog(*new ConfigurableDialogPrivate(), parent) {
    }

    ConfigurableDialog::~ConfigurableDialog() {
    }

    void ConfigurableDialog::done(int r) {
        Q_D(ConfigurableDialog);

        if (r == Accepted) {
            d->apply_helper();
        }

        finish();
        Q_EMIT finished();

        QDialog::done(r);
    }

    QWidget *ConfigurableDialog::widget() const {
        Q_D(const ConfigurableDialog);
        return d->widget;
    }

    QWidget *ConfigurableDialog::takeWidget() {
        Q_D(ConfigurableDialog);
        if (!d->widget)
            return nullptr;

        d->widget = nullptr;
        return d->mainLayout->takeAt(0)->widget();
    }

    void ConfigurableDialog::setWidget(QWidget *w) {
        Q_D(ConfigurableDialog);

        delete takeWidget();

        d->widget = w;
        d->mainLayout->insertWidget(0, w);
    }

    bool ConfigurableDialog::applyButtonVisible() const {
        Q_D(const ConfigurableDialog);
        return d->applyButton->isVisible();
    }

    void ConfigurableDialog::setApplyButtonVisible(bool visible) {
        Q_D(ConfigurableDialog);
        d->applyButton->setVisible(visible);
    }

    void ConfigurableDialog::apply() {
    }

    void ConfigurableDialog::finish() {
    }

    ConfigurableDialog::ConfigurableDialog(ConfigurableDialogPrivate &d, QWidget *parent)
        : QDialog(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
