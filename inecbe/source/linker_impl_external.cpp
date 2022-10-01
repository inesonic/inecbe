/*-*-c++-*-*************************************************************************************************************
* Copyright 2016 - 2022 Inesonic, LLC.
* 
* This file is licensed under two licenses.
*
* Inesonic Commercial License, Version 1:
*   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
*   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
*   strictly prohibited.
*
* GNU Public License, Version 2:
*   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
*   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
*   version.
*   
*   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
*   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
*   details.
*   
*   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
*   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************************************************************//**
* \file
*
* This file implements the \ref LinkerImplExternal class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QThread>
#include <QStringList>
#include <QProcess>
#include <QMutex>
#include <QMutexLocker>
#include <QFileInfo>

#if (defined(_WIN32) || defined(_WIN64))

    #include <QDir>
    #include <QFileInfo>

#endif

#include "cbe_linker_notifier.h"
#include "cbe_linker.h"
#include "linker_impl_external.h"

LinkerImplExternal::LinkerImplExternal(
        Cbe::LinkerNotifier* newNotifier
    ):jobQueue(new Cbe::SimpleJobQueue<Cbe::LinkerContext>()
    ),currentNotifier(
        newNotifier
    ) {
    generateDefaultSwitches   = true;
    isTerminatingThread       = false;
    currentDebugOutputEnabled = false;

    currentSystemRoot       = QString("/");

    #if (defined(__APPLE__))

        currentMacOsMajorVersionNumber = Cbe::Linker::defaultMacOsSdkVersionMajor;
        currentMacOsMinorVersionNumber = Cbe::Linker::defaultMacOsSdkVersionMinor;

    #elif (!defined(__linux__) && !defined(_WIN32) && !defined(_WIN64))

        #error Unknown platform

    #endif
}


LinkerImplExternal::~LinkerImplExternal() {}


void LinkerImplExternal::setDebugOutputEnabled(bool enableDebugOutput) {
    currentDebugOutputEnabled = enableDebugOutput;
}


bool LinkerImplExternal::debugOutputEnabled() const {
    return currentDebugOutputEnabled;
}


void LinkerImplExternal::setJobQueue(QSharedPointer<Cbe::JobQueue<Cbe::LinkerContext>> newJobQueue) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    jobQueue = newJobQueue;
}


void LinkerImplExternal::setNotifier(Cbe::LinkerNotifier* newNotifier) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    currentNotifier = newNotifier;
}


Cbe::LinkerNotifier* LinkerImplExternal::notifier() const {
    return currentNotifier;
}


bool LinkerImplExternal::isInternal() const {
    return false;
}


void LinkerImplExternal::setExecutableDirectory(const QString& newExecutableDirectory) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    currentExecutableDirectory = newExecutableDirectory;
}


QString LinkerImplExternal::executableDirectory() const {
    return currentExecutableDirectory;
}


void LinkerImplExternal::setLinkerExecutable(const QString& linkerExecutable) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    currentLinkerExecutable = linkerExecutable;
}


QString LinkerImplExternal::linkerExecutable() const {
    QString result;

    if (currentLinkerExecutable.isEmpty()) {
        #if (defined(_WIN32) || defined(_WIN64))

             result = QString("lld-link.exe");

        #elif (defined(__APPLE__))

            result = QString("lld");

        #elif (defined(__linux__))

            result = QString("lld");

        #else

            #error Unknown platform

        #endif
    } else {
        result = currentLinkerExecutable;
    }

    return result;
}


QList<QString> LinkerImplExternal::linkerSwitches() {
    if (generateDefaultSwitches) {
        generateDefaultSwitches = false;
        currentSwitches = setDefaultSwitches();
    }

    return currentSwitches;
}


void LinkerImplExternal::setLinkerSwitches(const QList<QString>& newLinkerSwitches) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    currentSwitches = newLinkerSwitches;
}


void LinkerImplExternal::setSystemRoot(const QString& newSystemRoot) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    currentSystemRoot = newSystemRoot;
}


QString LinkerImplExternal::systemRoot() const {
    return currentSystemRoot;
}


QList<QString> LinkerImplExternal::librarySearchPaths() const {
    return currentLibrarySearchPaths;
}


void LinkerImplExternal::setLibrarySearchPaths(const QList<QString>& newLibrarySearchPaths) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    if (currentLibrarySearchPaths != newLibrarySearchPaths) {
        currentLibrarySearchPaths = newLibrarySearchPaths;
        generateDefaultSwitches   = true;
    }
}


QList<QString> LinkerImplExternal::staticLibraries() const {
    return currentStaticLibraries;
}


void LinkerImplExternal::setStaticLibraries(const QList<QString>& newStaticLibraries) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    if (currentStaticLibraries != newStaticLibraries) {
        currentStaticLibraries  = newStaticLibraries;
        generateDefaultSwitches = true;
    }
}


QList<QString> LinkerImplExternal::dynamicLibraries() const {
    return currentDynamicLibraries;
}


void LinkerImplExternal::setDynamicLibraries(const QList<QString>& newDynamicLibraries) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    if (currentDynamicLibraries != newDynamicLibraries) {
        currentDynamicLibraries = newDynamicLibraries;
        generateDefaultSwitches = true;
    }
}


void LinkerImplExternal::setMacOsSdkVersion(unsigned majorVersionNumber, unsigned minorVersionNumber) {
    #if (defined(__APPLE__))

        currentMacOsMajorVersionNumber = majorVersionNumber;
        currentMacOsMinorVersionNumber = minorVersionNumber;

    #elif (defined(__linux__) || defined(_WIN32) || defined(_WIN64))

        (void) majorVersionNumber;
        (void) minorVersionNumber;

    #else

        #error Unknown platform.

    #endif
}


unsigned LinkerImplExternal::macOsSdkMajorVersion() const {
    #if (defined(__APPLE__))

        return currentMacOsMajorVersionNumber;

    #elif (defined(__linux__) || defined(_WIN32) || defined(_WIN64))

        return 0;

    #else

        #error Unknown platform.

    #endif
}


unsigned LinkerImplExternal::macOsSdkMinorVersion() const {
    #if (defined(__APPLE__))

        return currentMacOsMinorVersionNumber;

    #elif (defined(__linux__) || defined(_WIN32) || defined(_WIN64))

        return 0;

    #else

        #error Unknown platform.

    #endif
}


QList<QString> LinkerImplExternal::runTimeSearchPaths() const {
    #if (defined(__APPLE__) || defined(__linux__))

        return currentRunTimeSearchPaths;

    #elif (defined(_WIN32) || defined(_WIN64))

        return QList<QString>();

    #else

        #error Unknown platform.

    #endif
}


void LinkerImplExternal::setRunTimeSearchPaths(const QList<QString>& newRunTimeSearchPaths) {
    #if (defined(__APPLE__) || defined(__linux__))

        currentRunTimeSearchPaths = newRunTimeSearchPaths;

    #elif (defined(_WIN32) || defined(_WIN64))

        (void) newRunTimeSearchPaths;

    #else

        #error Unknown platform.

    #endif
}


void LinkerImplExternal::link(QSharedPointer<Cbe::LinkerContext> context) {
    jobQueueMutex.lock();
    jobQueue->enqueue(context);
    jobQueueMutex.unlock();

    if (isTerminatingThread) {
        wait();
    }

    if (!isRunning()) {
        start();
    }
}


void LinkerImplExternal::waitComplete() {
    wait();
}


bool LinkerImplExternal::waitComplete(unsigned long maximumTimeInMilliseconds) {
    return wait(maximumTimeInMilliseconds);
}


bool LinkerImplExternal::active() const {
    return isRunning();
}


void LinkerImplExternal::setSystemLibraries(const QList<QString>& systemLibraries) {
    currentSystemLibraries = systemLibraries;
}


QList<QString> LinkerImplExternal::systemLibraries() const {
    QList<QString> result;

    if (currentSystemLibraries.isEmpty()) {
        #if (defined(Q_OS_WIN))

            // Windows does not directly reference system libraries.

        #elif (defined(Q_OS_LINUX))

            result << "/lib64/ld-linux-x86-64.so.2";

        #elif (defined(Q_OS_DARWIN))

            result << "/usr/lib/libSystem.dylib";

        #else

            #error Unknown platform

        #endif
    } else {
        result = currentSystemLibraries;
    }

    return result;
}


void LinkerImplExternal::run() {
    QMutexLocker mutexLocker(&linkerAccessMutex);

    QProcess linkerExecutable;
    linkerExecutable.setProcessChannelMode(QProcess::MergedChannels);

    do {
        jobQueueMutex.lock();
        activeContext = jobQueue->dequeue();
        isTerminatingThread = activeContext.isNull();
        jobQueueMutex.unlock();

        if (!isTerminatingThread) {
            QStringList switches = buildLinkerCommandLine(activeContext);

            linkerStarted(activeContext);
            linkerExecutable.start(switches.first(), switches.mid(1));

            QString errorString;
            if (!linkerExecutable.waitForFinished(maximumLinkerExecutionTime)) {
                QProcess::ProcessError errorCode = linkerExecutable.error();
                QString errorReason;

                switch (errorCode) {
                    case QProcess::FailedToStart: {
                        errorReason = QString("Linker failed to start");
                        break;
                    }

                    case QProcess::Crashed: {
                        errorReason = QString("Linker crashed");
                        break;
                    }

                    case QProcess::Timedout: {
                        errorReason = QString("Linker timed out");
                        break;
                    }

                    case QProcess::WriteError: {
                        errorReason = QString("Write error");
                        break;
                    }

                    case QProcess::ReadError: {
                        errorReason = QString("Read error");
                        break;
                    }

                    case QProcess::UnknownError:  {
                        errorReason = QString("Unknown error");
                        break;
                    }

                    default: {
                        errorReason = QString("Unexpected error code %1").arg(errorCode);
                        break;
                    }
                }

                QString linkerOutput = QString::fromLocal8Bit(linkerExecutable.readAll()).trimmed();

                if (linkerOutput.isEmpty()) {
                    errorString = errorReason;
                } else {
                    errorString = QString("%1\n%2").arg(errorReason, linkerOutput);
                }
            } else if (linkerExecutable.exitStatus() != QProcess::NormalExit || linkerExecutable.exitCode() != 0) {
                errorString = QString::fromLocal8Bit(linkerExecutable.readAll()).trimmed();
            }

            if (!errorString.isEmpty()) {
                handleLinkerDiagnostic(activeContext, errorString);
            }

            linkerFinished(activeContext, errorString.isEmpty());

            activeContext.clear();
        }
    } while (!isTerminatingThread);
}


QStringList LinkerImplExternal::addOptions(
        QList<QString>&       switchList,
        const QList<QString>& parameters,
        const QString&        switchText,
        const QString&        extension,
        const QString&        prefix
    ) {
    #if (defined(_WIN32) || defined(_WIN64))

        QDir systemRootDirectory(currentSystemRoot);

        for(QList<QString>::const_iterator it=parameters.constBegin(),end=parameters.constEnd() ; it!=end ; ++it) {
            QFileInfo fileInformation(*it);
            QString   switchParameter;

            if (fileInformation.exists()) {
                switchParameter = *it;
            } else {
                switchParameter = QDir::cleanPath(systemRootDirectory.filePath(*it));
            }

            switchList << switchText + prefix + switchParameter + extension;
        }

    #elif (defined(__APPLE__) || defined(__linux__))

        for(QList<QString>::const_iterator it=parameters.constBegin(),end=parameters.constEnd() ; it!=end ; ++it) {
            switchList << switchText << (prefix + *it + extension);
        }

    #else

        #error Unknown platform

    #endif

    return switchList;
}


QStringList LinkerImplExternal::buildLinkerCommandLine(QSharedPointer<Cbe::LinkerContext> context) {
    if (generateDefaultSwitches) {
        generateDefaultSwitches = false;
        currentSwitches.clear();

        currentSwitches << currentExecutableDirectory + "/" + linkerExecutable();

        #if (defined(_WIN32) || defined(_WIN64))

            currentSwitches += setDefaultSwitches();

            addOptions(
                currentSwitches,
                locateLibraries(currentLibrarySearchPaths, currentStaticLibraries, QString(".lib")),
                QString()
            );

            addOptions(
                currentSwitches,
                locateLibraries(currentLibrarySearchPaths, currentDynamicLibraries, QString(".lib")),
                QString()
            );

        #elif (defined(__APPLE__))

            currentSwitches += setDefaultSwitches();
            currentSwitches << "-arch" << "x86_64"
                            << "-L/usr/lib"
                            << "-lc++";

            for (  QList<QString>::const_iterator
                       runTimeSearchPathIterator    = currentRunTimeSearchPaths.constBegin(),
                       runTimeSearchPathEndIterator = currentRunTimeSearchPaths.constEnd()
                 ; runTimeSearchPathIterator != runTimeSearchPathEndIterator
                 ; ++runTimeSearchPathIterator
                ) {
                currentSwitches << QString("-Wl,-rpath,%1").arg(*runTimeSearchPathIterator);
            }

        #elif (defined(__linux__))

            currentSwitches << "-flavor" << "gnu";

            currentSwitches += setDefaultSwitches();

            if (!currentSystemRoot.isEmpty()) {
                currentSwitches << "--sysroot=" + currentSystemRoot;
            }

            for (  QList<QString>::const_iterator
                       runTimeSearchPathIterator    = currentRunTimeSearchPaths.constBegin(),
                       runTimeSearchPathEndIterator = currentRunTimeSearchPaths.constEnd()
                 ; runTimeSearchPathIterator != runTimeSearchPathEndIterator
                 ; ++runTimeSearchPathIterator
                ) {
                currentSwitches << "--rpath";
                currentSwitches << *runTimeSearchPathIterator;
            }

        #else

            #error Unknown platform

        #endif

        #if (defined(__APPLE__) || defined(__linux__))

            for (  QList<QString>::const_iterator libraryPathIterator    = currentLibrarySearchPaths.constBegin(),
                                                  libraryPathEndIterator = currentLibrarySearchPaths.constEnd()
                 ; libraryPathIterator != libraryPathEndIterator
                 ; ++libraryPathIterator
                ) {
                currentSwitches.append(QString("-L%1").arg(*libraryPathIterator));
            }

            for (  QList<QString>::const_iterator libraryIterator    = currentDynamicLibraries.constBegin(),
                                                  libraryEndIterator = currentDynamicLibraries.constEnd()
                 ; libraryIterator != libraryEndIterator
                 ; ++libraryIterator
                ) {
                QString libraryName = *libraryIterator;
                if (libraryName.startsWith("/")) {
                    currentSwitches.append(libraryName);
                } else {
                    currentSwitches.append(QString("-l%1").arg(libraryName));
                }
            }

            for (  QList<QString>::const_iterator libraryIterator    = currentStaticLibraries.constBegin(),
                                                  libraryEndIterator = currentStaticLibraries.constEnd()
                 ; libraryIterator != libraryEndIterator
                 ; ++libraryIterator
                ) {
                QString libraryName = *libraryIterator;
                if (libraryName.startsWith("/")) {
                    currentSwitches.append(libraryName);
                } else {
                    currentSwitches.append(QString("-l%1").arg(libraryName));
                }
            }

        #elif (!defined(_WIN32) && !defined(_WIN64))

            #error Unknown platform

        #endif
    }

    QStringList switches(currentSwitches);

    #if (defined(_WIN32) || defined(_WIN64))

        switches += QString("/out:%1").arg(context->outputFile());
        switches += context->objectFiles();

        addOptions(
            switches,
            locateLibraries(currentLibrarySearchPaths, context->staticLibraries(), QString(".lib")),
            QString()
        );

        addOptions(
            switches,
            locateLibraries(currentLibrarySearchPaths, context->dynamicLibraries(), QString(".lib")),
            QString()
        );

    #elif (defined(__APPLE__) || defined(__linux__))

        QList<QString> contextDynamicLibraries = context->dynamicLibraries();
        for (  QList<QString>::const_iterator libraryIterator    = contextDynamicLibraries.constBegin(),
                                              libraryEndIterator = contextDynamicLibraries.constEnd()
             ; libraryIterator != libraryEndIterator
             ; ++libraryIterator
            ) {
            QString libraryName = *libraryIterator;
            if (libraryName.startsWith("/")) {
                switches.append(libraryName);
            } else {
                switches.append(QString("-l%1").arg(libraryName));
            }
        }

        QList<QString> contextStaticLibraries = context->staticLibraries();
        for (  QList<QString>::const_iterator libraryIterator    = contextStaticLibraries.constBegin(),
                                              libraryEndIterator = contextStaticLibraries.constEnd()
             ; libraryIterator != libraryEndIterator
             ; ++libraryIterator
            ) {
            QString libraryName = *libraryIterator;
            if (libraryName.startsWith("/")) {
                switches.append(libraryName);
            } else {
                switches.append(QString("-l%1").arg(libraryName));
            }
        }

        switches += context->objectFiles();

        switches << "-o" << context->outputFile();

    #else

        #error Unknown platform

    #endif

    return switches;
}


QStringList LinkerImplExternal::locateLibraries(
        const QStringList& searchPaths,
        const QStringList& libraries,
        const QString&     extension
    ) {
    QStringList result;

    for (  QStringList::const_iterator libraryIterator    = libraries.constBegin(),
                                       libraryEndIterator = libraries.constEnd()
         ; libraryIterator != libraryEndIterator
         ; ++libraryIterator
        ) {
        const QString& libraryName = *libraryIterator;
        QFileInfo      libraryFileInfo(libraryName);

        if (libraryFileInfo.exists() && libraryFileInfo.isFile()) {
            result << libraryFileInfo.canonicalFilePath();
        } else {
            QStringList::const_iterator pathIterator    = searchPaths.constBegin();
            QStringList::const_iterator pathEndIterator = searchPaths.constEnd();

            while (pathIterator != pathEndIterator) {
                const QString& path = *pathIterator;
                QFileInfo libraryFileInfo(path + "/" + libraryName + extension);
                if (libraryFileInfo.exists() && libraryFileInfo.isFile()) {
                    result << libraryFileInfo.canonicalFilePath();
                    pathIterator = pathEndIterator;
                } else {
                    ++pathIterator;
                }
            }
        }
    }

    return result;
}
