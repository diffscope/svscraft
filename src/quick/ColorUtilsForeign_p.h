#ifndef SVSCRAFT_COLORUTILSFOREIGN_P_H
#define SVSCRAFT_COLORUTILSFOREIGN_P_H

#include <QObject>
#include <qqmlintegration.h>

namespace SVS {

    class ColorUtilsForeign : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(ColorUtils)
        QML_SINGLETON

    public:
        explicit ColorUtilsForeign(QObject *parent = nullptr);
        ~ColorUtilsForeign() override;

        Q_INVOKABLE static QColor selectHighestContrastColor(const QColor &baseColor,
                                                             const QList<QColor> &availableColors);
    };

}

#endif //SVSCRAFT_COLORUTILSFOREIGN_P_H
