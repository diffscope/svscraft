/******************************************************************************
 * Copyright (c) 2025 OpenVPI                                                 *
 *                                                                            *
 * This file is part of SVSCraft                                              *
 *                                                                            *
 * SVSCraft is free software: you can redistribute it and/or modify it under  *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License, or (at your     *
 * option) any later version.                                                 *
 *                                                                            *
 * SVSCraft is distributed in the hope that it will be useful, but WITHOUT    *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public        *
 * License for more details.                                                  *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with SVSCraft. If not, see <https://www.gnu.org/licenses/>.          *
 ******************************************************************************/

#ifndef SVSCRAFT_DOWNLOADSESSION_H
#define SVSCRAFT_DOWNLOADSESSION_H

#include <QObject>
#include <QUrl>
#include <QNetworkReply>

#include <SVSCraftNetwork/SVSCraftNetworkGlobal.h>

class QIODevice;
class QNetworkAccessManager;

namespace SVS {

    class DownloadSessionPrivate;

    class SVSCRAFT_NETWORK_EXPORT DownloadSession : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DownloadSession)

        Q_PROPERTY(Status status READ status NOTIFY statusChanged)
        Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
        Q_PROPERTY(QString userAgent READ userAgent WRITE setUserAgent NOTIFY userAgentChanged)
        Q_PROPERTY(QIODevice* device READ device WRITE setDevice NOTIFY deviceChanged)
        Q_PROPERTY(QNetworkAccessManager* networkAccessManager READ networkAccessManager WRITE setNetworkAccessManager NOTIFY networkAccessManagerChanged)
        Q_PROPERTY(qint64 bytesReceived READ bytesReceived NOTIFY progressChanged)
        Q_PROPERTY(qint64 bytesTotal READ bytesTotal NOTIFY progressChanged)
        Q_PROPERTY(double speed READ speed NOTIFY speedChanged)
        Q_PROPERTY(qint64 elapsedTime READ elapsedTime NOTIFY elapsedTimeChanged)
        Q_PROPERTY(qint64 estimatedTime READ estimatedTime NOTIFY estimatedTimeChanged)

    public:
        enum class Status {
            Ready,
            Downloading,
            Paused,
            Completed,
            Cancelled,
            Error
        };
        Q_ENUM(Status)

        explicit DownloadSession(QObject *parent = nullptr);
        explicit DownloadSession(const QUrl &url, QIODevice *device, QObject *parent = nullptr);
        explicit DownloadSession(const QUrl &url, QIODevice *device, QNetworkAccessManager *manager, QObject *parent = nullptr);
        ~DownloadSession() override;

        Status status() const;

        QUrl url() const;
        void setUrl(const QUrl &url);

        QString userAgent() const;
        void setUserAgent(const QString &userAgent);

        QIODevice *device() const;
        void setDevice(QIODevice *device);

        QNetworkAccessManager *networkAccessManager() const;
        void setNetworkAccessManager(QNetworkAccessManager *manager);

        qint64 bytesReceived() const;
        qint64 bytesTotal() const;
        double speed() const;
        qint64 elapsedTime() const;
        qint64 estimatedTime() const;

        bool supportsResume() const;

    public Q_SLOTS:
        void start();
        void pause();
        void resume();
        void cancel();
        void reset();

    Q_SIGNALS:
        void statusChanged(Status status);
        void urlChanged(const QUrl &url);
        void userAgentChanged(const QString &userAgent);
        void deviceChanged(QIODevice *device);
        void networkAccessManagerChanged(QNetworkAccessManager *networkAccessManager);
        void progressChanged(qint64 bytesReceived, qint64 bytesTotal);
        void speedChanged(double bytesPerSecond);
        void elapsedTimeChanged(qint64 elapsedTime);
        void estimatedTimeChanged(qint64 estimatedTime);
        void supportsResumeChanged(bool supportsResume);
        void networkErrorOccurred(QNetworkReply::NetworkError code, const QString &description);
        void writeErrorOccurred(const QString &description);
        void finished();

    private:
        QScopedPointer<DownloadSessionPrivate> d_ptr;
    };

}

#endif // SVSCRAFT_DOWNLOADSESSION_H
