/*
    SPDX-FileCopyrightText: 2010 Michael Zanetti <mzanetti@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef UDISKSDEVICEINTERFACE_H
#define UDISKSDEVICEINTERFACE_H

#include <ifaces/deviceinterface.h>
#include "udisksdevice.h"

#include <QObject>
#include <QStringList>

namespace Solid
{
namespace Backends
{
namespace UDisks2
{

class DeviceInterface : public QObject, virtual public Solid::Ifaces::DeviceInterface
{
    Q_OBJECT
    Q_INTERFACES(Solid::Ifaces::DeviceInterface)
public:
    DeviceInterface(Device *device);
    virtual ~DeviceInterface();

protected:
    Device *m_device;

public:
    inline static QStringList toStringList(Solid::DeviceInterface::Type type)
    {
        QStringList list;

        switch (type) {
        case Solid::DeviceInterface::GenericInterface:
            list << "generic";
            break;
        case Solid::DeviceInterface::Block:
            list << "block";
            break;
        case Solid::DeviceInterface::StorageAccess:
            list << "volume";
            break;
        case Solid::DeviceInterface::StorageDrive:
            list << "storage";
            break;
        case Solid::DeviceInterface::StorageVolume:
            list << "volume";
            break;
        case Solid::DeviceInterface::Unknown:
            break;
        case Solid::DeviceInterface::Last:
            break;
        }

        return list;
    }

    inline static Solid::DeviceInterface::Type fromString(const QString &capability)
    {
        if (capability == "generic") {
            return Solid::DeviceInterface::GenericInterface;
        } else if (capability == "block") {
            return Solid::DeviceInterface::Block;
        } else if (capability == "storage") {
            return Solid::DeviceInterface::StorageDrive;
        } else if (capability == "volume") {
            return Solid::DeviceInterface::StorageVolume;
        } else {
            return Solid::DeviceInterface::Unknown;
        }
    }
};

}
}
}

#endif // UDISKSDEVICEINTERFACE_H
