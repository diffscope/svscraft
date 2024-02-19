#include "accessibleplugin.h"
#include "seekbar/accessibleseekbar.h"

#include <QStaticPlugin>

QAccessibleInterface *AccessiblePlugin::create(const QString &key, QObject *object)  {
    if (key == "SVS::SeekBar" && object && object->isWidgetType()) {
        return new SVS::AccessibleSeekBar(qobject_cast<QWidget *>(object));
    }
    return nullptr;
}

Q_IMPORT_PLUGIN(AccessiblePlugin)

