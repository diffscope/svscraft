#define QT_NO_KEYWORDS

#include "DesktopServices.h"

#include <QFileInfo>
#include <QProcess>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>

#ifdef Q_OS_LINUX
#   include <QDBusInterface>
#   include <QDBusReply>
#   include <gio/gio.h>
#endif


namespace SVS {
    bool DesktopServices::reveal(const QString &filename) {
        QFileInfo info(filename);
        do {
#if defined(Q_OS_WINDOWS)
            if (info.isFile()) {
                QStringList cmds;
                cmds << "/e,"
                     << "/select," << QDir::toNativeSeparators(filename);
                if (QProcess::startDetached("explorer.exe", cmds)) {
                    return true;
                }
            } else if (info.isDir()) {
                QStringList cmds;
                cmds << "/e,"
                     << "/root," << QDir::toNativeSeparators(filename);
                if (QProcess::startDetached("explorer.exe", cmds)) {
                    return true;
                }
            } else {
                return false;
            }
#elif defined(Q_OS_MAC)
            if (info.isDir()) {
                QString dirname = filename;
                if (!dirname.endsWith("/")) {
                    dirname.append("/");
                }
                if (QProcess::startDetached("bash", {"-c", "open \'" + dirname + "\'"})) {
                    return true;
                }
            } else if (info.isFile()) {
                QStringList scriptArgs;
                scriptArgs << QLatin1String("-e")
                           << QString::fromLatin1(
                                  "tell application \"Finder\" to reveal POSIX file \"%1\"")
                                  .arg(filename);
                if (QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs) != 0) {
                    break;
                }
                scriptArgs.clear();
                scriptArgs << QLatin1String("-e")
                           << QLatin1String("tell application \"Finder\" to activate");
                if (QProcess::execute("/usr/bin/osascript", scriptArgs) != 0) {
                    break;
                }
                return true;
            } else {
                return false;
            }
#else
            if (!info.isDir() && !info.isFile()) {
                return false;
            }
            QUrl url = QUrl::fromLocalFile(filename);

            QStringList uris;
            uris << url.toString();

            QDBusInterface iface(
                QStringLiteral("org.freedesktop.FileManager1"),
                QStringLiteral("/org/freedesktop/FileManager1"),
                QStringLiteral("org.freedesktop.FileManager1"),
                QDBusConnection::sessionBus()
            );

            if (!iface.isValid()) {
                break;
            }

            QDBusReply<void> reply = iface.call(info.isDir() ? QStringLiteral("ShowFolders") : QStringLiteral("ShowItems"), uris, QString(""));

            if (!reply.isValid()) {
                break;
            }
            return true;
#endif
        } while (false);
        return QDesktopServices::openUrl(QUrl::fromLocalFile(info.isDir() ? filename : info.absolutePath()));
    }
    QString DesktopServices::fileManagerName() {
#if defined(Q_OS_WINDOWS)
        return tr("File Explorer");
#elif defined(Q_OS_MAC)
        return tr("Finder")
#else
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
#endif
    }
}
