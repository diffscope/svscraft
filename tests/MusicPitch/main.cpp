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

#include <QtTest/QtTest>

#include <SVSCraftCore/MusicPitch.h>

using namespace SVS;

class TestMusicNote : public QObject {
    Q_OBJECT
private slots:
    void fromString_data() {
        QTest::addColumn<QString>("string");
        QTest::addColumn<int>("pitch");
        QTest::addColumn<bool>("result");

        QTest::newRow("ordinary") << "C5" << 60 << true;
        QTest::newRow("wildcard") << "C?" << -12 << true;
        QTest::newRow("accidental") << "Eb8" << 99 << true;
        QTest::newRow("spacing char") << " C#  5  " << 61 << true;
        QTest::newRow("multiple accidental") << "C###bb#b##5" << 63 << true;
        QTest::newRow("out of range") << "G#10" << 0 << false;
        QTest::newRow("invalid") << "H5" << 0 << false;
    }

    void fromString() {
        QFETCH(QString, string);
        QFETCH(int, pitch);
        QFETCH(bool, result);

        bool ok;
        auto n = MusicPitch::fromString(string, &ok);
        QCOMPARE(ok, result);
        QCOMPARE(n.pitch(), pitch);
    };
};

QTEST_MAIN(TestMusicNote)

#include "main.moc"
