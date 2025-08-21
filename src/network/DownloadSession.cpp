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

#include "DownloadSession.h"
#include "DownloadSession_p.h"

#include <QIODevice>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QHttpHeaders>

namespace SVS {

    DownloadSession::DownloadSession(QObject *parent)
        : QObject(parent), d_ptr(new DownloadSessionPrivate) {
        Q_D(DownloadSession);
        d->q_ptr = this;
        d->init();
    }

    DownloadSession::DownloadSession(const QUrl &url, QIODevice *device, QObject *parent)
        : QObject(parent), d_ptr(new DownloadSessionPrivate) {
        Q_D(DownloadSession);
        d->q_ptr = this;
        d->url = url;
        d->device = device;
        d->init();
    }

    DownloadSession::DownloadSession(const QUrl &url, QIODevice *device, QNetworkAccessManager *manager, QObject *parent)
        : QObject(parent), d_ptr(new DownloadSessionPrivate) {
        Q_D(DownloadSession);
        d->q_ptr = this;
        d->url = url;
        d->device = device;
        d->networkManager = manager;
        d->ownsNetworkManager = false;
        d->init();
    }

    DownloadSession::~DownloadSession() {
        Q_D(DownloadSession);
        d->cleanup();
    }

    DownloadSession::Status DownloadSession::status() const {
        Q_D(const DownloadSession);
        return d->status;
    }

    QUrl DownloadSession::url() const {
        Q_D(const DownloadSession);
        return d->url;
    }

    void DownloadSession::setUrl(const QUrl &url) {
        Q_D(DownloadSession);
        if (d->url != url) {
            d->url = url;
            Q_EMIT urlChanged(url);
        }
    }

    QString DownloadSession::userAgent() const {
        Q_D(const DownloadSession);
        return d->userAgent;
    }

    void DownloadSession::setUserAgent(const QString &userAgent) {
        Q_D(DownloadSession);
        if (d->userAgent != userAgent) {
            d->userAgent = userAgent;
            Q_EMIT userAgentChanged(userAgent);
        }
    }

    QIODevice *DownloadSession::device() const {
        Q_D(const DownloadSession);
        return d->device;
    }

    void DownloadSession::setDevice(QIODevice *device) {
        Q_D(DownloadSession);
        if (d->device != device) {
            d->device = device;
            emit deviceChanged(device);
        }
    }

    QNetworkAccessManager *DownloadSession::networkAccessManager() const {
        Q_D(const DownloadSession);
        return d->networkManager;
    }

    void DownloadSession::setNetworkAccessManager(QNetworkAccessManager *manager) {
        Q_D(DownloadSession);
        if (d->networkManager != manager) {
            if (d->ownsNetworkManager && d->networkManager) {
                delete d->networkManager;
            }
            d->networkManager = manager;
            d->ownsNetworkManager = false;
            emit networkAccessManagerChanged(manager);
        }
    }

    qint64 DownloadSession::bytesReceived() const {
        Q_D(const DownloadSession);
        return d->bytesReceived;
    }

    qint64 DownloadSession::bytesTotal() const {
        Q_D(const DownloadSession);
        return d->bytesTotal;
    }

    double DownloadSession::speed() const {
        Q_D(const DownloadSession);
        return d->currentSpeed;
    }

    qint64 DownloadSession::elapsedTime() const {
        Q_D(const DownloadSession);
        if (d->downloadTimer.isValid()) {
            return d->downloadTimer.elapsed();
        }
        return 0;
    }

    qint64 DownloadSession::estimatedTime() const {
        Q_D(const DownloadSession);
        if (d->bytesTotal > 0) {
            qint64 remainingBytes = d->bytesTotal - d->bytesReceived;
            if (remainingBytes <= 0) {
                return 0; // Download completed or over-completed
            }
            if (d->currentSpeed > 0) {
                return static_cast<qint64>(remainingBytes / d->currentSpeed * 1000); // Convert to milliseconds
            }
        }
        return -1; // Unknown
    }

    bool DownloadSession::supportsResume() const {
        Q_D(const DownloadSession);
        return d->supportsResume;
    }

