#ifndef SEEKBARACCESSIBLEPLUGIN_H
#define SEEKBARACCESSIBLEPLUGIN_H

#define QT_STATICPLUGIN

#include <QAccessiblePlugin>

class SeekBarAccessiblePlugin : public QAccessiblePlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QAccessibleFactoryInterface_iid FILE "plugin.json")
public:
    QAccessibleInterface *create(const QString &key, QObject *object) override;
};

#endif // SEEKBARACCESSIBLEPLUGIN_H
