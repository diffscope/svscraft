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

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QGroupBox>
#include <QTextEdit>
#include <QFile>
#include <QTimer>
#include <QElapsedTimer>
#include <QNetworkAccessManager>

#include <SVSCraftNetwork/DownloadSession.h>

class DownloadTestWidget : public QWidget {
    Q_OBJECT

public:
    explicit DownloadTestWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setupUI();
        setupConnections();
        
        // Set some default test URLs
        urlEdit->setText("https://httpbin.org/bytes/1048576"); // 1MB test file
        savePathEdit->setText(QDir::homePath() + "/download_test.bin");
        
        networkManager = new QNetworkAccessManager(this);
        updateStatus();
    }

private slots:
    void browseFile() {
        QString fileName = QFileDialog::getSaveFileName(this, 
            tr("Save Downloaded File"), savePathEdit->text());
        if (!fileName.isEmpty()) {
            savePathEdit->setText(fileName);
        }
    }
    
    void startDownload() {
        QString url = urlEdit->text().trimmed();
        QString filePath = savePathEdit->text().trimmed();
        
        if (url.isEmpty() || filePath.isEmpty()) {
            QMessageBox::warning(this, tr("Warning"), 
                tr("Please enter both URL and file path."));
            return;
        }
        
        // Create or recreate download session
        if (downloadSession) {
            downloadSession->deleteLater();
        }
        
        // Open file for writing
        if (file) {
            file->close();
            delete file;
        }
        
        file = new QFile(filePath, this);
        if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QMessageBox::critical(this, tr("Error"), 
                tr("Cannot open file for writing: %1").arg(file->errorString()));
            delete file;
            file = nullptr;
            return;
        }
        
        downloadSession = new SVS::DownloadSession(QUrl(url), file, networkManager, this);
        downloadSession->setUserAgent("DownloadSessionTest/1.0 (SVSCraft)");
        connectSessionSignals();
        
        downloadSession->start();
        logMessage(tr("Download started: %1").arg(url));
    }
    
    void pauseDownload() {
        if (downloadSession) {
            downloadSession->pause();
            logMessage(tr("Download paused"));
        }
    }
    
    void resumeDownload() {
        if (downloadSession) {
            downloadSession->resume();
            logMessage(tr("Download resumed"));
        }
    }
    
    void cancelDownload() {
        if (downloadSession) {
            downloadSession->cancel();
            logMessage(tr("Download cancelled"));
        }
    }
    
    void resetDownload() {
        if (downloadSession) {
            downloadSession->reset();
            logMessage(tr("Download reset"));
        }
        
        // Close and clean up file
        if (file && file->isOpen()) {
            file->flush();
            file->close();
            logMessage(tr("File closed"));
        }
    }
    
    void onStatusChanged(SVS::DownloadSession::Status status) {
        updateStatus();
        
        QString statusText;
        switch (status) {
            case SVS::DownloadSession::Status::Ready:
                statusText = tr("Ready");
                break;
            case SVS::DownloadSession::Status::Downloading:
                statusText = tr("Downloading");
                break;
            case SVS::DownloadSession::Status::Paused:
                statusText = tr("Paused");
                break;
            case SVS::DownloadSession::Status::Completed:
                statusText = tr("Completed");
                // Ensure file is flushed when download completes
                if (file && file->isOpen()) {
                    file->flush();
                    logMessage(tr("Download completed, file flushed to disk"));
                }
                break;
            case SVS::DownloadSession::Status::Cancelled:
                statusText = tr("Cancelled");
                break;
            case SVS::DownloadSession::Status::Error:
                statusText = tr("Error");
                // Flush any partial data when error occurs
                if (file && file->isOpen()) {
                    file->flush();
                    logMessage(tr("Error occurred, partial data flushed to disk"));
                }
                break;
        }
        
        statusLabel->setText(tr("Status: %1").arg(statusText));
        logMessage(tr("Status changed to: %1").arg(statusText));
    }
    
    void onProgressChanged(qint64 received, qint64 total) {
        if (total > 0) {
            int percentage = static_cast<int>((received * 100) / total);
            progressBar->setValue(percentage);
            progressBar->setFormat(tr("%1% (%2/%3)")
                .arg(percentage)
                .arg(formatBytes(received))
                .arg(formatBytes(total)));
        } else {
            progressBar->setValue(0);
            progressBar->setFormat(tr("Received: %1").arg(formatBytes(received)));
        }
        
        receivedLabel->setText(tr("Received: %1").arg(formatBytes(received)));
        if (total > 0) {
            totalLabel->setText(tr("Total: %1").arg(formatBytes(total)));
        }
    }
    
    void onSpeedChanged(double speed) {
        speedLabel->setText(tr("Speed: %1/s").arg(formatBytes(static_cast<qint64>(speed))));
        
        if (downloadSession) {
            qint64 elapsed = downloadSession->elapsedTime();
            qint64 estimated = downloadSession->estimatedTime();
            
            elapsedLabel->setText(tr("Elapsed: %1").arg(formatTime(elapsed)));
            
            if (estimated >= 0) {
                estimatedLabel->setText(tr("Estimated: %1").arg(formatTime(estimated)));
            } else {
                estimatedLabel->setText(tr("Estimated: Unknown"));
            }
        }
    }
    
    void onSupportsResumeChanged(bool supportsResume) {
        updateStatus();
        
        if (supportsResume) {
            logMessage(tr("Server supports resume functionality"));
        } else {
            logMessage(tr("Server does not support resume functionality"));
        }
    }
    
    void onNetworkError(QNetworkReply::NetworkError error, const QString &description) {
        QString errorMsg = tr("Network Error [%1]: %2").arg(static_cast<int>(error)).arg(description);
        logMessage(errorMsg);
        QMessageBox::critical(this, tr("Network Error"), errorMsg);
    }
    
    void onWriteError(const QString &description) {
        QString errorMsg = tr("Write Error: %1").arg(description);
        logMessage(errorMsg);
        QMessageBox::critical(this, tr("Write Error"), errorMsg);
    }
    
    void onFinished() {
        // Flush and close file to ensure data is written to disk
        if (file && file->isOpen()) {
            file->flush();
            if (downloadSession && downloadSession->status() == SVS::DownloadSession::Status::Completed) {
                logMessage(tr("File flushed successfully"));
            }
        }
        
        logMessage(tr("Download finished"));
        updateStatus();
    }

