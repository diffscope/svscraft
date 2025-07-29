#include "DesktopServices.h"

#include <QFileInfo>
#include <QProcess>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>

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
                if (!dirname.endsWith(Slash)) {
                    dirname.append(Slash);
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
            if (info.isDir()) {
                if (QProcess::startDetached("bash", {"-c", "xdg-open \'" + filename + "\'"})) {
                    return true;
                }
            } else if (info.isFile()) {
                QString arg = info.absolutePath();
                if (QProcess::startDetached("bash", {"-c", "xdg-open \'" + arg + "\'"})) {
                    return true;
                }
            } else {
                return false;
            }
#endif
        } while (false);
        return QDesktopServices::openUrl(QUrl::fromLocalFile(info.isDir() ? filename : info.absolutePath()));
    }
}