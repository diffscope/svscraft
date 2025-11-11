#ifndef SVSCRAFT_FLUENTSYSTEMICONSIMAGEPROVIDER_H
#define SVSCRAFT_FLUENTSYSTEMICONSIMAGEPROVIDER_H

#include <QQuickImageProvider>

#include <SVSCraftFluentSystemIcons/SVSCraftFluentSystemIconsGlobal.h>

namespace SVS {

    class SVSCRAFT_FLUENT_SYSTEM_ICONS_EXPORT FluentSystemIconsImageProvider : public QQuickImageProvider {
        Q_OBJECT
    public:
        explicit FluentSystemIconsImageProvider();
        ~FluentSystemIconsImageProvider() override;

        QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
    };

}

#endif //SVSCRAFT_FLUENTSYSTEMICONSIMAGEPROVIDER_H
