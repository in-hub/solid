/*
    SPDX-FileCopyrightText: 2010 Mario Bensi <mbensi@ipsquad.net>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "fstabmanager.h"
#include "fstabdevice.h"
#include "fstabhandling.h"
#include "../shared/rootdevice.h"
#include "fstabservice.h"
#include "fstabwatcher.h"
#include "fstab_debug.h"

using namespace Solid::Backends::Fstab;
using namespace Solid::Backends::Shared;

FstabManager::FstabManager(QObject *parent)
    : Solid::Ifaces::DeviceManager(parent)
{
    m_supportedInterfaces << Solid::DeviceInterface::StorageAccess;
    m_supportedInterfaces << Solid::DeviceInterface::NetworkShare;

    m_deviceList = FstabHandling::deviceList();

    connect(FstabWatcher::instance(), &FstabWatcher::fstabChanged, this, &FstabManager::onFstabChanged);
    connect(FstabWatcher::instance(), &FstabWatcher::mtabChanged, this, &FstabManager::onMtabChanged);
}

QString FstabManager::udiPrefix() const
{
    return QString::fromLatin1(FSTAB_UDI_PREFIX);
}

QSet<Solid::DeviceInterface::Type> FstabManager::supportedInterfaces() const
{
    return m_supportedInterfaces;
}

QStringList FstabManager::allDevices()
{
    QStringList result;

    result << udiPrefix();
    for (const QString &device : qAsConst(m_deviceList)) {
        result << udiPrefix() + "/" + device;
    }

    return result;
}

QStringList FstabManager::devicesFromQuery(const QString &parentUdi,
        Solid::DeviceInterface::Type type)
{
    if ((parentUdi == udiPrefix()) || parentUdi.isEmpty()) {
        QStringList result;
        if (type == Solid::DeviceInterface::StorageAccess) {
            for (const QString &device : qAsConst(m_deviceList)) {
                result << udiPrefix() + "/" + device;
            }
            return result;
        } else if (type == Solid::DeviceInterface::NetworkShare) {
            for (const QString &device : qAsConst(m_deviceList)) {
                result << udiPrefix() + "/" + device;
            }
            return result;
        }
    } else {
        if (type == Solid::DeviceInterface::StorageAccess ||
            type == Solid::DeviceInterface::NetworkShare) {
            return QStringList{parentUdi};
        }
    }

    return QStringList();
}

QObject *FstabManager::createDevice(const QString &udi)
{
    if (udi == udiPrefix()) {
        RootDevice *root = new RootDevice(FSTAB_UDI_PREFIX);

        root->setProduct(tr("Filesystem Volumes"));
        root->setDescription(tr("Mountable filesystems declared in your system"));
        root->setIcon("folder");

        return root;

    } else {
        // global device manager makes sure udi starts with udi prefix + '/'
        QString internalName = udi.mid(udiPrefix().length() + 1, -1);
        if (!m_deviceList.contains(internalName)) {
            return nullptr;
        }

        FstabDevice *device = new FstabDevice(udi);
        connect(this, &FstabManager::mtabChanged, device, &FstabDevice::onMtabChanged);
        return device;

    }
}

void FstabManager::onFstabChanged()
{
    FstabHandling::flushFstabCache();
    _k_updateDeviceList();
}

void FstabManager::_k_updateDeviceList()
{
    const QStringList deviceList = FstabHandling::deviceList();
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    const QSet<QString> newlist = deviceList.toSet();
    const QSet<QString> oldlist = m_deviceList.toSet();
#else
    const QSet<QString> newlist(deviceList.begin(), deviceList.end());
    const QSet<QString> oldlist(m_deviceList.begin(), m_deviceList.end());
#endif

    m_deviceList = deviceList;

    qCDebug(FSTAB) << oldlist << "->" << newlist;

    for (const QString &device : newlist) {
        if (!oldlist.contains(device)) {
            emit deviceAdded(udiPrefix() + "/" + device);
        }
    }

    for (const QString &device : oldlist) {
        if (!newlist.contains(device)) {
            emit deviceRemoved(udiPrefix() + "/" + device);
        }
    }
}

void FstabManager::onMtabChanged()
{
    FstabHandling::flushMtabCache();

    _k_updateDeviceList(); // devicelist is union of mtab and fstab

    for (const QString &device : qAsConst(m_deviceList)) {
        // notify storageaccess objects via device ...
        emit mtabChanged(device);
    }
}

FstabManager::~FstabManager()
{
}
