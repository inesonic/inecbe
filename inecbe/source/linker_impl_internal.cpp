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
* This file implements the \ref LinkerImplInternal class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#if (defined(_WIN32) || defined(_WIN64))

    #include <QDir>
    #include <QFileInfo>

#endif

#include <string>

#include "warnings.h"

SUPPRESS_LLVM_WARNINGS

#include <llvm/ADT/APInt.h>
#include <llvm/Support/raw_ostream.h>

#if (LLVM_VERSION == 100000 || LLVM_VERSION == 80000)

    #include <lld/Common/Driver.h>

#elif (LLVM_VERSION == 50001)

    #include <lld/Driver/Driver.h>

#else

    #error Unknown LLVM version

#endif

RESTORE_LLVM_WARNINGS

#include "cbe_job_queue.h"
#include "cbe_linker_context.h"
#include "cbe_linker_notifier.h"
#include "cbe_linker.h"
#include "linker_impl_internal.h"

LinkerImplInternal::LinkerImplInternal(
        Cbe::LinkerNotifier* newNotifier
    ):jobQueue(new Cbe::SimpleJobQueue<Cbe::LinkerContext>()
    ),currentNotifier(
        newNotifier
    ) {
    generateDefaultSwitches   = true;
    isTerminatingThread       = false;
    currentDebugOutputEnabled = false;

    currentSystemRoot = QString("/");

    #if (defined(__APPLE__))

        currentMacOsMajorVersionNumber = Cbe::Linker::defaultMacOsSdkVersionMajor;
        currentMacOsMinorVersionNumber = Cbe::Linker::defaultMacOsSdkVersionMinor;

    #elif (!defined(__linux__) && !defined(_WIN32) && !defined(_WIN64))

        #error Unknown platform

    #endif
}


LinkerImplInternal::~LinkerImplInternal() {}


void LinkerImplInternal::setDebugOutputEnabled(bool enableDebugOutput) {
    currentDebugOutputEnabled = enableDebugOutput;
}


bool LinkerImplInternal::debugOutputEnabled() const {
    return currentDebugOutputEnabled;
}


void LinkerImplInternal::setJobQueue(QSharedPointer<Cbe::JobQueue<Cbe::LinkerContext>> newJobQueue) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    jobQueue = newJobQueue;
}


void LinkerImplInternal::setNotifier(Cbe::LinkerNotifier* newNotifier) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    currentNotifier = newNotifier;
}


Cbe::LinkerNotifier* LinkerImplInternal::notifier() const {
    return currentNotifier;
}


bool LinkerImplInternal::isInternal() const {
    return true;
}


void LinkerImplInternal::setExecutableDirectory(const QString&) {}


QString LinkerImplInternal::executableDirectory() const {
    return QString();
}

void LinkerImplInternal::setLinkerExecutable(const QString&) {}


QString LinkerImplInternal::linkerExecutable() {
    return QString();
}


QList<QString> LinkerImplInternal::linkerSwitches() {
    if (generateDefaultSwitches) {
        generateDefaultSwitches = false;
        currentSwitches = setDefaultSwitches();
    }

    return currentSwitches;
}


void LinkerImplInternal::setLinkerSwitches(const QList<QString>& newLinkerSwitches) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    currentSwitches         = newLinkerSwitches;
    generateDefaultSwitches = false;
}


void LinkerImplInternal::setSystemRoot(const QString& newSystemRoot) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    currentSystemRoot = newSystemRoot;
}


QString LinkerImplInternal::systemRoot() const {
    return currentSystemRoot;
}


QList<QString> LinkerImplInternal::librarySearchPaths() const {
    return currentLibrarySearchPaths;
}


void LinkerImplInternal::setLibrarySearchPaths(const QList<QString>& newLibrarySearchPaths) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    currentLibrarySearchPaths = newLibrarySearchPaths;
}


QList<QString> LinkerImplInternal::staticLibraries() const {
    return currentStaticLibraries;
}


void LinkerImplInternal::setStaticLibraries(const QList<QString>& newStaticLibraries) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    currentStaticLibraries = newStaticLibraries;
}


QList<QString> LinkerImplInternal::dynamicLibraries() const {
    return currentDynamicLibraries;
}


