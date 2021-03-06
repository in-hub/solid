/*
    SPDX-FileCopyrightText: 2007 Kevin Ottens <ervin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef SOLID_BACKENDS_HAL_GENERICINTERFACE_H
#define SOLID_BACKENDS_HAL_GENERICINTERFACE_H

#include <solid/devices/ifaces/genericinterface.h>
#include <solid/genericinterface.h>
#include "haldeviceinterface.h"

namespace Solid
{
namespace Backends
{
namespace Hal
{
class HalDevice;

class GenericInterface : public DeviceInterface, virtual public Solid::Ifaces::GenericInterface
{
    Q_OBJECT
    Q_INTERFACES(Solid::Ifaces::GenericInterface)

public:
    GenericInterface(HalDevice *device);
    virtual ~GenericInterface();

    QVariant property(const QString &key) const override;
    QMap<QString, QVariant> allProperties() const override;
    bool propertyExists(const QString &key) const override;

Q_SIGNALS:
    void propertyChanged(const QMap<QString, int> &changes) override;
    void conditionRaised(const QString &condition, const QString &reason) override;
};
}
}
}

#endif // SOLID_BACKENDS_HAL_GENERICINTERFACE_H
