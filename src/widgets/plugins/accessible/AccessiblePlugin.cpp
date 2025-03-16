#include "AccessiblePlugin.h"

#include <QStaticPlugin>

#include "seekbar/AccessibleSeekBar.h"
#include "seekbar/AccessibleEditLabel.h"

QAccessibleInterface *AccessiblePlugin::create(const QString &key, QObject *object)  {
    if (key == "SVS::SeekBar" && object && object->isWidgetType()) {
        return new SVS::AccessibleSeekBar(qobject_cast<QWidget *>(object));
    } else if (key == "SVS::EditLabel" && object && object->isWidgetType()) {
        return new SVS::AccessibleEditLabel(qobject_cast<QWidget *>(object));
    }
    return nullptr;
}

Q_IMPORT_PLUGIN(AccessiblePlugin)