    void DownloadSession::start() {
        Q_D(DownloadSession);
        if (d->status != Status::Ready) {
            return; // Ignore invalid operation
        }
        
        if (!d->url.isValid() || !d->device) {
            d->setStatus(Status::Error);
            Q_EMIT writeErrorOccurred(QStringLiteral("Invalid URL or device"));
            return;
        }
        
        d->startDownloadInternal();
    }

    void DownloadSession::pause() {
        Q_D(DownloadSession);
        if (d->status != Status::Downloading) {
            return; // Ignore invalid operation
        }
        
        // Don't pause if resume is not supported
        if (!d->supportsResume) {
            return; // Ignore pause operation when resume is not supported
        }
        d->abortDownloadInternal();
        d->speedUpdateTimer->stop();
        
        // Clear speed-related internal state
        d->currentSpeed = 0.0;
        d->lastBytesReceived = d->bytesReceived; // Keep current position for resume
        d->lastSpeedUpdate = 0;
        
        d->setStatus(Status::Paused);
        
        // Clear speed and estimated time display when paused
        Q_EMIT speedChanged(0.0);
        Q_EMIT estimatedTimeChanged(-1);
    }

    void DownloadSession::resume() {
        Q_D(DownloadSession);
        if (d->status != Status::Paused) {
            return; // Ignore invalid operation
        }
        
        // Ensure device is still valid and seekable
        if (!d->device) {
            d->setStatus(Status::Error);
            Q_EMIT writeErrorOccurred(QStringLiteral("Device is null"));
            return;
        }
        
        // Try to seek to the position where we left off
        if (d->device->isOpen() && d->device->isSequential() == false) {
            if (!d->device->seek(d->bytesReceived)) {
                d->setStatus(Status::Error);
                Q_EMIT writeErrorOccurred(QStringLiteral("Failed to seek device to resume position"));
                return;
            }
        }
        
        d->startDownloadInternal(d->bytesReceived);
    }

    void DownloadSession::cancel() {
        Q_D(DownloadSession);
        if (d->status != Status::Downloading && d->status != Status::Paused) {
            return; // Ignore invalid operation
        }
        
        d->abortDownloadInternal();
        d->speedUpdateTimer->stop();
        
        // Clear speed-related internal state
        d->currentSpeed = 0.0;
        d->lastBytesReceived = 0;
        d->lastSpeedUpdate = 0;
        
        d->setStatus(Status::Cancelled);
        
        // Emit progress changed to reset UI progress display
        Q_EMIT progressChanged(0, -1);
        Q_EMIT speedChanged(0.0);
        Q_EMIT elapsedTimeChanged(0);
        Q_EMIT estimatedTimeChanged(-1);
    }

    void DownloadSession::reset() {
        Q_D(DownloadSession);
        
        // Cancel any ongoing download
        if (d->status == Status::Downloading || d->status == Status::Paused) {
            d->abortDownloadInternal();
        }
        
        // Reset state
        d->bytesReceived = 0;
        d->bytesTotal = -1;
        d->startByte = 0;
        d->currentSpeed = 0.0;
        d->lastBytesReceived = 0;
        d->lastSpeedUpdate = 0;
        d->lastModified.clear();
        d->etag.clear();
        
        // Reset resume support and emit signal if changed
        if (d->supportsResume) {
            d->supportsResume = false;
            Q_EMIT supportsResumeChanged(false);
        }
        
        d->downloadTimer.invalidate();
        d->speedUpdateTimer->stop();
        
        // Reset device position if possible
        if (d->device && d->device->isOpen()) {
            d->device->seek(0);
        }
        
        d->setStatus(Status::Ready);
        
        // Emit progress changed to reset UI progress display
        Q_EMIT progressChanged(0, -1);
        Q_EMIT speedChanged(0.0);
        Q_EMIT elapsedTimeChanged(0);
        Q_EMIT estimatedTimeChanged(-1);
    }

    // DownloadSessionPrivate implementation

    void DownloadSessionPrivate::init() {
        Q_Q(DownloadSession);
        
        // Create network manager if not provided
        if (!networkManager) {
            networkManager = new QNetworkAccessManager(q);
            ownsNetworkManager = true;
        }
        
        // Create speed update timer
        speedUpdateTimer = new QTimer(q);
        speedUpdateTimer->setInterval(1000); // Update every second
        QObject::connect(speedUpdateTimer, &QTimer::timeout, q, [this]() {
            updateSpeed();
        });
    }

