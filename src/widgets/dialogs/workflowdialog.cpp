#include "workflowdialog.h"

#include <QResizeEvent>
#include <QPropertyAnimation>
#include <QLabel>

#include <QMWidgets/ctabbutton.h>
#include <QMWidgets/qmequalboxlayout.h>
#include <QMWidgets/qmview.h>

namespace SVS {

    static int AnimationDuration = 250;

    class QMWorkflowDialogContainer;

    class WorkflowDialogPrivate : public QObject {
        Q_DECLARE_PUBLIC(WorkflowDialog)
    public:
        WorkflowDialogPrivate();
        ~WorkflowDialogPrivate();

        void init();

        WorkflowDialog *q_ptr;

        CTabButton *prevButton, *nextButton, *finishButton, *okButton, *cancelButton, *helpButton;
        QMEqualBoxLayout *buttonsLayout;

        WorkflowPage *widget;

        QMWorkflowDialogContainer *container;
        QVBoxLayout *mainLayout;

        QPropertyAnimation *m_animation;
        QPropertyAnimation *m_animation2;
        QLabel *m_label;

        QMap<WorkflowPage::Button, QAbstractButton *> buttons;

        void prepareTransition();
        virtual void doPrevTransition();
        virtual void doNextTransition();

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;

    private:
        void _q_prevClicked();
        void _q_nextClicked();
        void _q_finishClicked();
        void _q_helpClicked();

        void _q_buttonsChanged(WorkflowPage::Buttons buttons);
        void _q_buttonTextChanged(WorkflowPage::Button which, const QString &text);
        void _q_buttonEnabledChanged(WorkflowPage::Button which, bool enabled);
    };

    WorkflowDialogPrivate::WorkflowDialogPrivate() {
        widget = nullptr;
    }

    WorkflowDialogPrivate::~WorkflowDialogPrivate() {
    }

    class QMWorkflowDialogContainer : public QWidget {
    public:
        explicit QMWorkflowDialogContainer(WorkflowDialogPrivate *d, QWidget *parent = nullptr)
            : QWidget(parent), d(d) {
        }

        ~QMWorkflowDialogContainer() {
        }

        QSize sizeHint() const override {
            return d->widget ? d->widget->sizeHint() : QWidget::sizeHint();
        }

        QSize minimumSizeHint() const override {
            return d->widget ? d->widget->minimumSizeHint() : QWidget::minimumSizeHint();
        }

        bool event(QEvent *event) override {
            switch (event->type()) {
                case QEvent::LayoutRequest:
                    updateGeometry();
                    break;
                default:
                    break;
            }
            return QWidget::event(event);
        }

        WorkflowDialogPrivate *d;
    };

    void WorkflowDialogPrivate::init() {
        Q_Q(WorkflowDialog);

        q->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        q->setWindowFlag(Qt::WindowCloseButtonHint, true);

        m_animation = new QPropertyAnimation(this);
        m_animation2 = new QPropertyAnimation(this);

        m_animation->setDuration(AnimationDuration);
        m_animation2->setDuration(AnimationDuration);

        m_animation->setEasingCurve(QEasingCurve::OutCubic);
        m_animation2->setEasingCurve(QEasingCurve::OutCubic);

        container = new QMWorkflowDialogContainer(this);
        container->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        container->installEventFilter(this);

        m_label = new QLabel(container);
        m_label->hide();

        prevButton = new CTabButton();
        nextButton = new CTabButton();
        finishButton = new CTabButton();
        okButton = new CTabButton();
        cancelButton = new CTabButton();
        helpButton = new CTabButton();

        buttons[WorkflowPage::PreviousButton] = prevButton;
        buttons[WorkflowPage::NextButton] = nextButton;
        buttons[WorkflowPage::FinishButton] = finishButton;
        buttons[WorkflowPage::OkButton] = okButton;
        buttons[WorkflowPage::CancelButton] = cancelButton;
        buttons[WorkflowPage::HelpButton] = helpButton;

        buttonsLayout = new QMEqualBoxLayout(QBoxLayout::LeftToRight);
        buttonsLayout->setAlignment(Qt::AlignRight);
        buttonsLayout->addWidget2(prevButton);
        buttonsLayout->addWidget2(nextButton);
        buttonsLayout->addWidget2(finishButton);
        buttonsLayout->addWidget2(okButton);
        buttonsLayout->addWidget2(cancelButton);
        buttonsLayout->addWidget2(helpButton);

        mainLayout = new QVBoxLayout();
        mainLayout->addWidget(container);
        mainLayout->addLayout(buttonsLayout);

        q->setLayout(mainLayout);

        connect(prevButton, &QPushButton::clicked, this, &WorkflowDialogPrivate::_q_prevClicked);
        connect(nextButton, &QPushButton::clicked, this, &WorkflowDialogPrivate::_q_nextClicked);
        connect(okButton, &QPushButton::clicked, this, &WorkflowDialogPrivate::_q_finishClicked);
        connect(finishButton, &QPushButton::clicked, this,
                &WorkflowDialogPrivate::_q_finishClicked);
        connect(cancelButton, &QPushButton::clicked, q, &WorkflowDialog::reject);
        connect(helpButton, &QPushButton::clicked, this, &WorkflowDialogPrivate::_q_helpClicked);

        q->setPage(nullptr);
    }

