#ifndef SVSCRAFT_COMMANDPALETTEHELPER_P_H
#define SVSCRAFT_COMMANDPALETTEHELPER_P_H

#include <QObject>
#include <qqmlintegration.h>

class QAbstractItemModel;

namespace SVS {

    class CommandPaletteHelper : public QObject {
        Q_OBJECT
        QML_ELEMENT
        QML_SINGLETON
    public:
        explicit CommandPaletteHelper(QObject *parent = nullptr);
        ~CommandPaletteHelper() override;

        Q_INVOKABLE static QAbstractItemModel *createFilterModel(QAbstractItemModel *model, QObject *popup);
        Q_INVOKABLE static QString highlightString(const QString &s, const QString &t, const QColor &color);
    };
}

#endif //COMMANDPALETTEHELPER_P_H
