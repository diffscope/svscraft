#include "globalhelper_p.h"

#include <QCursor>

namespace SVS {
    GlobalHelper::GlobalHelper(QObject *parent) : QObject(parent) {
    }
    GlobalHelper::~GlobalHelper() = default;
    QPoint GlobalHelper::cursorPos() {
        return QCursor::pos();
    }
}

#include "moc_globalhelper_p.cpp"