#include "Semver.h"
#include "Semver_p.h"

#include <QString>
#include <QStringList>

namespace SVS {

    Semver::Semver() = default;

    Semver::Semver(int major, int minor, int patch, const QStringList& preRelease,
                   const QStringList &build) {
        QString semverString = QString("%1.%2.%3").arg(major).arg(minor).arg(patch);
        
        if (!preRelease.isEmpty()) {
            semverString += "-" + preRelease.join(".");
        }
        
        if (!build.isEmpty()) {
            semverString += "+" + build.join(".");
        }
        
        semver::version<> version;
        if (semver::parse(semverString.toStdString(), version)) {
            auto data = new SemverData;
            data->semver = std::move(version);
            d.reset(data);
        }
    }

    Semver::Semver(const QString &semver) {
        semver::version<> version;
        if (semver::parse(semver.toStdString(), version)) {
            auto data = new SemverData;
            data->semver = std::move(version);
            d.reset(data);
        }
    }

    Semver::Semver(const Semver &other) : d(other.d) {
    }

    Semver &Semver::operator=(const Semver &other) {
        d = other.d;
        return *this;
    }

    Semver::Semver(Semver &&other) noexcept : d(std::move(other.d)) {
    }

    Semver &Semver::operator=(Semver &&other) noexcept {
        d = std::move(other.d);
        return *this;
    }

    Semver::~Semver() = default;

    bool Semver::isValid() const {
        return d != nullptr;
    }

    int Semver::major() const {
        return d ? static_cast<int>(d->semver.major()) : 0;
    }

    int Semver::minor() const {
        return d ? static_cast<int>(d->semver.minor()) : 0;
    }

    int Semver::patch() const {
        return d ? static_cast<int>(d->semver.patch()) : 0;
    }

    QStringList Semver::preRelease() const {
        if (!d) {
            return {};
        }
        const auto &preReleaseTag = d->semver.prerelease_tag();
        if (preReleaseTag.empty()) {
            return {};
        }
        return QString::fromStdString(preReleaseTag).split('.');
    }

    QStringList Semver::build() const {
        if (!d) {
            return {};
        }
        const auto &buildMetadata = d->semver.build_metadata();
        if (buildMetadata.empty()) {
            return {};
        }
        return QString::fromStdString(buildMetadata).split('.');
    }

    QString Semver::toString() const {
        return d ? QString::fromStdString(d->semver.to_string()) : QString();
    }

    std::partial_ordering Semver::operator<=>(const Semver &other) const {
        if (!d || !other.d) {
            return std::partial_ordering::unordered;
        }
        return d->semver <=> other.d->semver;
    }

    bool Semver::operator==(const Semver &other) const {
        if (!d || !other.d) {
            return false;
        }
        return d->semver == other.d->semver;
    }

    bool Semver::operator!=(const Semver &other) const {
        return !(*this == other);
    }

    QDataStream &operator<<(QDataStream &stream, const Semver &s) {
        return stream << s.toString();
    }
    QDataStream &operator>>(QDataStream &stream, Semver &s) {
        QString str;
        stream >> str;
        s.d.reset();
        if (str.isEmpty()) {
            return stream;
        }
        semver::version<> version;
        if (semver::parse(str.toStdString(), version)) {
            auto data = new SemverData;
            data->semver = std::move(version);
            s.d.reset(data);
        }
        return stream;
    }
}