private:
    void setupUI() {
        setWindowTitle(tr("DownloadSession Test Application"));
        setMinimumSize(600, 500);
        
        auto *mainLayout = new QVBoxLayout(this);
        
        // URL and file selection
        auto *inputGroup = new QGroupBox(tr("Download Settings"));
        auto *inputLayout = new QGridLayout(inputGroup);
        
        inputLayout->addWidget(new QLabel(tr("Download URL:")), 0, 0);
        urlEdit = new QLineEdit;
        inputLayout->addWidget(urlEdit, 0, 1);
        
        inputLayout->addWidget(new QLabel(tr("Save to:")), 1, 0);
        savePathEdit = new QLineEdit;
        inputLayout->addWidget(savePathEdit, 1, 1);
        browseButton = new QPushButton(tr("Browse..."));
        inputLayout->addWidget(browseButton, 1, 2);
        
        mainLayout->addWidget(inputGroup);
        
        // Control buttons
        auto *controlGroup = new QGroupBox(tr("Download Control"));
        auto *controlLayout = new QHBoxLayout(controlGroup);
        
        startButton = new QPushButton(tr("Start"));
        pauseButton = new QPushButton(tr("Pause"));
        resumeButton = new QPushButton(tr("Resume"));
        cancelButton = new QPushButton(tr("Cancel"));
        resetButton = new QPushButton(tr("Reset"));
        
        controlLayout->addWidget(startButton);
        controlLayout->addWidget(pauseButton);
        controlLayout->addWidget(resumeButton);
        controlLayout->addWidget(cancelButton);
        controlLayout->addWidget(resetButton);
        controlLayout->addStretch();
        
        mainLayout->addWidget(controlGroup);
        
        // Progress display
        auto *progressGroup = new QGroupBox(tr("Progress Information"));
        auto *progressLayout = new QGridLayout(progressGroup);
        
        statusLabel = new QLabel(tr("Status: Ready"));
        progressLayout->addWidget(statusLabel, 0, 0, 1, 2);
        
        progressBar = new QProgressBar;
        progressBar->setRange(0, 100);
        progressBar->setValue(0);
        progressBar->setFormat(tr("0% (0/0)"));
        progressLayout->addWidget(progressBar, 1, 0, 1, 2);
        
        receivedLabel = new QLabel(tr("Received: 0 B"));
        totalLabel = new QLabel(tr("Total: Unknown"));
        speedLabel = new QLabel(tr("Speed: 0 B/s"));
        elapsedLabel = new QLabel(tr("Elapsed: 00:00"));
        estimatedLabel = new QLabel(tr("Estimated: Unknown"));
        
        progressLayout->addWidget(receivedLabel, 2, 0);
        progressLayout->addWidget(totalLabel, 2, 1);
        progressLayout->addWidget(speedLabel, 3, 0);
        progressLayout->addWidget(elapsedLabel, 3, 1);
        progressLayout->addWidget(estimatedLabel, 4, 0, 1, 2);
        
        mainLayout->addWidget(progressGroup);
        
        // Log display
        auto *logGroup = new QGroupBox(tr("Log Messages"));
        auto *logLayout = new QVBoxLayout(logGroup);
        
        logEdit = new QTextEdit;
        logEdit->setMaximumHeight(150);
        logEdit->setReadOnly(true);
        logLayout->addWidget(logEdit);
        
        mainLayout->addWidget(logGroup);
    }
    
    void setupConnections() {
        connect(browseButton, &QPushButton::clicked, this, &DownloadTestWidget::browseFile);
        connect(startButton, &QPushButton::clicked, this, &DownloadTestWidget::startDownload);
        connect(pauseButton, &QPushButton::clicked, this, &DownloadTestWidget::pauseDownload);
        connect(resumeButton, &QPushButton::clicked, this, &DownloadTestWidget::resumeDownload);
        connect(cancelButton, &QPushButton::clicked, this, &DownloadTestWidget::cancelDownload);
        connect(resetButton, &QPushButton::clicked, this, &DownloadTestWidget::resetDownload);
    }
    
    void connectSessionSignals() {
        if (!downloadSession) return;
        
        connect(downloadSession, &SVS::DownloadSession::statusChanged,
                this, &DownloadTestWidget::onStatusChanged);
        connect(downloadSession, &SVS::DownloadSession::progressChanged,
                this, &DownloadTestWidget::onProgressChanged);
        connect(downloadSession, &SVS::DownloadSession::speedChanged,
                this, &DownloadTestWidget::onSpeedChanged);
        connect(downloadSession, &SVS::DownloadSession::supportsResumeChanged,
                this, &DownloadTestWidget::onSupportsResumeChanged);
        connect(downloadSession, &SVS::DownloadSession::networkErrorOccurred,
                this, &DownloadTestWidget::onNetworkError);
        connect(downloadSession, &SVS::DownloadSession::writeErrorOccurred,
                this, &DownloadTestWidget::onWriteError);
        connect(downloadSession, &SVS::DownloadSession::finished,
                this, &DownloadTestWidget::onFinished);
    }
    
    void updateStatus() {
        bool hasSession = downloadSession != nullptr;
        bool isReady = hasSession && downloadSession->status() == SVS::DownloadSession::Status::Ready;
        bool isDownloading = hasSession && downloadSession->status() == SVS::DownloadSession::Status::Downloading;
        bool isPaused = hasSession && downloadSession->status() == SVS::DownloadSession::Status::Paused;
        bool canReset = hasSession && (downloadSession->status() == SVS::DownloadSession::Status::Completed ||
                                      downloadSession->status() == SVS::DownloadSession::Status::Cancelled ||
                                      downloadSession->status() == SVS::DownloadSession::Status::Error);
        bool supportsResume = hasSession && downloadSession->supportsResume();
        
        startButton->setEnabled(!hasSession || isReady);
        pauseButton->setEnabled(isDownloading && supportsResume);
        resumeButton->setEnabled(isPaused);
        cancelButton->setEnabled(isDownloading || isPaused);
        resetButton->setEnabled(canReset);
    }
    
    void logMessage(const QString &message) {
        QString timestamp = QTime::currentTime().toString("hh:mm:ss");
        logEdit->append(QString("[%1] %2").arg(timestamp, message));
    }
    
    void clearProgress() {
        progressBar->setValue(0);
        progressBar->setFormat(tr("0% (0/0)"));
        receivedLabel->setText(tr("Received: 0 B"));
        totalLabel->setText(tr("Total: Unknown"));
        speedLabel->setText(tr("Speed: 0 B/s"));
        elapsedLabel->setText(tr("Elapsed: 00:00"));
        estimatedLabel->setText(tr("Estimated: Unknown"));
    }
    
    QString formatBytes(qint64 bytes) {
        const QStringList units = {tr("B"), tr("KB"), tr("MB"), tr("GB"), tr("TB")};
        double size = bytes;
        int unitIndex = 0;
        
        while (size >= 1024.0 && unitIndex < units.size() - 1) {
            size /= 1024.0;
            ++unitIndex;
        }
        
        return QString("%1 %2").arg(QString::number(size, 'f', 2)).arg(units[unitIndex]);
    }
    
    QString formatTime(qint64 milliseconds) {
        if (milliseconds < 0) return tr("Unknown");
        
        int seconds = static_cast<int>(milliseconds / 1000);
        int minutes = seconds / 60;
        int hours = minutes / 60;
        
        seconds %= 60;
        minutes %= 60;
        
        if (hours > 0) {
            return QString("%1:%2:%3")
                .arg(hours, 2, 10, QChar('0'))
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0'));
        } else {
            return QString("%1:%2")
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0'));
        }
    }

private:
    // UI components
    QLineEdit *urlEdit = nullptr;
    QLineEdit *savePathEdit = nullptr;
    QPushButton *browseButton = nullptr;
    QPushButton *startButton = nullptr;
    QPushButton *pauseButton = nullptr;
    QPushButton *resumeButton = nullptr;
    QPushButton *cancelButton = nullptr;
    QPushButton *resetButton = nullptr;
    QProgressBar *progressBar = nullptr;
    QLabel *statusLabel = nullptr;
    QLabel *receivedLabel = nullptr;
    QLabel *totalLabel = nullptr;
    QLabel *speedLabel = nullptr;
    QLabel *elapsedLabel = nullptr;
    QLabel *estimatedLabel = nullptr;
    QTextEdit *logEdit = nullptr;
    
    // Download components
    SVS::DownloadSession *downloadSession = nullptr;
    QNetworkAccessManager *networkManager = nullptr;
    QFile *file = nullptr;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    app.setApplicationName("DownloadSession Test");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("OpenVPI");
    app.setOrganizationDomain("openvpi.org");
    
    DownloadTestWidget widget;
    widget.show();
    
    return app.exec();
}

#include "main.moc"