    void WorkflowDialogPrivate::prepareTransition() {
        Q_Q(WorkflowDialog);

        if (!widget) {
            m_label->setPixmap(QPixmap());
            return;
        }

        auto pixmap = QMView::createPixmap(widget->size(), q->window()->windowHandle());
        pixmap.fill(Qt::transparent);
        widget->render(&pixmap);

        m_label->setPixmap(pixmap);
    }

    void WorkflowDialogPrivate::doPrevTransition() {
        Q_Q(WorkflowDialog);

        m_label->adjustSize();
        m_label->move(0, 0);

        widget->setGeometry(QRect(QPoint(-container->width(), 0), container->size()));

        QRect labelEnd(QPoint(container->width(), 0), m_label->size());
        QRect widgetEnd(QPoint(), widget->size());

        m_animation->setTargetObject(m_label);
        m_animation->setPropertyName("geometry");
        m_animation->setStartValue(m_label->geometry());
        m_animation->setEndValue(labelEnd);

        m_animation2->setTargetObject(widget);
        m_animation2->setPropertyName("geometry");
        m_animation2->setStartValue(widget->geometry());
        m_animation2->setEndValue(widgetEnd);

        m_label->show();

        m_animation->start();
        m_animation2->start();
    }

    void WorkflowDialogPrivate::doNextTransition() {
        Q_Q(WorkflowDialog);

        m_label->adjustSize();
        m_label->move(0, 0);

        widget->setGeometry(QRect(QPoint(container->width(), 0), container->size()));

        QRect labelEnd(QPoint(-container->width(), 0), m_label->size());
        QRect widgetEnd(QPoint(), widget->size());

        m_animation->setTargetObject(m_label);
        m_animation->setPropertyName("geometry");
        m_animation->setStartValue(m_label->geometry());
        m_animation->setEndValue(labelEnd);

        m_animation2->setTargetObject(widget);
        m_animation2->setPropertyName("geometry");
        m_animation2->setStartValue(widget->geometry());
        m_animation2->setEndValue(widgetEnd);

        m_label->show();

        m_animation->start();
        m_animation2->start();
    }

    bool WorkflowDialogPrivate::eventFilter(QObject *obj, QEvent *event) {
        Q_Q(WorkflowDialog);

        if (obj == container) {
            switch (event->type()) {
                case QEvent::Resize: {
                    m_animation->stop();
                    m_animation2->stop();

                    m_label->hide();

                    if (widget) {
                        widget->setGeometry(
                            QRect(QPoint(), static_cast<QResizeEvent *>(event)->size()));
                    }
                    break;
                }
                default:
                    break;
            }
        }

        return QObject::eventFilter(obj, event);
    }

    void WorkflowDialogPrivate::_q_prevClicked() {
        Q_Q(WorkflowDialog);

        if (m_animation->state() == QAbstractAnimation::Running) {
            return;
        }

        widget->clearPage();

        prepareTransition();

        if (!q->prev()) {
            return;
        }
        Q_EMIT q->aboutToPrev();

        if (widget)
            doPrevTransition();
    }

    void WorkflowDialogPrivate::_q_nextClicked() {
        Q_Q(WorkflowDialog);

        if (m_animation->state() == QAbstractAnimation::Running) {
            return;
        }

        if (!widget->validatePage()) {
            return;
        }

        prepareTransition();

        if (!q->next()) {
            return;
        }
        Q_EMIT q->aboutToNext();

        if (widget)
            doNextTransition();
    }

