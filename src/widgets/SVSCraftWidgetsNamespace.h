#ifndef SVSCRAFT_SVSCRAFTWIDGETSNAMESPACE_H
#define SVSCRAFT_SVSCRAFTWIDGETSNAMESPACE_H

#include <QObject>

#include <SVSCraftWidgets/SVSCraftWidgetsGlobal.h>

namespace SVS {

    Q_NAMESPACE_EXPORT(SVSCRAFT_WIDGETS_EXPORT)

    enum Role {
        DisplayRole = Qt::DisplayRole,
        DecorationRole = Qt::DecorationRole,

        // Customized
        SubtitleRole = Qt::UserRole + 2000,
        DescriptionRole,
        CategoryRole,
        EnumerationRole,
        SeparatorRole,
        IconSizeRole,
        ObjectPointerRole,
        AlignmentRole,
        InternalDataRole,
        InternalTypeRole,
        KeywordRole,

        UserRole = Qt::UserRole + 4000,
    };

    Q_ENUM_NS(Role)

}

#endif // SVSCRAFT_SVSCRAFTWIDGETSNAMESPACE_H