    void DownloadSessionPrivate::cleanup() {
        abortDownloadInternal();
        
        if (ownsNetworkManager && networkManager) {
            delete networkManager;
            networkManager = nullptr;
        }
    }

    void DownloadSessionPrivate::setStatus(DownloadSession::Status newStatus) {
        Q_Q(DownloadSession);
        if (status != newStatus) {
            status = newStatus;
            Q_EMIT q->statusChanged(newStatus);
            
            if (newStatus == DownloadSession::Status::Completed ||
                newStatus == DownloadSession::Status::Cancelled ||
                newStatus == DownloadSession::Status::Error) {
                Q_EMIT q->finished();
            }
        }
    }

    void DownloadSessionPrivate::updateProgress(qint64 received, qint64 total) {
        Q_Q(DownloadSession);
        bool progressChanged = false;
        
        if (bytesReceived != received + startByte) {
            bytesReceived = received + startByte;
            progressChanged = true;
        }
        
        if (bytesTotal != total && total > 0) {
            // For resume requests, total might be the remaining bytes
            // We need to add the start byte to get the actual total
            if (startByte > 0 && total < bytesTotal) {
                bytesTotal = total + startByte;
            } else if (startByte == 0) {
                bytesTotal = total;
            }
            progressChanged = true;
        }
        
        if (progressChanged) {
            Q_EMIT q->progressChanged(bytesReceived, bytesTotal);
            Q_EMIT q->elapsedTimeChanged(q->elapsedTime());
            Q_EMIT q->estimatedTimeChanged(q->estimatedTime());
        }
    }

    void DownloadSessionPrivate::updateSpeed() {
        Q_Q(DownloadSession);
        
        if (!downloadTimer.isValid()) {
            return;
        }
        
        qint64 currentTime = downloadTimer.elapsed();
        qint64 timeDiff = currentTime - lastSpeedUpdate;
        
        if (timeDiff > 0) {
            qint64 bytesDiff = bytesReceived - lastBytesReceived;
            // Calculate average speed over the last second (no smoothing)
            currentSpeed = (bytesDiff * 1000.0) / timeDiff; // bytes per second
            
            lastBytesReceived = bytesReceived;
            lastSpeedUpdate = currentTime;
            
            Q_EMIT q->speedChanged(currentSpeed);
        }
    }

    void DownloadSessionPrivate::startDownloadInternal(qint64 startFromByte) {
        Q_Q(DownloadSession);

        abortDownloadInternal();

        startByte = startFromByte;
        QNetworkRequest request = createRequest(startFromByte);
        reply = networkManager->get(request);

        if (!reply) {
            setStatus(DownloadSession::Status::Error);
            Q_EMIT q->networkErrorOccurred(QNetworkReply::UnknownNetworkError,
                                           "Failed to create network request");
            return;
        }

        // Connect signals
        QObject::connect(reply, &QNetworkReply::metaDataChanged, q,
                         [this]() { handleMetaDataChanged(); });

        QObject::connect(
            reply, &QNetworkReply::downloadProgress, q,
            [this](qint64 received, qint64 total) { updateProgress(received, total); });

        QObject::connect(reply, &QNetworkReply::readyRead, q, [this]() { handleReadyRead(); });

        QObject::connect(reply, &QNetworkReply::finished, q, [this]() { handleReplyFinished(); });

        QObject::connect(reply, &QNetworkReply::errorOccurred, q,
                         [this](QNetworkReply::NetworkError error) { handleReplyError(error); });

        setStatus(DownloadSession::Status::Downloading);
        downloadTimer.start();
        speedUpdateTimer->start();
        lastSpeedUpdate = 0;
        lastBytesReceived = startFromByte;
    }
    void DownloadSessionPrivate::abortDownloadInternal() {
        if (reply) {
            reply->abort();
        }
        if (reply) {
            reply->deleteLater();
            reply = nullptr;
        }
    }

