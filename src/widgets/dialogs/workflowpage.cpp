#include "workflowpage.h"

#include <QLabel>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include <QMap>

#include <QMWidgets/qmdecoratorv2.h>

namespace SVS {

    class WorkflowPagePrivate : public QObject {
        Q_DECLARE_PUBLIC(WorkflowPage)
    public:
        WorkflowPagePrivate();
        ~WorkflowPagePrivate();

        void init();

        void reloadStrings();

        QString buttonText(WorkflowPage::Button button) const;
        void setButtonText(WorkflowPage::Button button, const QString &text);

        WorkflowPage *q_ptr;

        QLabel *titleLabel;
        QLabel *descriptionLabel;
        QStackedWidget *stackedWidget;

        QVBoxLayout *rightLayout;
        QWidget *rightWidget;

        QWidget *emptyWidget;

        QSplitter *splitter;
        QVBoxLayout *mainLayout;

        WorkflowPage::Buttons buttons;

        struct ButtonData {
            QString text;
            QString overrideText;
            bool enabled;
            ButtonData() : enabled(true){};
        };

        QMap<WorkflowPage::Button, ButtonData> buttonsData;
    };

    WorkflowPagePrivate::WorkflowPagePrivate() {
    }

    WorkflowPagePrivate::~WorkflowPagePrivate() {
    }

    void WorkflowPagePrivate::init() {
        Q_Q(WorkflowPage);

        titleLabel = new QLabel();
        descriptionLabel = new QLabel();
        stackedWidget = new QStackedWidget();

        rightLayout = new QVBoxLayout();
        rightLayout->setMargin(0);
        rightLayout->setSpacing(0);

        rightLayout->addWidget(titleLabel);
        rightLayout->addWidget(descriptionLabel);
        rightLayout->addWidget(stackedWidget);

        rightWidget = new QWidget();
        rightWidget->setLayout(rightLayout);

        emptyWidget = new QWidget();

        splitter = new QSplitter(Qt::Horizontal);
        splitter->setChildrenCollapsible(false);
        splitter->addWidget(emptyWidget);
        splitter->addWidget(rightWidget);

        mainLayout = new QVBoxLayout();
        mainLayout->setMargin(0);
        mainLayout->setSpacing(0);
        mainLayout->addWidget(splitter);
        q->setLayout(mainLayout);

        titleLabel->hide();
        descriptionLabel->hide();
        emptyWidget->hide();

        qIDec->installLocale(this, _LOC(WorkflowPagePrivate, this));
    }

    void WorkflowPagePrivate::reloadStrings() {
        Q_Q(WorkflowPage);

        auto tryUpdateText = [&](WorkflowPage::Button button, const QString &text) {
            auto &data = buttonsData[button];
            data.text = text;
            if (data.overrideText.isEmpty()) {
                Q_EMIT q->buttonTextChanged(button, text);
            }
        };

        tryUpdateText(WorkflowPage::PreviousButton, WorkflowPage::tr("Previous"));
        tryUpdateText(WorkflowPage::NextButton, WorkflowPage::tr("Next"));
        tryUpdateText(WorkflowPage::FinishButton, WorkflowPage::tr("Finish"));
        tryUpdateText(WorkflowPage::OkButton, WorkflowPage::tr("Confirm"));
        tryUpdateText(WorkflowPage::CancelButton, WorkflowPage::tr("Cancel"));
        tryUpdateText(WorkflowPage::HelpButton, WorkflowPage::tr("Help"));
    }

    QString WorkflowPagePrivate::buttonText(WorkflowPage::Button button) const {
        auto &data = buttonsData[button];
        return data.overrideText.isEmpty() ? data.text : data.overrideText;
    }

    void WorkflowPagePrivate::setButtonText(WorkflowPage::Button button, const QString &text) {
        Q_Q(WorkflowPage);
        auto &data = buttonsData[button];
        data.overrideText = text;
        Q_EMIT q->buttonTextChanged(button, text.isEmpty() ? data.text : data.overrideText);
    }

