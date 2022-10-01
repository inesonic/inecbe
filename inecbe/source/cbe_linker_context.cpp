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
* This file implements the \ref Cbe::LinkerContext class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QSharedDataPointer>
#include <QSharedData>

#include "cbe_common.h"
#include "cbe_linker_context_private.h"
#include "cbe_linker_context.h"

namespace Cbe {
    LinkerContext::LinkerContext(
            const QString&        outputFile,
            const QList<QString>& newObjectFiles,
            const QList<QString>& newStaticLibraries,
            const QList<QString>& newDynamicLibraries
        ):impl(new LinkerContext::Private(
            outputFile,
            newObjectFiles,
            newStaticLibraries,
            newDynamicLibraries
        )) {}


    LinkerContext::LinkerContext(
            const QString&        outputFile,
            const QString&        objectFile,
            const QList<QString>& newStaticLibraries,
            const QList<QString>& newDynamicLibraries
        ):impl(new LinkerContext::Private(
            outputFile,
            objectFile,
            newStaticLibraries,
            newDynamicLibraries
        )) {}


    LinkerContext::LinkerContext(const LinkerContext& other) {
        impl = other.impl;
    }


    LinkerContext::~LinkerContext() {}


    void LinkerContext::setOutputFile(const QString& newOutputFile) {
        impl->setOutputFile(newOutputFile);
    }


    QString LinkerContext::outputFile() const {
        return impl->outputFile();
    }


    void LinkerContext::setObjectFiles(const QList<QString>& newObjectFiles) {
        impl->setObjectFiles(newObjectFiles);
    }


    void LinkerContext::setObjectFile(const QString& newObjectFile) {
        impl->setObjectFiles(QList<QString>() << newObjectFile);
    }


    QList<QString> LinkerContext::objectFiles() const {
        return impl->objectFiles();
    }


    void LinkerContext::setStaticLibraries(const QList<QString>& newStaticLibraries) {
        impl->setStaticLibraries(newStaticLibraries);
    }


    QList<QString> LinkerContext::staticLibraries() const {
        return impl->staticLibraries();
    }


    void LinkerContext::setDynamicLibraries(const QList<QString>& newDynamicLibraries) {
        impl->setDynamicLibraries(newDynamicLibraries);
    }


    QList<QString> LinkerContext::dynamicLibraries() const {
        return impl->dynamicLibraries();
    }


    LinkerContext& LinkerContext::operator=(const LinkerContext& other) {
        impl = other.impl;
        return *this;
    }


    void LinkerContext::linkerStarted() {}


    void LinkerContext::linkerFinished(bool) {}


    void LinkerContext::handleLinkerDiagnostic(const QString &) {}
}
