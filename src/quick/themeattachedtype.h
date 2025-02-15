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
        Q_PROPERTY(SVS::Theme::BackgroundLevel backgroundLevel READ backgroundLevel WRITE setBackgroundLevel NOTIFY backgroundLevelChanged)
        Q_PROPERTY(bool foldable READ foldable WRITE setFoldable NOTIFY foldableChanged)
        Q_PROPERTY(bool folded READ folded WRITE setFolded NOTIFY foldedChanged)
        Q_PROPERTY(double trackStart READ trackStart WRITE setTrackStart NOTIFY trackStartChanged)
    public:
        ~ThemeAttachedType() override;

        Theme::ControlType controlType() const;
        void setControlType(Theme::ControlType value);

        SVS::Theme::BackgroundLevel backgroundLevel() const;
        void setBackgroundLevel(SVS::Theme::BackgroundLevel value);

        bool foldable() const;
        void setFoldable(bool value);

        bool folded() const;
        void setFolded(bool value);

        double trackStart() const;
        void setTrackStart(double value);

    signals:
        void controlTypeChanged(SVS::Theme::ControlType value);
        void backgroundLevelChanged(SVS::Theme::BackgroundLevel value);
        void foldableChanged(bool value);
        void foldedChanged(bool value);
        void trackStartChanged(double value);

    private:
        friend class ThemedItem;
        explicit ThemeAttachedType(QObject *parent = nullptr);
        QScopedPointer<ThemeAttachedTypePrivate> d_ptr;
    };

}

#endif //THEMEATTACHEDTYPE_H
