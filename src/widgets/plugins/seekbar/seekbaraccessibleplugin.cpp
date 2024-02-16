#include "seekbaraccessibleplugin.h"
#include "accessibleseekbar.h"

#include <QStaticPlugin>

QAccessibleInterface *SeekBarAccessiblePlugin::create(const QString &key, QObject *object)  {
    if (key == "SVS::SeekBar" && object && object->isWidgetType()) {
        return new SVS::AccessibleSeekBar(qobject_cast<QWidget *>(object));
    }
    return nullptr;
}

Q_IMPORT_PLUGIN(SeekBarAccessiblePlugin)

