#include "svscraftvaluetypes_p.h"

#include <QVariant>

namespace SVS {
    LongTimeForeign::LongTimeForeign(const QVariantMap &time) {
        v = LongTime(time.value("minute").toInt(), time.value("second").toInt(), time.value("millisecond").toInt());
    }

    MusicTimeForeign::MusicTimeForeign(const QVariantMap &time) {
        v = MusicTime(time.value("measure").toInt(), time.value("beat").toInt(), time.value("tick").toInt());
    }
    MusicTimeSignatureForeign::MusicTimeSignatureForeign(const QVariantMap &signature) {
        v = MusicTimeSignature(signature.value("numerator").toInt(), signature.value("denominator").toInt());
    }
    MusicPitchForeign::MusicPitchForeign(const QVariantMap &pitch) {
        auto key = pitch.value("key").toInt();
        auto octave = pitch.value("octave").toInt();
        auto prediction = key >= 0 && key < 12 && (octave == -1 || (octave >= 0 && key + octave * 12 >= 0 && key + octave * 12 < 128));
        if (!prediction) {
            v = {};
        } else {
            v = MusicPitch(key, octave);
        }
    }
}

#include "moc_svscraftvaluetypes_p.cpp"