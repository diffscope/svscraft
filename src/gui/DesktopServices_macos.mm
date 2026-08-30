#define QT_NO_KEYWORDS

#include "DesktopServices.h"

#include <QDir>

#include <Cocoa/Cocoa.h>

namespace SVS {
    bool DesktopServices::reveal(const QString &dir, const QStringList &files) {
        const QDir root(dir);
        @autoreleasepool {
            if (files.isEmpty()) {
                return [[NSWorkspace sharedWorkspace]
                    openURL:[NSURL fileURLWithPath:root.absolutePath().toNSString()]];
            }
            NSMutableArray<NSURL *> *urls =
                [NSMutableArray arrayWithCapacity:static_cast<NSInteger>(files.size())];
            for (const QString &file : files) {
                [urls addObject:[NSURL fileURLWithPath:root.absoluteFilePath(file).toNSString()]];
            }
            [[NSWorkspace sharedWorkspace] activateFileViewerSelectingURLs:urls];
            return true;
        }
    }

    QString DesktopServices::fileManagerName() {
        return tr("Finder");
    }
}
