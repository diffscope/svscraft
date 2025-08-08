// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef SVSCRAFT_QML_EXTRAS_QQMLSETTINGS_P_H
#define SVSCRAFT_QML_EXTRAS_QQMLSETTINGS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>
#include <QtCore/qsettings.h>
#include <QtCore/qurl.h>
#include <QtQml/qqml.h>
#include <QtQml/qqmlparserstatus.h>

namespace SVS {

    class QQmlSettingsPrivate;

    class QQmlSettings : public QObject, public QQmlParserStatus
    {
        Q_OBJECT
        Q_INTERFACES(QQmlParserStatus)
        Q_DECLARE_PRIVATE(QQmlSettings)
        QML_NAMED_ELEMENT(Settings)
        QML_ADDED_IN_VERSION(6, 5)

        Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY categoryChanged FINAL)
        Q_PROPERTY(QSettings *settings READ settings WRITE setSettings NOTIFY settingsChanged FINAL)

    public:
        explicit QQmlSettings(QObject *parent = nullptr);
        ~QQmlSettings() override;

        QString category() const;
        void setCategory(const QString &category);

        QSettings *settings() const;
        void setSettings(QSettings *settings);

        Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue = {}) const;
        Q_INVOKABLE void setValue(const QString &key, const QVariant &value);
        Q_INVOKABLE void sync();

        Q_SIGNALS:
            void categoryChanged(const QString &arg);
            void settingsChanged(QSettings *settings);

    protected:
        void timerEvent(QTimerEvent *event) override;

        void classBegin() override;
        void componentComplete() override;

    private:
        QScopedPointer<QQmlSettingsPrivate> d_ptr;

        Q_PRIVATE_SLOT(d_func(), void _q_propertyChanged())
    };
}

#endif // SVSCRAFT_QML_EXTRAS_QQMLSETTINGS_P_H
