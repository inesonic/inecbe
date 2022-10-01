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
* This file implements the \ref Cbe::LinkerContext::Private class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QSharedData>

#include "cbe_linker_context.h"
#include "cbe_linker_context_private.h"

namespace Cbe {
    LinkerContext::Private::Private(
            const QString&        outputFile,
            const QList<QString>& newObjectFiles,
            const QList<QString>& newStaticLibraries,
            const QList<QString>& newDynamicLibraries
        ) {
        currentOutputFile       = outputFile;
        currentObjectFiles      = newObjectFiles;
        currentStaticLibraries  = newStaticLibraries;
        currentDynamicLibraries = newDynamicLibraries;
    }


    LinkerContext::Private::Private(
            const QString&        outputFile,
            const QString&        objectFile,
            const QList<QString>& newStaticLibraries,
            const QList<QString>& newDynamicLibraries
        ) {
        currentOutputFile       = outputFile;
        currentObjectFiles      = QList<QString>() << objectFile;
        currentStaticLibraries  = newStaticLibraries;
        currentDynamicLibraries = newDynamicLibraries;
    }


    LinkerContext::Private::Private(const LinkerContext::Private& other):QSharedData(other) {
        currentOutputFile       = other.currentOutputFile;
        currentObjectFiles      = other.currentObjectFiles;
        currentStaticLibraries  = other.currentStaticLibraries;
        currentDynamicLibraries = other.currentDynamicLibraries;
    }


    LinkerContext::Private::~Private() {}


    void LinkerContext::Private::setOutputFile(const QString& newOutputFile) {
        currentOutputFile = newOutputFile;
    }


    QString LinkerContext::Private::outputFile() const {
        return currentOutputFile;
    }


    void LinkerContext::Private::setObjectFiles(const QList<QString>& newObjectFiles) {
        currentObjectFiles = newObjectFiles;
    }


    const QList<QString>& LinkerContext::Private::objectFiles() const {
        return currentObjectFiles;
    }


    void LinkerContext::Private::setStaticLibraries(const QList<QString>& newStaticLibraries) {
        currentStaticLibraries = newStaticLibraries;
    }


    const QList<QString>& LinkerContext::Private::staticLibraries() const {
        return currentStaticLibraries;
    }


    void LinkerContext::Private::setDynamicLibraries(const QList<QString>& newDynamicLibraries) {
        currentDynamicLibraries = newDynamicLibraries;
    }


    const QList<QString>& LinkerContext::Private::dynamicLibraries() const {
        return currentDynamicLibraries;
    }
}