    WorkflowPage::WorkflowPage(QWidget *parent)
        : WorkflowPage(*new WorkflowPagePrivate(), parent) {
    }

    WorkflowPage::~WorkflowPage() {
    }

    QSplitter *WorkflowPage::splitter() const {
        Q_D(const WorkflowPage);
        return d->splitter;
    }

    QString WorkflowPage::title() const {
        Q_D(const WorkflowPage);
        return d->titleLabel->text();
    }

    void WorkflowPage::setTitle(const QString &title) {
        Q_D(WorkflowPage);
        d->titleLabel->setText(title);
        d->titleLabel->setHidden(title.isEmpty());
    }

    QString WorkflowPage::description() const {
        Q_D(const WorkflowPage);
        return d->descriptionLabel->text();
    }

    void WorkflowPage::setDescription(const QString &description) {
        Q_D(WorkflowPage);
        d->descriptionLabel->setText(description);
        d->descriptionLabel->setHidden(description.isEmpty());
    }

    QWidget *WorkflowPage::takeWidget() {
        Q_D(WorkflowPage);
        auto w = d->stackedWidget->widget(0);
        if (!w)
            return w;
        d->stackedWidget->removeWidget(w);
        return w;
    }

    QWidget *WorkflowPage::widget() const {
        Q_D(const WorkflowPage);
        return d->stackedWidget->widget(0);
    }

    void WorkflowPage::setWidget(QWidget *w) {
        Q_D(WorkflowPage);
        delete takeWidget();
        d->stackedWidget->addWidget(w);
    }

    QWidget *WorkflowPage::takeSideWidget() {
        Q_D(WorkflowPage);
        if (d->emptyWidget)
            return nullptr;

        auto w = d->splitter->widget(0);
        d->splitter->replaceWidget(0, d->emptyWidget = new QWidget());
        d->emptyWidget->hide();
        return w;
    }

    QWidget *WorkflowPage::sideWidget() const {
        Q_D(const WorkflowPage);
        if (d->emptyWidget)
            return nullptr;

        return d->splitter->widget(0);
    }

    void WorkflowPage::setSideWidget(QWidget *w) {
        Q_D(WorkflowPage);

        auto org = d->splitter->widget(0);
        delete org;
        d->emptyWidget = nullptr;
        d->splitter->insertWidget(0, w);

        d->splitter->setStretchFactor(0, 0);
        d->splitter->setStretchFactor(1, 1);

        w->show();
    }

    WorkflowPage::Buttons WorkflowPage::buttons() const {
        Q_D(const WorkflowPage);
        return d->buttons;
    }

    void WorkflowPage::setButtons(WorkflowPage::Buttons buttons) {
        Q_D(WorkflowPage);
        d->buttons = buttons;
        Q_EMIT buttonsChanged(buttons);
    }

    void WorkflowPage::setButton(WorkflowPage::Button button, bool visible) {
        Q_D(WorkflowPage);
        d->buttons = visible ? (d->buttons | button) : (d->buttons & ~button);
        Q_EMIT buttonsChanged(d->buttons);
    }

    QString WorkflowPage::buttonText(WorkflowPage::Button which) const {
        Q_D(const WorkflowPage);
        return d->buttonText(which);
    }

    void WorkflowPage::setButtonText(WorkflowPage::Button which, const QString &text) {
        Q_D(WorkflowPage);
        d->setButtonText(which, text);
    }

    bool WorkflowPage::buttonEnabled(WorkflowPage::Button which) const {
        Q_D(const WorkflowPage);
        return d->buttonsData[which].enabled;
    }

    void WorkflowPage::setButtonEnabled(WorkflowPage::Button which, bool enabled) {
        Q_D(WorkflowPage);
        d->buttonsData[which].enabled = enabled;
        Q_EMIT buttonEnabledChanged(which, enabled);
    }

    void WorkflowPage::clearPage() {
    }

    bool WorkflowPage::validatePage() {
        return true;
    }

    WorkflowPage::WorkflowPage(WorkflowPagePrivate &d, QWidget *parent)
        : QFrame(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.init();
    }

}