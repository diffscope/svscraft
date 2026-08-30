#define QT_NO_KEYWORDS

#include "DesktopServices.h"

#include <QDir>
#include <QLocale>

#include <Cocoa/Cocoa.h>

namespace {

    // Parses a property list file, tolerating binary plists (e.g. .loctable) and
    // UTF-16 encoded text in addition to the plain OpenStep .strings format.
    NSDictionary<NSString *, id> *dictionaryWithPropertyListFileContents(NSURL *url) {
        NSData *data = [NSData dataWithContentsOfURL:url];
        if (!data) {
            return nil;
        }
        id object = [NSPropertyListSerialization propertyListWithData:data options:0 format:nil
                                                                error:nil];
        if ([object isKindOfClass:[NSDictionary class]]) {
            return object;
        }
        for (NSNumber *encoding in @[ @(NSUTF16StringEncoding), @(NSUTF8StringEncoding) ]) {
            NSString *text = [[NSString alloc] initWithData:data
                                                   encoding:encoding.unsignedIntegerValue];
            object = [text propertyList];
            if ([object isKindOfClass:[NSDictionary class]]) {
                return object;
            }
        }
        return nil;
    }

    // Reads Finder.app and returns its display name in each available localization,
    // keyed by localization identifier (e.g. "en", "zh_CN").
    NSDictionary<NSString *, NSString *> *finderLocalizedNames(NSString **genericName) {
        NSURL *finderURL = [[NSWorkspace sharedWorkspace]
            URLForApplicationWithBundleIdentifier:@"com.apple.finder"];
        if (!finderURL) {
            finderURL =
                [NSURL fileURLWithPath:@"/System/Library/CoreServices/Finder.app" isDirectory:YES];
        }
        NSBundle *finderBundle = [NSBundle bundleWithURL:finderURL];
        if (!finderBundle) {
            return nil;
        }
        *genericName = [finderBundle objectForInfoDictionaryKey:@"CFBundleName"];
        NSMutableDictionary<NSString *, NSString *> *names = [NSMutableDictionary dictionary];

        // Current macOS merges the localized Info.plist entries into a single binary
        // InfoPlist.loctable structured as { localizationID: { key: value } }.
        NSURL *locTableURL = [finderBundle URLForResource:@"InfoPlist" withExtension:@"loctable"];
        NSDictionary<NSString *, id> *locTable =
            locTableURL ? dictionaryWithPropertyListFileContents(locTableURL) : nil;
        for (NSString *localization in locTable) {
            NSDictionary<NSString *, NSString *> *infoStrings = locTable[localization];
            if (![infoStrings isKindOfClass:[NSDictionary class]]) {
                continue;
            }
            NSString *name = infoStrings[@"CFBundleDisplayName"] ?: infoStrings[@"CFBundleName"];
            if (name.length) {
                names[localization] = name;
            }
        }

        // Older macOS ships one InfoPlist.strings per localization directory.
        for (NSString *localization in finderBundle.localizations) {
            if (names[localization]) {
                continue;
            }
            NSURL *stringsURL = [finderBundle URLForResource:@"InfoPlist" withExtension:@"strings"
                                                 subdirectory:nil localization:localization];
            NSDictionary<NSString *, NSString *> *infoStrings =
                stringsURL ? dictionaryWithPropertyListFileContents(stringsURL) : nil;
            NSString *name = infoStrings[@"CFBundleDisplayName"] ?: infoStrings[@"CFBundleName"];
            if (name.length) {
                names[localization] = name;
            } else if ((*genericName).length) {
                names[localization] = *genericName;
            }
        }
        return names;
    }

}

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
        static QString name;
        if (!name.isEmpty()) {
            return name;
        }
        const QString bcp47Name = QLocale().bcp47Name();
        @autoreleasepool {
            NSString *genericName = nil;
            NSDictionary<NSString *, NSString *> *localizedNames = finderLocalizedNames(&genericName);
            NSArray<NSString *> *matchedLocalizations = [NSBundle
                preferredLocalizationsFromArray:localizedNames.allKeys
                                 forPreferences:@[ bcp47Name.toNSString() ]];
            if (matchedLocalizations.count) {
                name = QString::fromNSString(localizedNames[matchedLocalizations.firstObject]);
            }
            if (name.isEmpty() && genericName.length) {
                name = QString::fromNSString(genericName);
            }
            if (name.isEmpty()) {
                name = QStringLiteral("Finder");
            }
        }
        return name;
    }
}
