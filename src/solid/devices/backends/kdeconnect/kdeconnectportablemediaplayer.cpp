/*
    Copyright 2015 Kai Uwe Broulik <kde@privat.broulik.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "kdeconnectportablemediaplayer.h"

#include "kdeconnectdevice.h"

#include <QDebug>

using namespace Solid::Backends::KdeConnect;

PortableMediaPlayer::PortableMediaPlayer(KdeConnectDevice *device)
    : DeviceInterface(device)
    , m_id(device->deviceId())
{
    qDebug() << "give me portable media player" << m_id;
}

PortableMediaPlayer::~PortableMediaPlayer()
{

}

QStringList PortableMediaPlayer::supportedProtocols() const
{
    return QStringList("kdeconnect");
}

QStringList PortableMediaPlayer::supportedDrivers(QString protocol) const
{
    return QStringList();
}

QVariant PortableMediaPlayer::driverHandle(const QString &driver) const
{
    qDebug() << "driver handle giving it" << m_id;
    Q_UNUSED(driver);
    return m_id;
}

//#include "backends/udev/udevportablemediaplayer.moc"
