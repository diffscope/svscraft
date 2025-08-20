#ifndef SVSCRAFT_SEMVER_H
#define SVSCRAFT_SEMVER_H

#include <compare>

#include <QSharedDataPointer>
#include <QMetaType>
#include <QStringList>

#include <SVSCraftCore/SVSCraftCoreGlobal.h>

namespace SVS {

    class SemverData;

    class SVSCRAFT_CORE_EXPORT Semver {
        Q_GADGET
        Q_PROPERTY(bool valid READ isValid CONSTANT)
        Q_PROPERTY(int major READ major CONSTANT)
        Q_PROPERTY(int minor READ minor CONSTANT)
        Q_PROPERTY(int patch READ patch CONSTANT)
        Q_PROPERTY(QStringList preRelease READ preRelease CONSTANT)
        Q_PROPERTY(QStringList build READ build CONSTANT)

    public:
        Semver();
        Semver(int major, int minor, int patch, const QStringList& preRelease = {},
               const QStringList &build = {});

        Q_INVOKABLE explicit Semver(const QString &semver);

        Semver(const Semver &other);
        Semver &operator=(const Semver &other);
        Semver(Semver &&other) noexcept;
        Semver &operator=(Semver &&other) noexcept;

        ~Semver();

        bool isValid() const;

        int major() const;
        int minor() const;
        int patch() const;
        QStringList preRelease() const;
        QStringList build() const;

        Q_INVOKABLE QString toString() const;

        std::partial_ordering operator<=>(const Semver &) const;
        bool operator==(const Semver &) const;
        bool operator!=(const Semver &) const;

        SVSCRAFT_CORE_EXPORT friend QDataStream &operator<<(QDataStream &, const Semver &);
        SVSCRAFT_CORE_EXPORT friend QDataStream &operator>>(QDataStream &, Semver &);

    private:
        QSharedDataPointer<SemverData> d;

    };

}

#endif //SVSCRAFT_SEMVER_H
