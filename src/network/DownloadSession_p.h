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

#ifndef SVSCRAFT_DOWNLOADSESSION_P_H
#define SVSCRAFT_DOWNLOADSESSION_P_H

#include <QElapsedTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QUrl>
#include <QPointer>

#include <SVSCraftNetwork/DownloadSession.h>

class QIODevice;

namespace SVS {

    class DownloadSessionPrivate {
        Q_DECLARE_PUBLIC(DownloadSession)
    public:
        DownloadSession *q_ptr;
        
        // Core properties
        QUrl url;
        QString userAgent = QStringLiteral("SVSCraft/1.0");
        QIODevice *device = nullptr;
        QNetworkAccessManager *networkManager = nullptr;
        bool ownsNetworkManager = false;
        
        // Download state
        DownloadSession::Status status = DownloadSession::Status::Ready;
        QPointer<QNetworkReply> reply;
        qint64 bytesReceived = 0;
        qint64 bytesTotal = -1;
        qint64 startByte = 0; // For resume functionality
        
        // Progress tracking
        QElapsedTimer downloadTimer;
        QTimer *speedUpdateTimer = nullptr;
        double currentSpeed = 0.0;
        qint64 lastBytesReceived = 0;
        qint64 lastSpeedUpdate = 0;
        
        // Resume support metadata
        QString lastModified;
        QString etag;
        bool supportsResume = false;
        
        void init();
        void cleanup();
        void setStatus(DownloadSession::Status newStatus);
        void updateProgress(qint64 received, qint64 total);
        void updateSpeed();
        void startDownloadInternal(qint64 startFromByte = 0);
        void abortDownloadInternal();
        void handleMetaDataChanged();
        void handleReplyFinished();
        void handleReplyError(QNetworkReply::NetworkError error);
        void handleReadyRead();
        bool checkFileConsistency(QNetworkReply *reply);
        bool verifyFileConsistency(QNetworkReply *reply);
        void saveResumeMetadata(QNetworkReply *reply);
        QNetworkRequest createRequest(qint64 startFromByte = 0);
    };

}

#endif // SVSCRAFT_DOWNLOADSESSION_P_H
