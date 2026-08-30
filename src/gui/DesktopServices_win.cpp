#define QT_NO_KEYWORDS

#include "DesktopServices.h"

#include <vector>

#include <QDir>

#include <windows.h>
#include <shlobj.h>
#include <Shlwapi.h>

namespace SVS {
    bool DesktopServices::reveal(const QString &dir, const QStringList &files) {
        const QDir root(dir);

        PIDLIST_ABSOLUTE dirPidl = nullptr;
        const QString nativeDirPath = QDir::toNativeSeparators(root.absolutePath());
        HRESULT hr = SHParseDisplayName(reinterpret_cast<const wchar_t *>(nativeDirPath.utf16()), nullptr, &dirPidl, 0, nullptr);
        if (FAILED(hr)) {
            return false;
        }

        std::vector<PIDLIST_ABSOLUTE> ownedItems;
        std::vector<LPCITEMIDLIST> items;
        items.reserve(files.size());
        for (const QString &file : files) {
            const QString nativeFilePath =
                QDir::toNativeSeparators(QDir::cleanPath(root.absoluteFilePath(file)));
            PIDLIST_ABSOLUTE itemPidl = nullptr;
            if (FAILED(SHParseDisplayName(reinterpret_cast<const wchar_t *>(nativeFilePath.utf16()), nullptr, &itemPidl, 0, nullptr))) {
                continue;
            }
            if (LPCITEMIDLIST childPidl = ILFindChild(dirPidl, itemPidl)) {
                items.push_back(childPidl);
                ownedItems.push_back(itemPidl);
            } else {
                ILFree(itemPidl);
            }
        }

        hr = SHOpenFolderAndSelectItems(dirPidl, static_cast<UINT>(items.size()), items.data(), 0);
        for (const PIDLIST_ABSOLUTE item : ownedItems) {
            ILFree(item);
        }
        ILFree(dirPidl);
        return SUCCEEDED(hr);
    }

    QString DesktopServices::fileManagerName() {
        wchar_t buffer[256];
        HRESULT hr = SHLoadIndirectString(
            L"@shell32.dll,-12352",
            buffer,
            256,
            nullptr
        );
        if (SUCCEEDED(hr)) {
            return QString::fromWCharArray(buffer);
        } else {
            return QStringLiteral("File Explorer");
        }
    }
}