    void DownloadSessionPrivate::handleMetaDataChanged() {
        Q_Q(DownloadSession);
        
        if (!reply) {
            return;
        }
        
        // Save resume metadata as soon as we receive response headers
        saveResumeMetadata(reply);
        
        // Check file consistency for resume requests
        if (startByte > 0 && !checkFileConsistency(reply)) {
            // File has changed, need to restart from beginning
            // Clean up current reply first
            abortDownloadInternal();
            
            // Reset state without calling reset() to avoid accessing null reply
            bytesReceived = 0;
            bytesTotal = -1;
            startByte = 0;
            currentSpeed = 0.0;
            lastBytesReceived = 0;
            lastSpeedUpdate = 0;
            downloadTimer.invalidate();
            speedUpdateTimer->stop();
            
            // Reset device position if possible
            if (device && device->isOpen()) {
                device->seek(0);
            }
            
            // Automatically restart download from beginning
            startDownloadInternal(0);
            return;
        }
    }

    void DownloadSessionPrivate::handleReplyFinished() {
        Q_Q(DownloadSession);
        
        if (!reply) {
            return;
        }
        
        speedUpdateTimer->stop();
        
        // Store the error before doing anything else
        QNetworkReply::NetworkError replyError = reply->error();
        
        if (replyError == QNetworkReply::NoError) {
            // Read any remaining data
            handleReadyRead();
            
            // Clear speed when completed (speed should be 0 when finished)
            currentSpeed = 0.0;
            Q_EMIT q->speedChanged(0.0);
            
            // Estimated time should naturally be 0 since download is complete
            // (remainingBytes = 0, so estimatedTime = 0)
            Q_EMIT q->estimatedTimeChanged(q->estimatedTime());
            
            setStatus(DownloadSession::Status::Completed);
        } else if (replyError == QNetworkReply::OperationCanceledError) {
            // This is expected for pause/cancel operations
            // Status is already set by pause() or cancel()
        } else {
            // Network error already handled by handleReplyError
        }
        
        // Only delete if we still own the reply
        if (reply) {
            reply->deleteLater();
            reply = nullptr;
        }
    }

    void DownloadSessionPrivate::handleReplyError(QNetworkReply::NetworkError error) {
        Q_Q(DownloadSession);
        
        if (error == QNetworkReply::OperationCanceledError) {
            // This is expected for pause/cancel operations
            return;
        }
        setStatus(DownloadSession::Status::Error);
        
        QString errorString = reply ? reply->errorString() : QString("Unknown network error");
        Q_EMIT q->networkErrorOccurred(error, errorString);
    }

    void DownloadSessionPrivate::handleReadyRead() {
        Q_Q(DownloadSession);
        
        if (!reply || !device) {
            return;
        }
        
        QByteArray data = reply->readAll();
        qint64 written = device->write(data);
        
        if (written != data.size()) {
            speedUpdateTimer->stop();
            setStatus(DownloadSession::Status::Error);
            Q_EMIT q->writeErrorOccurred(QString("Failed to write data to device. Expected: %1, Written: %2")
                                            .arg(data.size()).arg(written));
        }
    }

    bool DownloadSessionPrivate::checkFileConsistency(QNetworkReply *reply) {
        if (!reply) {
            return false;
        }
        
        // Check if server returned partial content (206) for resume request
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        
        if (startByte > 0) {
            if (statusCode == 206) {
                // Server returned partial content - this is good for resume
                // But we still need to verify file consistency
                return verifyFileConsistency(reply);
            } else if (statusCode == 200) {
                // Server ignored Range header and returned full content
                // This could mean:
                // 1. Server doesn't support ranges
                // 2. If-Range condition failed (file changed)
                // 3. Server chose to ignore the range for other reasons
                
                // If we sent If-Range and got 200, the file might have changed
                // We need to check ETag/Last-Modified to be sure
                return verifyFileConsistency(reply);
            } else {
                // Other status codes (416 Range Not Satisfiable, etc.)
                return false;
            }
        }
        
        // For initial download (startByte == 0), always consistent
        return true;
    }

