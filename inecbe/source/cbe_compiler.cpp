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
* This file implements the \ref Cbe::Compiler class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QByteArray>

#include "cbe_compiler_context.h"
#include "cbe_job_queue.h"
#include "cbe_compiler_notifier.h"
#include "cbe_compiler_private.h"
#include "cbe_compiler.h"

namespace Cbe {
    Compiler::Compiler(CompilerNotifier* newNotifier):impl(new Compiler::Private(newNotifier, this)) {
        if (newNotifier != nullptr) {
            newNotifier->currentCompiler = this;
        }
    }


    Compiler::~Compiler() {
        if (impl->notifier() != nullptr) {
            impl->notifier()->currentCompiler = nullptr;
        }
    }


    void Compiler::setDebugOutputEnabled(bool enableDebugOutput) {
        impl->setDebugOutputEnabled(enableDebugOutput);
    }


    void Compiler::setDebugOutputDisabled(bool disableDebugOutput) {
        setDebugOutputEnabled(!disableDebugOutput);
    }


    bool Compiler::debugOutputEnabled() const {
        return impl->debugOutputEnabled();
    }


    bool Compiler::debugOutputDisabled() const {
        return !debugOutputEnabled();
    }


    void Compiler::setJobQueue(QSharedPointer<JobQueue<CompilerContext> > newJobQueue) {
        impl->setJobQueue(newJobQueue);
    }


    void Compiler::setJobQueue(JobQueue<CompilerContext> *newJobQueue) {
        impl->setJobQueue(QSharedPointer<JobQueue<CompilerContext>>(newJobQueue));
    }


    bool Compiler::isInternal() const {
        return impl->isInternal();
    }


    bool Compiler::isExternal() const {
        return !isInternal();
    }


    void Compiler::setExecutableDirectory(const QString& newExecutableDirectory) {
        impl->setExecutableDirectory(newExecutableDirectory);
    }


    QString Compiler::executableDirectory() const {
        return impl->executableDirectory();
    }


    QList<QString> Compiler::compilerSwitches() const {
        return impl->compilerSwitches();
    }


    void Compiler::setCompilerSwitches(const QList<QString>& newCompilerSwitches) {
        impl->setCompilerSwitches(newCompilerSwitches);
    }


    void Compiler::setSystemRoot(const QString& newSystemRoot) {
        impl->setSystemRoot(newSystemRoot);
    }


    QString Compiler::systemRoot() const {
        return impl->systemRoot();
    }


    QList<QString> Compiler::headerSearchPaths() const {
        return impl->headerSearchPaths();
    }


    void Compiler::setHeaderSearchPaths(const QList<QString>& newHeaderSearchPaths) {
        impl->setHeaderSearchPaths(newHeaderSearchPaths);
    }


    QList<QString> Compiler::headers() const {
        return impl->headers();
    }


    void Compiler::setHeaders(const QList<QString>& newHeaders) {
        impl->setHeaders(newHeaders);
    }


    QList<QString> Compiler::precompiledHeaders() const {
        return impl->precompiledHeaders();
    }


    void Compiler::setPrecompiledHeaders(const QList<QString>& newPrecompiledHeaders) {
        impl->setPrecompiledHeaders(newPrecompiledHeaders);
    }


    QString Compiler::resourceDirectory() const {
        return impl->resourceDirectory();
    }


    void Compiler::setResourceDirectory(const QString& newResourceDirectory) {
        impl->setResourceDirectory(newResourceDirectory);
    }


    QString Compiler::gccToolchain() const {
        return impl->gccToolchain();
    }


    void Compiler::setGccToolchain(const QString& newGccToolchainPrefix) {
        impl->setGccToolchain(newGccToolchainPrefix);
    }


    QString Compiler::targetTriple() const {
        return impl->targetTriple();
    }


    void Compiler::setTargetTriple(const QString& newTargetTriple) {
        impl->setTargetTriple(newTargetTriple);
    }


    void Compiler::compile(QSharedPointer<CompilerContext> context) {
        return impl->compile(context);
    }


    void Compiler::compile(CompilerContext* context) {
        impl->compile(QSharedPointer<CompilerContext>(context));
    }


    void Compiler::waitComplete() {
        impl->waitComplete();
    }


    bool Compiler::waitComplete(unsigned long maximumTimeInMilliseconds) {
        return impl->waitComplete(maximumTimeInMilliseconds);
    }


    bool Compiler::active() const {
        return impl->active();
    }


    void Compiler::setNotifier(CompilerNotifier* newNotifier) {
        if (impl->notifier() != nullptr) {
            impl->notifier()->currentCompiler = nullptr;
        }

        impl->setNotifier(newNotifier);

        if (newNotifier != nullptr) {
            newNotifier->currentCompiler = this;
        }
    }


    CompilerNotifier* Compiler::notifier() const {
        return impl->notifier();
    }
}
