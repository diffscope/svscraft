#include "musicmode.h"

namespace SVS {

    QList<MusicPitch> MusicMode::scale(MusicPitch tonic) const {
        QList<MusicPitch> pitches;
        for (int i = 0; i < 12; i++) {
            if ((1 << i) & flags) {
                if (tonic.isWildcard()) {
                    pitches.append(MusicPitch(qint8(tonic.key() + i % 12), MusicPitch::Wildcard));
                } else if (tonic.pitch() + i < 128) {
                    pitches.append(MusicPitch(qint8(tonic.pitch() + i)));
                }
            }
        }
        return pitches;
    }

}