#ifndef THEMEDITEM_P_H
#define THEMEDITEM_P_H

#include <QObject>
#include <SVSCraftQuick/theme.h>

namespace SVS {

    class ThemedItemAttachedType;

    class ThemedItemPrivate;

    class ThemedItem : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ThemedItem)
        QML_ANONYMOUS

        Q_PROPERTY(SVS::Theme::ControlType controlType READ controlType WRITE setControlType NOTIFY controlTypeChanged)
        Q_PROPERTY(SVS::Theme::BackgroundLevel backgroundLevel READ backgroundLevel WRITE setBackgroundLevel NOTIFY backgroundLevelChanged)
        Q_PROPERTY(SVS::Theme::ForegroundLevel foregroundLevel READ foregroundLevel WRITE setForegroundLevel NOTIFY foregroundLevelChanged)
        Q_PROPERTY(bool foldable READ foldable WRITE setFoldable NOTIFY foldableChanged)
        Q_PROPERTY(bool folded READ folded WRITE setFolded NOTIFY foldedChanged)
        Q_PROPERTY(double trackStart READ trackStart WRITE setTrackStart NOTIFY trackStartChanged)
        Q_PROPERTY(bool flat READ flat WRITE setFlat NOTIFY flatChanged)
        Q_PROPERTY(SVS::Theme::TabIndicator tabIndicator READ tabIndicator WRITE setTabIndicator NOTIFY tabIndicatorChanged)
        Q_PROPERTY(SVS::Theme::DividerStroke dividerStroke READ dividerStroke WRITE setDividerStroke NOTIFY dividerStrokeChanged)
    public:
        ~ThemedItem() override;

        Theme::ControlType controlType() const;
        void setControlType(Theme::ControlType value);

        SVS::Theme::BackgroundLevel backgroundLevel() const;
        void setBackgroundLevel(SVS::Theme::BackgroundLevel value);

        SVS::Theme::ForegroundLevel foregroundLevel() const;
        void setForegroundLevel(SVS::Theme::ForegroundLevel value);

        bool foldable() const;
        void setFoldable(bool value);

        bool folded() const;
        void setFolded(bool value);

        double trackStart() const;
        void setTrackStart(double value);

        bool flat() const;
        void setFlat(bool value);

        Theme::TabIndicator tabIndicator() const;
        void setTabIndicator(Theme::TabIndicator value);

        Theme::DividerStroke dividerStroke() const;
        void setDividerStroke(Theme::DividerStroke value);

    signals:
        void controlTypeChanged();
        void backgroundLevelChanged();
        void foregroundLevelChanged();
        void foldableChanged();
        void foldedChanged();
        void trackStartChanged();
        void flatChanged();
        void tabIndicatorChanged();
        void dividerStrokeChanged();

    private:
        friend class ThemedItemAttachedType;
        explicit ThemedItem(QObject *parent = nullptr);
        QScopedPointer<ThemedItemPrivate> d_ptr;
    };

}

#endif //THEMEDITEM_P_H
