#include "FluentSystemIcons.h"

#include <QDir>
#include <QFileInfo>
#include <QFontDatabase>
#include <QHash>
#include <QImageReader>
#include <QPainter>
#include <QPixmap>
#include <QGuiApplication>
#include <QLoggingCategory>

#include <SVSCraftFluentSystemIcons/private/iconmanifest_p.h>

namespace SVS {

    Q_LOGGING_CATEGORY(lcFluentSystemIcons, "svscraft.fluentsystemicons")

    QHash<QString, QPixmap> m_cache;

    static int normalizeSize(int size) {
        constexpr int normalSizeList[] = {
            48, 32, 28, 24, 20, 16, 12, 10
        };
        for (auto s : normalSizeList) {
            if (size >= s)
                return s;
        }
        return 10;
    }

    static int filledFontId = -1;
    static int regularFontId = -1;

    static QPixmap paintPixmapFromFont(int ch, int size, FluentSystemIcons::Style style) {
        if (filledFontId == -1) {
            filledFontId = QFontDatabase::addApplicationFont(":/svscraft/fluent-system-icons/FluentSystemIcons-Filled.ttf");
            regularFontId = QFontDatabase::addApplicationFont(":/svscraft/fluent-system-icons/FluentSystemIcons-Regular.ttf");
        }
        if (filledFontId == -1 || regularFontId == -1) {
            qCWarning(lcFluentSystemIcons) << "Failed to load Fluent System Icons fonts";
            return {};
        }
        QFont font(QFontDatabase::applicationFontFamilies(style == FluentSystemIcons::Regular ? regularFontId : filledFontId));
        font.setPixelSize(size);
        QPixmap pixmap(size, size);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setPen(Qt::black);
        painter.setFont(font);
        QStringView s = QChar::fromUcs4(ch);
        painter.drawText(QRect(0, 0, size, size), Qt::AlignCenter, s.toString());
        return pixmap;
    }

    static QDir customIconDir(":/svscraft/fluent-system-icons/icons");

    static QSet<QString> loadCustomIconList() {
        QSet<QString> s;
        for (const auto &fileInfo : customIconDir.entryInfoList(QDir::Files)) {
            s.insert(fileInfo.baseName());
        }
        return s;
    }

    static QPixmap loadPixmapFromCustomIcon(const QString &name, int size) {
        QImageReader reader(customIconDir.filePath(name + ".svg"));
        reader.setScaledSize(QSize(size, size));
        return QPixmap::fromImageReader(&reader);
    }

    QPixmap FluentSystemIcons::getIcon(const QString &name, Direction direction, int size, Style style) {
        static const auto ltrString = QStringLiteral("_ltr");
        static const auto rtlString = QStringLiteral("_rtl");
        static const QStringList ltrTrySteps {ltrString, {}, rtlString};
        static const QStringList rtlTrySteps {rtlString, {}, ltrString};
        static const QStringList autoLtrTrySteps {{}, ltrString, rtlString};
        static const QStringList autoRtlTrySteps {{}, rtlString, ltrString};
        static const QStringList trySizes10 = {"_10", "_12", "_16", "_20", "_24", "_28", "_32", "_48"};
        static const QStringList trySizes12 = {"_12", "_16", "_20", "_24", "_28", "_32", "_48", "_10"};
        static const QStringList trySizes16 = {"_16", "_20", "_24", "_28", "_32", "_48", "_12", "_10"};
        static const QStringList trySizes20 = {"_20", "_24", "_28", "_32", "_48", "_16", "_12", "_10"};
        static const QStringList trySizes24 = {"_24", "_28", "_32", "_48", "_16", "_20", "_12", "_10"};
        static const QStringList trySizes28 = {"_28", "_32", "_48", "_16", "_20", "_24", "_12", "_10"};
        static const QStringList trySizes32 = {"_32", "_48", "_16", "_20", "_24", "_28", "_12", "_10"};
        static const QStringList trySizes48 = {"_48", "_32", "_28", "_24", "_20", "_16", "_12", "_10"};
        static const QSet<QString> customIconList = loadCustomIconList();

        auto normalizedSize = normalizeSize(size);
        auto cacheKey = QStringList{name, QString::number(direction), QString::number(normalizedSize), QString::number(style), QString::number(QGuiApplication::layoutDirection())}.join(",");
        if (m_cache.contains(cacheKey))
            return m_cache[cacheKey];

        const auto &tryDirections = direction == Ltr ? ltrTrySteps : direction == Rtl ? rtlTrySteps : QGuiApplication::layoutDirection() == Qt::LeftToRight ? autoLtrTrySteps : autoRtlTrySteps;
        auto trySizes = [normalizedSize] {
            switch (normalizedSize) {
                case 10: return trySizes10;
                case 12: return trySizes12;
                case 16: return trySizes16;
                case 20: return trySizes20;
                case 24: return trySizes24;
                case 28: return trySizes28;
                case 32: return trySizes32;
                case 48: return trySizes48;
                default: return trySizes16;
            }
        }();
        QStringList tryStyles {
            style == Regular ? "_regular" : "_filled",
            style == Regular ? "_filled" : "_regular"
        };
        for (const auto &trySize : trySizes) {
            for (const auto &tryDirection : tryDirections) {
                for (const auto &tryStyle : tryStyles) {
                    const auto &charset = tryStyle.at(1) == 'r' ? fluentSystemIcons_RegularIconsCharset : fluentSystemIcons_FilledIconsCharset;
                    auto key = QStringList{QStringLiteral("ic_fluent_"), name, tryDirection, trySize, tryStyle}.join("");
                    QPixmap pixmap;
                    if (customIconList.contains(key)) {
                        qCDebug(lcFluentSystemIcons) << "Found custom icon" << key;
                        pixmap = loadPixmapFromCustomIcon(key, size);
                    } else if (auto ch = charset.value(key)) {
                        qCDebug(lcFluentSystemIcons) << "Found icon" << key;
                        auto foundStyle = tryStyle.at(1) == 'r' ? Regular : Filled;
                        pixmap = paintPixmapFromFont(ch, size, foundStyle);
                    } else {
                        continue;
                    }
                    if ((direction == Rtl || direction == Auto && QGuiApplication::layoutDirection() == Qt::RightToLeft) && fluentSystemIcons_MirrorRtlIcons.contains(key)) {
                        pixmap = pixmap.transformed(QTransform().scale(-1, 1));
                    }
                    m_cache.insert(cacheKey, pixmap);
                    return pixmap;
                }
            }
        }
        qCWarning(lcFluentSystemIcons) << "Failed to find icon" << name << direction << size << style;
        m_cache.insert(cacheKey, {});
        return {};
    }
}