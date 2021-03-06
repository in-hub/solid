/*
    SPDX-FileCopyrightText: 2014 Alejandro Fiestas Olivares <afiestas@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "solid-power.h"

#include <solid/power.h>
#include <solid/acpluggedjob.h>

#include <QTextStream>

using namespace Solid;
using namespace std;

static QTextStream sCout(stdout);

SolidPower::SolidPower(QObject* parent) : QObject(parent)
{
}

void SolidPower::show()
{
    auto job = Solid::Power::isAcPlugged(this);
    job->exec();

    QString status = job->isPlugged() ? "yes" : "no";

    sCout << "is AC plugged:\t" << status << endl;
}

void SolidPower::listen()
{
    auto power = Power::self();

    connect(power, &Power::acPluggedChanged, [](bool plugged){
        QTextStream lOut(stdout);
        QString status = plugged ? "yes" : "no";
        lOut << "\tAC plugged changed:\t" << status << endl;
    });

    connect(power, &Power::aboutToSuspend, [](){
        QTextStream lOut(stdout);
        lOut << "\tAbout to suspend" << endl;
    });

    connect(power, &Power::resumeFromSuspend, [](){
        QTextStream lOut(stdout);
        lOut << "\tResume from suspend" << endl;
    });
}
