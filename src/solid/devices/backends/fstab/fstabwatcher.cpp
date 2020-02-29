/*
    SPDX-FileCopyrightText: 2010 Mario Bensi <mbensi@ipsquad.net>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "fstabwatcher.h"
#include "soliddefs_p.h"

#include <QCoreApplication>
#include <QFileSystemWatcher>
#include <QSocketNotifier>
#include <QFile>

using namespace Solid::Backends::Fstab;

Q_GLOBAL_STATIC(FstabWatcher, globalFstabWatcher)

#define MTAB "/etc/mtab"
#ifdef Q_OS_SOLARIS
#define FSTAB "/etc/vfstab"
#else
#define FSTAB "/etc/fstab"
#endif

FstabWatcher::FstabWatcher()
    : m_isRoutineInstalled(false)
    , m_fileSystemWatcher(new QFileSystemWatcher(this))
{
    if (qApp) {
        connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(orphanFileSystemWatcher()));
    }

    m_mtabFile = new QFile(MTAB, this);
    if (m_mtabFile && m_mtabFile->symLinkTarget().startsWith("/proc/")
            && m_mtabFile->open(QIODevice::ReadOnly)) {

        m_mtabSocketNotifier = new QSocketNotifier(m_mtabFile->handle(),
                QSocketNotifier::Exception, this);
        connect(m_mtabSocketNotifier,
                SIGNAL(activated(int)), this, SIGNAL(mtabChanged()));
    } else {
        m_fileSystemWatcher->addPath(MTAB);
    }

    m_fileSystemWatcher->addPath(FSTAB);
    connect(m_fileSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(onFileChanged(QString)));
}

FstabWatcher::~FstabWatcher()
{
    // The QFileSystemWatcher doesn't work correctly in a singleton
    // The solution so far was to destroy the QFileSystemWatcher when the application quits
    // But we have some crash with this solution.
    // For the moment to workaround the problem, we detach the QFileSystemWatcher from the parent
    // effectively leaking it on purpose.

#if 0
    //qRemovePostRoutine(globalFstabWatcher.destroy);
#else
    m_fileSystemWatcher->setParent(nullptr);
#endif
}

void FstabWatcher::orphanFileSystemWatcher()
{
    m_fileSystemWatcher->setParent(nullptr);
}

FstabWatcher *FstabWatcher::instance()
{
#if 0
    FstabWatcher *fstabWatcher = globalFstabWatcher;

    if (fstabWatcher && !fstabWatcher->m_isRoutineInstalled) {
        qAddPostRoutine(globalFstabWatcher.destroy);
        fstabWatcher->m_isRoutineInstalled = true;
    }
    return fstabWatcher;
#else
    return globalFstabWatcher;
#endif
}

void FstabWatcher::onFileChanged(const QString &path)
{
    if (path == MTAB) {
        emit mtabChanged();
        if (!m_fileSystemWatcher->files().contains(MTAB)) {
            m_fileSystemWatcher->addPath(MTAB);
        }
    }
    if (path == FSTAB) {
        emit fstabChanged();
        if (!m_fileSystemWatcher->files().contains(FSTAB)) {
            m_fileSystemWatcher->addPath(FSTAB);
        }
    }
}

