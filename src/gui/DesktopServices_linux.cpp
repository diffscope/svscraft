#define QT_NO_KEYWORDS

#include "DesktopServices.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDir>
#include <QUrl>

#include <gio/gio.h>

namespace SVS {
    bool DesktopServices::reveal(const QString &dir, const QStringList &files) {
        QDBusInterface iface(
            QStringLiteral("org.freedesktop.FileManager1"),
            QStringLiteral("/org/freedesktop/FileManager1"),
            QStringLiteral("org.freedesktop.FileManager1"),
            QDBusConnection::sessionBus()
        );

        if (!iface.isValid()) {
            return false;
        }

        const QDir root(dir);
        QStringList uris;
        uris.reserve(files.size());
        for (const QString &file : files) {
            const QString path = QDir::cleanPath(root.absoluteFilePath(file));
            uris << QUrl::fromLocalFile(path).toString();
        }

        QDBusReply<void> reply;
        if (uris.isEmpty()) {
            reply = iface.call(QStringLiteral("ShowFolders"),
                               QStringList() << QUrl::fromLocalFile(root.absolutePath()).toString(),
                               QString());
        } else {
            reply = iface.call(QStringLiteral("ShowItems"), uris, QString());
        }

        return reply.isValid();
    }

    QString DesktopServices::fileManagerName() {
        static QString name;
        static const QString defaultName = tr("File Manager");
        if (!name.isEmpty()) {
            return name;
        }
        if (GAppInfo *appInfo = g_app_info_get_default_for_type("inode/directory", FALSE)) {
            name = QString::fromUtf8(g_app_info_get_display_name(appInfo));
            g_object_unref(appInfo);
            return name;
        } else {
            return name = defaultName;
        }
    }
}
