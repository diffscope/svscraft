#include "themeattachedtype.h"
#include "themeattachedtype_p.h"

namespace SVS {
    ThemeAttachedType::ThemeAttachedType(QObject *parent) : QObject(parent), d_ptr(new ThemeAttachedTypePrivate) {
        Q_D(ThemeAttachedType);
        d->q_ptr = this;
    }
    ThemeAttachedType::~ThemeAttachedType() = default;
    Theme::ControlType ThemeAttachedType::controlType() const {
        Q_D(const ThemeAttachedType);
        return d->controlType;
    }
    void ThemeAttachedType::setControlType(Theme::ControlType value) {
        Q_D(ThemeAttachedType);
        if (d->controlType != value) {
            d->controlType = value;
            emit controlTypeChanged(value);
        }
    }
    SVS::Theme::BackgroundLevel ThemeAttachedType::backgroundLevel() const {
        Q_D(const ThemeAttachedType);
        return d->backgroundLevel;
    }
    void ThemeAttachedType::setBackgroundLevel(SVS::Theme::BackgroundLevel value) {
        Q_D(ThemeAttachedType);
        if (d->backgroundLevel != value) {
            d->backgroundLevel = value;
            emit backgroundLevelChanged(d->backgroundLevel);
        }
    }
    bool ThemeAttachedType::foldable() const {
        Q_D(const ThemeAttachedType);
        return d->foldable;
    }
    void ThemeAttachedType::setFoldable(bool value) {
        Q_D(ThemeAttachedType);
        if (d->foldable != value) {
            d->foldable = value;
            emit foldableChanged(value);
        }
    }
    bool ThemeAttachedType::folded() const {
        Q_D(const ThemeAttachedType);
        return d->folded;
    }
    void ThemeAttachedType::setFolded(bool value) {
        Q_D(ThemeAttachedType);
        if (d->folded != value) {
            d->folded = value;
            emit foldedChanged(value);
        }
    }
    double ThemeAttachedType::trackStart() const {
        Q_D(const ThemeAttachedType);
        return d->trackStart;
    }
    void ThemeAttachedType::setTrackStart(double value) {
        Q_D(ThemeAttachedType);
        if (d->trackStart != value) {
            d->trackStart = value;
            emit trackStartChanged(value);
        }
    }
    bool ThemeAttachedType::flat() const {
        Q_D(const ThemeAttachedType);
        return d->flat;
    }
    void ThemeAttachedType::setFlat(bool value) {
        Q_D(ThemeAttachedType);
        if (d->flat != value) {
            d->flat = value;
            emit flatChanged(value);
        }
    }
    Theme::TabIndicator ThemeAttachedType::tabIndicator() const {
        Q_D(const ThemeAttachedType);
        return d->tabIndicator;
    }
    void ThemeAttachedType::setTabIndicator(Theme::TabIndicator value) {
        Q_D(ThemeAttachedType);
        if (d->tabIndicator != value) {
            d->tabIndicator = value;
            emit tabIndicatorChanged(value);
        }
    }
    Theme::DividerStroke ThemeAttachedType::dividerStroke() const {
        Q_D(const ThemeAttachedType);
        return d->dividerStroke;
    }
    void ThemeAttachedType::setDividerStroke(Theme::DividerStroke value) {
        Q_D(ThemeAttachedType);
        if (d->dividerStroke != value) {
            d->dividerStroke = value;
            emit dividerStrokeChanged(value);
        }
    }
}