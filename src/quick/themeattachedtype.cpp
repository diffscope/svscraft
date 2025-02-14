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
}