void LinkerImplInternal::setDynamicLibraries(const QList<QString>& newDynamicLibraries) {
    QMutexLocker mutexLocker(&linkerAccessMutex);
    currentDynamicLibraries = newDynamicLibraries;
}


void LinkerImplInternal::setMacOsSdkVersion(unsigned majorVersionNumber, unsigned minorVersionNumber) {
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


unsigned LinkerImplInternal::macOsSdkMajorVersion() const {
    #if (defined(__APPLE__))

        return currentMacOsMajorVersionNumber;

    #elif (defined(__linux__) || defined(_WIN32) || defined(_WIN64))

        return 0;

    #else

        #error Unknown platform.

    #endif
}


unsigned LinkerImplInternal::macOsSdkMinorVersion() const {
    #if (defined(__APPLE__))

        return currentMacOsMinorVersionNumber;

    #elif (defined(__linux__) || defined(_WIN32) || defined(_WIN64))

        return 0;

    #else

        #error Unknown platform.

    #endif
}


QList<QString> LinkerImplInternal::runTimeSearchPaths() const {
    #if (defined(__APPLE__) || defined(__linux__))

        return currentRunTimeSearchPaths;

    #elif (defined(_WIN32) || defined(_WIN64))

        return QList<QString>();

    #else

        #error Unknown platform.

    #endif
}


void LinkerImplInternal::setRunTimeSearchPaths(const QList<QString>& newRunTimeSearchPaths) {
    #if (defined(__APPLE__) || defined(__linux__))

        currentRunTimeSearchPaths = newRunTimeSearchPaths;

    #elif (defined(_WIN32) || defined(_WIN64))

        (void) newRunTimeSearchPaths;

    #else

        #error Unknown platform.

    #endif
}


void LinkerImplInternal::link(QSharedPointer<Cbe::LinkerContext> context) {
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


void LinkerImplInternal::waitComplete() {
    wait();
}


bool LinkerImplInternal::waitComplete(unsigned long maximumTimeInMilliseconds) {
    return wait(maximumTimeInMilliseconds);
}


bool LinkerImplInternal::active() const {
    return isRunning();
}


void LinkerImplInternal::setSystemLibraries(const QList<QString>& systemLibraries) {
    currentSystemLibraries = systemLibraries;
}


QList<QString> LinkerImplInternal::systemLibraries() const {
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


void LinkerImplInternal::run() {
    QMutexLocker mutexLocker(&linkerAccessMutex);

    do {
        jobQueueMutex.lock();
        activeContext = jobQueue->dequeue();
        isTerminatingThread = activeContext.isNull();
        jobQueueMutex.unlock();

        if (!isTerminatingThread) {
            buildUserFriendlyCommandLine(activeContext);

            std::string errorString;
            llvm::raw_string_ostream errorStream(errorString);

            char**   linkerOptions = currentLinkerSwitches.data();
            unsigned numberSwitches  = currentLinkerSwitches.size();

            linkerStarted(activeContext);

            std::string linkerOutput;
            llvm::raw_string_ostream linkerOutputStream(linkerOutput);

            #if (defined(_WIN32) || defined(_WIN64))

                bool success = lld::coff::link(
                    llvm::makeArrayRef(linkerOptions, numberSwitches),
                    false,
                    linkerOutputStream,
                    errorStream
                );

            #elif (defined(__APPLE__))
                #if (LLVM_VERSION == 100000)

                    bool success = lld::mach_o::link(
                        llvm::makeArrayRef(linkerOptions, numberSwitches),
                        false,
                        linkerOutputStream,
                        errorStream
                    );

                #elif (LLVM_VERSION == 80000)

                    bool success = lld::mach_o::link(
                        llvm::makeArrayRef(linkerOptions, numberSwitches),
                        false,
                        errorStream
                    );

                #elif (LLVM_VERSION == 50001)

                    bool success = lld::mach_o::link(llvm::makeArrayRef(linkerOptions, numberSwitches), errorStream);

                #else

                    #error "Unknown LLVM version"

                #endif
            #elif (defined(__linux__))

                // Appears that canExitEarly causes the linker to directly call exit(x) rather than return gracefully.
                bool success = lld::elf::link(llvm::makeArrayRef(linkerOptions, numberSwitches), false, errorStream);

            #else

                #error Unknown platform

            #endif

            // TODO: Do we want to do anything with the linker output here ?

            if (!success) {
                handleLinkerDiagnostic(activeContext, QString::fromStdString(errorStream.str()));
            }

            linkerFinished(activeContext, success);

            activeContext.clear();
        }
    } while (!isTerminatingThread);
}


QList<QString> LinkerImplInternal::addOptions(
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
            switchList << switchText + (prefix + *it + extension);
        }

    #else

        #error Unknown platform

    #endif

    return switchList;
}


void LinkerImplInternal::buildUserFriendlyCommandLine(QSharedPointer<Cbe::LinkerContext> context) {
    clearUserFriendlyCommandLine();

    if (generateDefaultSwitches) {
        currentSwitches.clear();
        generateDefaultSwitches = false;

        #if (defined(_WIN32) || defined(_WIN64))

            currentSwitches << "";
            currentSwitches += setDefaultSwitches();
            addOptions(currentSwitches, currentLibrarySearchPaths, QString("/libpath:"));

            addOptions(currentSwitches, currentStaticLibraries, QString(), QString(".lib"));
            addOptions(currentSwitches, currentDynamicLibraries, QString(), QString(".lib"));

        #elif (defined(__APPLE__))

            currentSwitches << "";
            currentSwitches += setDefaultSwitches();

            if (!currentSystemRoot.isEmpty()) {
                currentSwitches << "-syslibroot" << currentSystemRoot;
            }

            currentSwitches << "-sdk_version"
                            << QString("%1.%2")
                               .arg(currentMacOsMajorVersionNumber)
                               .arg(currentMacOsMinorVersionNumber);

            for (  QList<QString>::const_iterator
                       runTimeSearchPathIterator    = currentRunTimeSearchPaths.constBegin(),
                       runTimeSearchPathEndIterator = currentRunTimeSearchPaths.constEnd()
                 ; runTimeSearchPathIterator != runTimeSearchPathEndIterator
                 ; ++runTimeSearchPathIterator
                ) {
                currentSwitches << "-rpath";
                currentSwitches << *runTimeSearchPathIterator;
            }

            addOptions(currentSwitches, currentLibrarySearchPaths, QString("-L"));
            addOptions(currentSwitches, currentDynamicLibraries, QString("-l"));
            addOptions(currentSwitches, currentStaticLibraries, QString("-l"));

        #elif (defined(__linux__))

            currentSwitches << "";
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

            addOptions(currentSwitches, currentLibrarySearchPaths, QString("-L"));
            addOptions(switches, currentStaticLibraries, QString("-l"));
            addOptions(switches, currentDynamicLibraries, QString("-l"));

        #else

            #error Unknown platform

        #endif
    }

    QList<QString> switches(currentSwitches);

    #if (defined(_WIN32) || defined(_WIN64))

        switches += QString("/out:%1").arg(context->outputFile());
        switches += context->objectFiles();

        addOptions(switches, context->staticLibraries(), QString(), QString(".lib"));
        addOptions(switches, context->dynamicLibraries(), QString(), QString(".lib"));

    #elif (defined(__APPLE__))

        addOptions(switches, context->dynamicLibraries(), QString("-l"));
        addOptions(switches, context->staticLibraries(), QString("-l"));

        switches += context->objectFiles();

        switches << "-o" << context->outputFile();

    #elif (defined(__linux__))

        addOptions(switches, context->dynamicLibraries(), QString("-l"));
        addOptions(switches, context->staticLibraries(), QString("-l"));

        switches += context->objectFiles();

        switches << "-o" << context->outputFile();

    #else

        #error Unknown platform

    #endif

    for (QList<QString>::const_iterator it=switches.begin(),end=switches.end() ; it!=end ; ++it) {
        currentLinkerSwitches.push_back(strdup(it->toLocal8Bit().data()));
    }
}


void LinkerImplInternal::clearUserFriendlyCommandLine() {
    QVector<char*>::iterator it  = currentLinkerSwitches.begin();
    QVector<char*>::iterator end = currentLinkerSwitches.end();

    while (it != end) {
        std::free(*it);
        ++it;
    }

    currentLinkerSwitches.clear();
}
