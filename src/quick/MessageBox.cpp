#include "MessageBox.h"
#include "MessageBox_p.h"

#include <memory>

#include <QQuickItem>
#include <QQuickWindow>
#include <QEventLoop>

namespace SVS {

    MessageBox *MessageBoxAttachedType::qmlAttachedProperties(QObject *object) {
        QQuickItem *item = qobject_cast<QQuickItem *>(object);
        if (!item)
            qmlWarning(object) << "MessageBox should be attached to an Item";
        return new MessageBox(item);
    }
    void MessageBoxDialogDoneListener::done(const QVariant &id) const {
        eventLoop->exit(id.toInt());
    }

    MessageBox::MessageBox(QQuickItem *parent) : QObject(parent), m_item(parent) {
    }
    MessageBox::~MessageBox() = default;
    SVSCraft::StandardButton MessageBox::information(const QString &title, const QString &text,
                                                     SVSCraft::StandardButtons buttons,
                                                     SVSCraft::StandardButton defaultButton) const {
        return information(qmlEngine(m_item), m_item->window(), title, text, buttons, defaultButton);
    }
    SVSCraft::StandardButton MessageBox::warning(const QString &title, const QString &text,
                                                 SVSCraft::StandardButtons buttons,
                                                 SVSCraft::StandardButton defaultButton) const {
        return warning(qmlEngine(m_item), m_item->window(), title, text, buttons, defaultButton);
    }
    SVSCraft::StandardButton MessageBox::critical(const QString &title, const QString &text,
                                                  SVSCraft::StandardButtons buttons,
                                                  SVSCraft::StandardButton defaultButton) const {
        return critical(qmlEngine(m_item), m_item->window(), title, text, buttons, defaultButton);
    }
    SVSCraft::StandardButton MessageBox::question(const QString &title, const QString &text,
                                                  SVSCraft::StandardButtons buttons,
                                                  SVSCraft::StandardButton defaultButton) const {
        return question(qmlEngine(m_item), m_item->window(), title, text, buttons, defaultButton);
    }
    SVSCraft::StandardButton MessageBox::success(const QString &title, const QString &text,
                                                 SVSCraft::StandardButtons buttons,
                                                 SVSCraft::StandardButton defaultButton) const {
        return success(qmlEngine(m_item), m_item->window(), title, text, buttons, defaultButton);
    }

    static SVSCraft::StandardButton showMessageBox(QQmlEngine *engine, QWindow *parent, const QString &title, const QString &text, SVSCraft::StandardButtons buttons, SVSCraft::StandardButton defaultButton, SVSCraft::MessageBoxIcon icon) {
        if (!engine) {
            qWarning() << "SVS::showMessageBox: QML engine is null";
            return SVSCraft::NoButton;
        }
        QQmlComponent component(engine, ":/qt/qml/SVSCraft/UIComponents/qml/MessageBoxDialog.qml");
        std::unique_ptr<QQuickWindow> mb(qobject_cast<QQuickWindow *>(component.create()));
        Q_ASSERT(mb);
        mb->setTransientParent(parent);
        mb->setProperty("text", title);
        mb->setProperty("informativeText", text);
        mb->setProperty("buttons", buttons.toInt());
        mb->setProperty("primaryButton", defaultButton);
        mb->setProperty("icon", icon);
        QEventLoop eventLoop;
        MessageBoxDialogDoneListener listener(&eventLoop);
        QObject::connect(mb.get(), SIGNAL(done(QVariant)), &listener, SLOT(done(QVariant)));
        mb->show();
        return static_cast<SVSCraft::StandardButton>(eventLoop.exec());

    }

    SVSCraft::StandardButton MessageBox::information(QQmlEngine *engine, QWindow *parent,
                                                     const QString &title,
                                                     const QString &text,
                                                     SVSCraft::StandardButtons buttons,
                                                     SVSCraft::StandardButton defaultButton) {
        return showMessageBox(engine, parent, title, text, buttons, defaultButton, SVSCraft::Information);
    }
    SVSCraft::StandardButton MessageBox::warning(QQmlEngine *engine, QWindow *parent,
                                                 const QString &title,
                                                 const QString &text,
                                                 SVSCraft::StandardButtons buttons,
                                                 SVSCraft::StandardButton defaultButton) {
        return showMessageBox(engine, parent, title, text, buttons, defaultButton, SVSCraft::Warning);
    }
    SVSCraft::StandardButton MessageBox::critical(QQmlEngine *engine, QWindow *parent,
                                                  const QString &title,
                                                  const QString &text,
                                                  SVSCraft::StandardButtons buttons,
                                                  SVSCraft::StandardButton defaultButton) {
        return showMessageBox(engine, parent, title, text, buttons, defaultButton, SVSCraft::Critical);
    }
    SVSCraft::StandardButton MessageBox::question(QQmlEngine *engine, QWindow *parent,
                                                  const QString &title,
                                                  const QString &text,
                                                  SVSCraft::StandardButtons buttons,
                                                  SVSCraft::StandardButton defaultButton) {
        return showMessageBox(engine, parent, title, text, buttons, defaultButton, SVSCraft::Question);
    }
    SVSCraft::StandardButton MessageBox::success(QQmlEngine *engine, QWindow *parent,
                                                 const QString &title,
                                                 const QString &text,
                                                 SVSCraft::StandardButtons buttons,
                                                 SVSCraft::StandardButton defaultButton) {
        return showMessageBox(engine, parent, title, text, buttons, defaultButton, SVSCraft::Success);
    }
}