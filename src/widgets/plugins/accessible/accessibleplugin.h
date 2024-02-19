#ifndef ACCESSIBLEPLUGIN_H
#define ACCESSIBLEPLUGIN_H

#define QT_STATICPLUGIN

#include <QAccessiblePlugin>

class AccessiblePlugin : public QAccessiblePlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QAccessibleFactoryInterface_iid FILE "plugin.json")
public:
    QAccessibleInterface *create(const QString &key, QObject *object) override;
};

#endif // ACCESSIBLEPLUGIN_H
