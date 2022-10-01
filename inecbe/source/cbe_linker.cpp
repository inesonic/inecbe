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
* This file implements the \ref Cbe::Linker class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QSharedPointer>

#include "cbe_linker_notifier.h"
#include "cbe_linker_context.h"
#include "cbe_linker_private.h"
#include "cbe_linker.h"

namespace Cbe {
    Linker::Linker(LinkerNotifier* newNotifier):impl(new Linker::Private(newNotifier, this)) {
        if (newNotifier != nullptr) {
            newNotifier->currentLinker = this;
        }
    }


    Linker::~Linker() {
        if (impl->notifier() != nullptr) {
            impl->notifier()->currentLinker = nullptr;
        }
    }


    void Linker::setDebugOutputEnabled(bool enableDebugOutput) {
        impl->setDebugOutputEnabled(enableDebugOutput);
    }


    void Linker::setDebugOutputDisabled(bool disableDebugOutput) {
        setDebugOutputEnabled(!disableDebugOutput);
    }


    bool Linker::debugOutputEnabled() const {
        return impl->debugOutputEnabled();
    }


    bool Linker::debugOutputDisabled() const {
        return !debugOutputEnabled();
    }


    void Linker::setJobQueue(QSharedPointer<JobQueue<LinkerContext>> newJobQueue) {
        impl->setJobQueue(newJobQueue);
    }


    void Linker::setJobQueue(JobQueue<LinkerContext>* newJobQueue) {
        impl->setJobQueue(QSharedPointer<JobQueue<LinkerContext>>(newJobQueue));
    }


    void Linker::setNotifier(LinkerNotifier* newNotifier) {
        if (impl->notifier() != nullptr) {
            impl->notifier()->currentLinker = nullptr;
        }

        impl->setNotifier(newNotifier);

        if (newNotifier != nullptr) {
            newNotifier->currentLinker = this;
        }
    }


    LinkerNotifier* Linker::notifier() const {
        return impl->notifier();
    }


    bool Linker::isInternal() const {
        return impl->isInternal();
    }


    bool Linker::isExternal() const {
        return !isInternal();
    }


    void Linker::setExecutableDirectory(const QString& newExecutableDirectory) {
        impl->setExecutableDirectory(newExecutableDirectory);
    }


    QString Linker::executableDirectory() const {
        return impl->executableDirectory();
    }


    void Linker::setLinkerExecutable(const QString& linkerExecutable) {
        impl->setLinkerExecutable(linkerExecutable);
    }


    QString Linker::linkerExecutable() const {
        return impl->linkerExecutable();
    }


    QList<QString> Linker::linkerSwitches() const {
        return impl->linkerSwitches();
    }


    void Linker::setLinkerSwitches(const QList<QString>& newLinkerSwitches) {
        impl->setLinkerSwitches(newLinkerSwitches);
    }


    void Linker::setSystemRoot(const QString& newSystemRoot) {
        impl->setSystemRoot(newSystemRoot);
    }


    QString Linker::systemRoot() const {
        return impl->systemRoot();
    }


    QList<QString> Linker::librarySearchPaths() const {
        return impl->librarySearchPaths();
    }


    void Linker::setLibrarySearchPaths(const QList<QString>& newLibrarySearchPaths) {
        impl->setLibrarySearchPaths(newLibrarySearchPaths);
    }


    QList<QString> Linker::staticLibraries() const {
        return impl->staticLibraries();
    }


    void Linker::setStaticLibraries(const QList<QString>& newStaticLibraries) {
        impl->setStaticLibraries(newStaticLibraries);
    }


    QList<QString> Linker::dynamicLibraries() const {
        return impl->dynamicLibraries();
    }


    void Linker::setDynamicLibraries(const QList<QString>& newDynamicLibraries) {
        impl->setDynamicLibraries(newDynamicLibraries);
    }


    void Linker::setMacOsSdkVersion(unsigned majorVersionNumber, unsigned minorVersionNumber) {
        impl->setMacOsSdkVersion(majorVersionNumber, minorVersionNumber);
    }


    unsigned Linker::macOsSdkMajorVersion() const {
        return impl->macOsSdkMajorVersion();
    }


    unsigned Linker::macOsSdkMinorVersion() const {
        return impl->macOsSdkMinorVersion();
    }


    QList<QString> Linker::runTimeSearchPaths() const {
        return impl->runTimeSearchPaths();
    }


    void Linker::setRunTimeSearchPaths(const QList<QString>& newRunTimeSearchPaths) {
        return impl->setRunTimeSearchPaths(newRunTimeSearchPaths);
    }


    void Linker::link(QSharedPointer<LinkerContext> context) {
        impl->link(context);
    }


    void Linker::link(LinkerContext* context) {
        impl->link(QSharedPointer<LinkerContext>(context));
    }


    void Linker::waitComplete() {
        impl->waitComplete();
    }


    bool Linker::waitComplete(unsigned long maximumTimeInMilliseconds) {
        return impl->waitComplete(maximumTimeInMilliseconds);
    }


    bool Linker::active() const {
        return impl->active();
    }


    void Linker::setSystemLibraries(const QList<QString>& systemLibraries) {
        impl->setSystemLibraries(systemLibraries);
    }


    QList<QString> Linker::systemLibraries() const {
        return impl->systemLibraries();
    }


    QList<QString> Linker::setDefaultSwitches() const {
        return QList<QString>(); // FIXME
    }


    void Linker::linkerStarted(QSharedPointer<LinkerContext> context) {
        if (notifier() != nullptr) {
            notifier()->linkerStarted(context);
        }

        context->linkerStarted();
    }


    void Linker::linkerFinished(QSharedPointer<LinkerContext> context, bool success) {
        context->linkerFinished(success);

        if (notifier() != nullptr) {
            notifier()->linkerFinished(context, success);
        }
    }


    void Linker::handleLinkerDiagnostic(QSharedPointer<LinkerContext> context, const QString& diagnosticMessage) {
        if (notifier() != nullptr) {
            notifier()->handleLinkerDiagnostic(context, diagnosticMessage);
        }

        context->handleLinkerDiagnostic(diagnosticMessage);
    }
}