    bool DownloadSessionPrivate::verifyFileConsistency(QNetworkReply *reply) {
        if (!reply) {
            return false;
        }
        
        QHttpHeaders headers = reply->headers();
        QString currentETag = QString::fromUtf8(headers.value(QHttpHeaders::WellKnownHeader::ETag));
        QString currentLastModified = QString::fromUtf8(headers.value(QHttpHeaders::WellKnownHeader::LastModified));
        
        // Case 1: We have ETag from previous request
        if (!etag.isEmpty()) {
            if (!currentETag.isEmpty()) {
                // Both old and new ETag available - compare them
                return etag == currentETag;
            } else {
                // Server stopped providing ETag - this is suspicious
                // Fall back to Last-Modified if available
                if (!lastModified.isEmpty() && !currentLastModified.isEmpty()) {
                    return lastModified == currentLastModified;
                }
                // No reliable way to verify - assume file changed
                return false;
            }
        }
        
        // Case 2: No ETag from previous request, but we have Last-Modified
        if (!lastModified.isEmpty()) {
            if (!currentLastModified.isEmpty()) {
                // Both old and new Last-Modified available - compare them
                return lastModified == currentLastModified;
            } else {
                // Server stopped providing Last-Modified - suspicious
                return false;
            }
        }
        
        // Case 3: First request or server never provided any metadata
        // We can't verify consistency, but this might be legitimate for:
        // - First download attempt
        // - Servers that don't provide ETag/Last-Modified
        if (etag.isEmpty() && lastModified.isEmpty()) {
            // For resume requests without previous metadata, be conservative
            if (startByte > 0) {
                // We're trying to resume but have no way to verify file integrity
                // This could happen if:
                // 1. Server didn't provide ETag/Last-Modified in initial request
                // 2. Previous download session lost metadata
                // Be conservative and restart from beginning
                return false;
            }
            // For initial requests, always OK
            return true;
        }
        
        // Should not reach here, but be safe
        return false;
    }

    void DownloadSessionPrivate::saveResumeMetadata(QNetworkReply *reply) {
        Q_Q(DownloadSession);
        if (!reply) {
            return;
        }
        
        // Get response headers
        QHttpHeaders headers = reply->headers();
        
        // Check if server supports range requests
        QByteArrayView acceptRanges = headers.value(QHttpHeaders::WellKnownHeader::AcceptRanges);
        bool newSupportsResume = acceptRanges.contains("bytes");
        
        // Emit signal if support status changed
        if (supportsResume != newSupportsResume) {
            supportsResume = newSupportsResume;
            Q_EMIT q->supportsResumeChanged(supportsResume);
        }
        
        // Save ETag and Last-Modified for consistency checking
        etag = QString::fromUtf8(headers.value(QHttpHeaders::WellKnownHeader::ETag));
        lastModified = QString::fromUtf8(headers.value(QHttpHeaders::WellKnownHeader::LastModified));
    }

    QNetworkRequest DownloadSessionPrivate::createRequest(qint64 startFromByte) {
        QNetworkRequest request(url);
        
        // Set standard headers using QHttpHeaders
        QHttpHeaders headers;
        headers.append(QHttpHeaders::WellKnownHeader::UserAgent, userAgent.toUtf8());
        
        // Set range header for resume
        if (startFromByte > 0) {
            QByteArray rangeValue = "bytes=" + QByteArray::number(startFromByte) + "-";
            headers.append(QHttpHeaders::WellKnownHeader::Range, rangeValue);
            
            // Set If-Range header for consistency checking
            // If-Range ensures that:
            // - If file hasn't changed (ETag/Last-Modified matches), server returns 206 with partial content
            // - If file has changed, server returns 200 with full content
            // This prevents downloading corrupt partial content from a changed file
            if (!etag.isEmpty()) {
                // Prefer ETag as it's more reliable than timestamp
                headers.append(QHttpHeaders::WellKnownHeader::IfRange, etag.toUtf8());
            } else if (!lastModified.isEmpty()) {
                // Fall back to Last-Modified if ETag not available
                headers.append(QHttpHeaders::WellKnownHeader::IfRange, lastModified.toUtf8());
            }
            // Note: If neither ETag nor Last-Modified is available from previous request,
            // we don't send If-Range and rely on manual consistency checking in verifyFileConsistency()
        }
        
        request.setHeaders(headers);
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        request.setAttribute(QNetworkRequest::AutoDeleteReplyOnFinishAttribute, false);

        return request;
    }

}
