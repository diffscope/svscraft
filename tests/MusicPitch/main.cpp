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
