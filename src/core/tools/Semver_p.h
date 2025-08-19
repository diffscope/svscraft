#ifndef SVSCRAFT_SEMVER_P_H
#define SVSCRAFT_SEMVER_P_H

#include <SVSCraftCore/Semver.h>

#include <QSharedData>

#include <semver.hpp>

namespace SVS {

    class SemverData : public QSharedData {
    public:
        semver::version<> semver;
    };

}

#endif //SVSCRAFT_SEMVER_P_H
