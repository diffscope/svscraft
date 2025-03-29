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

#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QQuickStyle>
#include <QFile>

#include <SVSCraftCore/WaveformMipmap.h>

using namespace SVS;

class Helper : public QObject {
    Q_OBJECT
    Q_PROPERTY(SVS::WaveformMipmap waveformMipmap READ waveformMipmap NOTIFY waveformMipmapChanged)
    Q_PROPERTY(qsizetype size READ size NOTIFY waveformMipmapChanged)
public:
    Q_INVOKABLE void load(const QUrl &url, WaveformMipmap::Level level, WaveformMipmap::SampleType sampleType, bool useRms) {
        QFile f(url.toLocalFile());
        f.open(QIODevice::ReadOnly);
        auto b = f.readAll();
        auto fileSize = b.size() / 4;
        m_waveformMipmap = WaveformMipmap(fileSize, level, sampleType, useRms);
        m_waveformMipmap.load(reinterpret_cast<const float *>(b.constData()), 0, fileSize, 0);
        emit waveformMipmapChanged();
    }
    Q_INVOKABLE void loadMipmap(const QUrl &url) {
        QFile f(url.toLocalFile());
        f.open(QIODevice::ReadOnly);
        QDataStream stream(&f);
        stream >> m_waveformMipmap;
        if (!m_waveformMipmap.isValid()) {
            qDebug() << "Failed to load mipmap";
        }
        emit waveformMipmapChanged();
    }
    Q_INVOKABLE void saveMipmap(const QUrl &url) const {
        QFile f(url.toLocalFile());
        f.open(QIODevice::WriteOnly);
        QDataStream stream(&f);
        stream << m_waveformMipmap;
    }

    WaveformMipmap waveformMipmap() const {
        return m_waveformMipmap;
    }

    qsizetype size() const {
        return m_waveformMipmap.size();
    }

    WaveformMipmap m_waveformMipmap;

signals:
    void waveformMipmapChanged();
};

int main(int argc, char *argv[]) {
    QGuiApplication a(argc, argv);

    auto sf = QSurfaceFormat::defaultFormat();
    sf.setSamples(8);
    QSurfaceFormat::setDefaultFormat(sf);

    QQuickStyle::setStyle("SVSCraft.UIComponents");
    QQuickStyle::setFallbackStyle("Basic");

    QQmlApplicationEngine engine;
    Helper helper;
    QJSEngine::setObjectOwnership(&helper, QJSEngine::CppOwnership);
    engine.setInitialProperties({{"helper", QVariant::fromValue(&helper)}});
    engine.load(":/qt/qml/SVSCraft/Test/WaveformThumbnail/main.qml");

    return a.exec();
}

#include "main.moc"