#include "svscraftvaluetypes_p.h"

#include <QVariant>

namespace SVS {
    LongTimeForeign::LongTimeForeign(const QVariantList &time) {
        v = LongTime(time.size() >= 1 ? time[0].toInt() : 0, time.size() >= 2 ? time[1].toInt() : 0, time.size() >= 3 ? time[2].toInt() : 0);
    }

    MusicTimeForeign::MusicTimeForeign(const QVariantList &time) {
        v = MusicTime(time.size() >= 1 ? time[0].toInt() : 0, time.size() >= 2 ? time[1].toInt() : 0, time.size() >= 3 ? time[2].toInt() : 0);
    }
    MusicTimeSignatureForeign::MusicTimeSignatureForeign(const QVariantList &signature) {
        v = MusicTimeSignature(signature.size() >= 1 ? signature[0].toInt() : 0, signature.size() >= 2 ? signature[1].toInt() : 0);
    }
    MusicPitchForeign::MusicPitchForeign(const QVariantList &pitch) {
        auto key = pitch.size() >= 1 ? pitch[0].toInt() : 0;
        auto octave = pitch.size() >= 2 ? pitch[1].toInt() : 0;
        auto prediction = key >= 0 && key < 12 && (octave == -1 || (octave >= 0 && key + octave * 12 >= 0 && key + octave * 12 < 128));
        if (!prediction) {
            v = {};
        } else {
            v = MusicPitch(key, octave);
        }
    }
}

#include "moc_svscraftvaluetypes_p.cpp"