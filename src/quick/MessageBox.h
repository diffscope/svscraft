#ifndef SVSCRAFT_MESSAGEBOX_H
#define SVSCRAFT_MESSAGEBOX_H

#include <functional>

#include <QObject>
#include <qqmlintegration.h>

#include <SVSCraftCore/SVSCraftNamespace.h>
#include <SVSCraftQuick/SVSCraftQuickGlobal.h>

class QWindow;
class QQuickItem;
class QQmlEngine;

namespace SVS {

    class MessageBoxAttachedType;

    class SVSCRAFT_QUICK_EXPORT MessageBox : public QObject {
        Q_OBJECT
        QML_ANONYMOUS
    public:
        ~MessageBox() override;

        Q_INVOKABLE SVSCraft::StandardButton information(
            const QString &title,
            const QString &text,
            SVSCraft::StandardButtons buttons = SVSCraft::Ok,
            SVSCraft::StandardButton defaultButton = SVSCraft::Ok) const;

        Q_INVOKABLE SVSCraft::StandardButton warning(
            const QString &title,
            const QString &text,
            SVSCraft::StandardButtons buttons = SVSCraft::Ok,
            SVSCraft::StandardButton defaultButton = SVSCraft::Ok) const;

        Q_INVOKABLE SVSCraft::StandardButton critical(
            const QString &title,
            const QString &text,
            SVSCraft::StandardButtons buttons = SVSCraft::Ok,
            SVSCraft::StandardButton defaultButton = SVSCraft::Ok) const;

        Q_INVOKABLE SVSCraft::StandardButton question(
            const QString &title,
            const QString &text,
            SVSCraft::StandardButtons buttons = SVSCraft::Yes | SVSCraft::No,
            SVSCraft::StandardButton defaultButton = SVSCraft::NoButton) const;

        Q_INVOKABLE SVSCraft::StandardButton success(
            const QString &title,
            const QString &text,
            SVSCraft::StandardButtons buttons = SVSCraft::Ok,
            SVSCraft::StandardButton defaultButton = SVSCraft::Ok) const;

        static SVSCraft::StandardButton information(
            QQmlEngine *engine,
            QWindow *parent,
            const QString &title,
            const QString &text,
            SVSCraft::StandardButtons buttons = SVSCraft::Ok,
            SVSCraft::StandardButton defaultButton = SVSCraft::Ok);

        static SVSCraft::StandardButton warning(
            QQmlEngine *engine,
            QWindow *parent,
            const QString &title,
            const QString &text,
            SVSCraft::StandardButtons buttons = SVSCraft::Ok,
            SVSCraft::StandardButton defaultButton = SVSCraft::Ok);

        static SVSCraft::StandardButton critical(
            QQmlEngine *engine,
            QWindow *parent,
            const QString &title,
            const QString &text,
            SVSCraft::StandardButtons buttons = SVSCraft::Ok,
            SVSCraft::StandardButton defaultButton = SVSCraft::Ok);

        static SVSCraft::StandardButton question(
            QQmlEngine *engine,
            QWindow *parent,
            const QString &title,
            const QString &text,
            SVSCraft::StandardButtons buttons = SVSCraft::Yes | SVSCraft::No,
            SVSCraft::StandardButton defaultButton = SVSCraft::NoButton);

        static SVSCraft::StandardButton success(
            QQmlEngine *engine,
            QWindow *parent,
            const QString &title,
            const QString &text,
            SVSCraft::StandardButtons buttons = SVSCraft::Ok,
            SVSCraft::StandardButton defaultButton = SVSCraft::Ok);

        using AlertHandler = std::function<void(QObject *)>;
        static AlertHandler alertHandler();
        static void setAlertHandler(const AlertHandler &handler);
        static void defaultAlertHandler(QObject *messageBox);

    private:
        friend class MessageBoxAttachedType;
        explicit MessageBox(QQuickItem *parent = nullptr);

        QQuickItem *m_item;

    };

}

#endif //SVSCRAFT_MESSAGEBOX_H