    void WorkflowDialogPrivate::_q_finishClicked() {
        Q_Q(WorkflowDialog);

        if (m_animation->state() == QAbstractAnimation::Running) {
            return;
        }

        if (!widget->validatePage()) {
            return;
        }

        if (!q->finish()) {
            return;
        }
        Q_EMIT q->finished();

        q->accept();
    }

    void WorkflowDialogPrivate::_q_helpClicked() {
        Q_Q(WorkflowDialog);

        if (widget)
            Q_EMIT widget->helpRequested();

        Q_EMIT q->helpRequested();
    }

    void WorkflowDialogPrivate::_q_buttonsChanged(WorkflowPage::Buttons buttons) {
        for (auto it = this->buttons.begin(); it != this->buttons.end(); ++it) {
            auto key = it.key();
            auto btn = it.value();
            btn->setVisible(buttons & key);
            btn->setText(widget->buttonText(key));
            btn->setEnabled(widget->buttonEnabled(key));
        }

        if (cancelButton->isDefault() || helpButton->isDefault()) {
            if (buttons & WorkflowPage::FinishButton && finishButton->isEnabled()) {
                finishButton->setDefault(true);
            } else if (buttons & WorkflowPage::OkButton && okButton->isEnabled()) {
                okButton->setDefault(true);
            } else if (buttons & WorkflowPage::NextButton && nextButton->isEnabled()) {
                nextButton->setDefault(true);
            } else if (buttons & WorkflowPage::PreviousButton && prevButton->isEnabled()) {
                prevButton->setDefault(true);
            }
        }
    }

    void WorkflowDialogPrivate::_q_buttonTextChanged(WorkflowPage::Button which,
                                                      const QString &text) {
        buttons[which]->setText(text);
    }

    void WorkflowDialogPrivate::_q_buttonEnabledChanged(WorkflowPage::Button which,
                                                         bool enabled) {
        buttons[which]->setEnabled(enabled);
    }

    WorkflowDialog::WorkflowDialog(QWidget *parent)
        : WorkflowDialog(*new WorkflowDialogPrivate(), parent) {
    }

    WorkflowDialog::~WorkflowDialog() {
    }

    WorkflowPage *WorkflowDialog::page() const {
        Q_D(const WorkflowDialog);
        return d->widget;
    }

    WorkflowPage *WorkflowDialog::takePage() {
        Q_D(WorkflowDialog);
        auto org = d->widget;
        if (org) {
            d->m_animation->stop();
            d->m_animation2->stop();

            disconnect(org, &WorkflowPage::buttonsChanged, d,
                       &WorkflowDialogPrivate::_q_buttonsChanged);
            disconnect(org, &WorkflowPage::buttonTextChanged, d,
                       &WorkflowDialogPrivate::_q_buttonTextChanged);
            disconnect(org, &WorkflowPage::buttonEnabledChanged, d,
                       &WorkflowDialogPrivate::_q_buttonEnabledChanged);

            org->setParent(nullptr);

            d->widget = nullptr;
            d->container->updateGeometry();
        }
        return org;
    }

    void WorkflowDialog::setPage(WorkflowPage *w) {
        Q_D(WorkflowDialog);

        delete takePage();

        if (!w) {
            for (const auto &btn : qAsConst(d->buttons)) {
                btn->hide();
            }
            return;
        }

        w->setParent(d->container);
        w->setGeometry(QRect(QPoint(), d->container->size()));
        w->show();

        d->widget = w;
        d->container->updateGeometry();

        // Update buttons first
        d->_q_buttonsChanged(w->buttons());

        connect(w, &WorkflowPage::buttonsChanged, d, &WorkflowDialogPrivate::_q_buttonsChanged);
        connect(w, &WorkflowPage::buttonTextChanged, d,
                &WorkflowDialogPrivate::_q_buttonTextChanged);
        connect(w, &WorkflowPage::buttonEnabledChanged, d,
                &WorkflowDialogPrivate::_q_buttonEnabledChanged);
    }

    bool WorkflowDialog::prev() {
        return true;
    }

    bool WorkflowDialog::next() {
        return true;
    }

    bool WorkflowDialog::finish() {
        return true;
    }

    WorkflowDialog::WorkflowDialog(WorkflowDialogPrivate &d, QWidget *parent)
        : QDialog(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}