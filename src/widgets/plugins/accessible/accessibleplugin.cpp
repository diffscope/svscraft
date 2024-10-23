#include "accessibleplugin.h"

#include <QStaticPlugin>

#include "seekbar/accessibleseekbar.h"
#include "seekbar/accessibleeditlabel.h"

QAccessibleInterface *AccessiblePlugin::create(const QString &key, QObject *object)  {
    if (key == "SVS::SeekBar" && object && object->isWidgetType()) {
        return new SVS::AccessibleSeekBar(qobject_cast<QWidget *>(object));
    } else if (key == "SVS::EditLabel" && object && object->isWidgetType()) {
        return new SVS::AccessibleEditLabel(qobject_cast<QWidget *>(object));
    }
    return nullptr;
}

Q_IMPORT_PLUGIN(AccessiblePlugin)

