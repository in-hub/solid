/*
    Copyright 2013 Albert Vaca <albertvaka@gmail.com>

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

#include "phonepowerbattery.h"

using namespace Solid::Backends::PhonePower;

Battery::Battery(PhonePowerDevice *device)
    : DeviceInterface(device)
    , mDevice(device)
{
    connect(device, SIGNAL(changed()), this, SLOT(slotChanged()));

}

Battery::~Battery()
{
}

bool Battery::isPlugged() const
{
    return true;
}

Solid::Battery::BatteryType Battery::type() const
{
    return Solid::Battery::PhoneBattery;
}

int Battery::chargePercent() const
{
    return mDevice->chargePercent();
}

int Battery::capacity() const
{
    return 100;
}

bool Battery::isRechargeable() const
{
    return true;
}

bool Battery::isPowerSupply() const
{
    return false;
}

Solid::Battery::ChargeState Battery::chargeState() const
{
    return mDevice->chargeState();
}

void Battery::slotChanged()
{
    /*
    if (m_device) {
        const int old_chargePercent = m_chargePercent;
        const int old_capacity = m_capacity;
        const Solid::Battery::ChargeState old_chargeState = m_chargeState;
        const bool old_isPlugged = m_isPlugged;
        const bool old_isPowerSupply = m_isPowerSupply;
        updateCache();

        if (old_chargePercent != m_chargePercent)
        {
            emit chargePercentChanged(m_chargePercent, m_device.data()->udi());
        }

        if (old_capacity != m_capacity) {
            emit capacityChanged(m_capacity, m_device.data()->udi());
        }

        if (old_chargeState != m_chargeState)
        {
            emit chargeStateChanged(m_chargeState, m_device.data()->udi());
        }

        if (old_isPlugged != m_isPlugged)
        {
            emit plugStateChanged(m_isPlugged, m_device.data()->udi());
        }

        if (old_isPowerSupply != m_isPowerSupply)
        {
            emit powerSupplyStateChanged(m_isPowerSupply, m_device.data()->udi());
        }
    }

    */
}

#include "backends/phonepower/phonepowerbattery.moc"