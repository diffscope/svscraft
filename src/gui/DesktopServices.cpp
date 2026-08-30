#define QT_NO_KEYWORDS

#include "DesktopServices.h"

#include <QDesktopServices>
#include <QFileInfo>
#include <QUrl>

namespace SVS {
    bool DesktopServices::reveal(const QString &filename) {
        const QFileInfo info(filename);
        if (!info.isDir() && !info.isFile()) {
            return false;
        }
        if (info.isDir() ? reveal(info.absoluteFilePath(), {}) : reveal(info.absolutePath(), {info.absoluteFilePath()})) {
            return true;
        }
        return QDesktopServices::openUrl(QUrl::fromLocalFile(info.isDir() ? filename : info.absolutePath()));
    }
}
