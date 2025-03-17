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

#ifndef SVSCRAFT_DESCRIPTIVETEXT_P_H
#define SVSCRAFT_DESCRIPTIVETEXT_P_H

#include <QObject>
#include <qqmlintegration.h>

class QQuickItem;

namespace SVS {

    class DescriptiveTextAttachedType;

    class DescriptiveTextPrivate;

    class DescriptiveText : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DescriptiveText)
        QML_ANONYMOUS

        Q_PROPERTY(bool activated READ activated WRITE setActivated NOTIFY activatedChanged)
        Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip NOTIFY toolTipChanged)
        Q_PROPERTY(QString statusTip READ statusTip WRITE setStatusTip NOTIFY statusTipChanged)
        Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
        Q_PROPERTY(QString defaultStatusText READ defaultStatusText WRITE setDefaultStatusText NOTIFY defaultStatusTextChanged)

    public:
        ~DescriptiveText() override;

        bool activated() const;
        void setActivated(bool activated);

        QString toolTip() const;
        void setToolTip(const QString &toolTip);

        QString statusTip() const;
        void setStatusTip(const QString &statusTip);

        QString statusText() const;

        QString defaultStatusText() const;
        void setDefaultStatusText(const QString &defaultStatusText);

    signals:
        void activatedChanged();
        void toolTipChanged();
        void statusTipChanged();
        void statusTextChanged();
        void defaultStatusTextChanged();

    private:
        friend class DescriptiveTextAttachedType;
        explicit DescriptiveText(QQuickItem *parent = nullptr);

        QScopedPointer<DescriptiveTextPrivate> d_ptr;

    };

}

#endif // SVSCRAFT_DESCRIPTIVETEXT_P_H
