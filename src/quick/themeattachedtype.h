#ifndef THEMEATTACHEDTYPE_H
#define THEMEATTACHEDTYPE_H

#include <QObject>
#include <SVSCraftQuick/theme.h>

namespace SVS {

    class ThemedItem;

    class ThemeAttachedTypePrivate;

    class ThemeAttachedType : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ThemeAttachedType)
        QML_ANONYMOUS

        Q_PROPERTY(SVS::Theme::ControlType controlType READ controlType WRITE setControlType NOTIFY controlTypeChanged)
    public:
        ~ThemeAttachedType() override;

        Theme::ControlType controlType() const;
        void setControlType(Theme::ControlType value);

    signals:
        void controlTypeChanged(SVS::Theme::ControlType value);

    private:
        friend class ThemedItem;
        explicit ThemeAttachedType(QObject *parent = nullptr);
        QScopedPointer<ThemeAttachedTypePrivate> d_ptr;
    };

}

#endif //THEMEATTACHEDTYPE_H
