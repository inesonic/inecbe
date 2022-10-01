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
* This file implements the \ref Cbe::CppCompilerContext class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QByteArray>
#include <QSharedDataPointer>
#include <QSharedData>

#include "cbe_common.h"

#include "cbe_cpp_compiler_context_private.h"
#include "cbe_compiler_context.h"
#include "cbe_cpp_compiler_context.h"

namespace Cbe {
    CppCompilerContext::CppCompilerContext(
            const QString&        newObjectFile,
            const QList<QString>& newPchFiles,
            const QList<QString>& newHeaderFiles
        ):CompilerContext(
            newObjectFile,
            newPchFiles
        ),impl(
            new CppCompilerContext::Private(newHeaderFiles)
        ) {}


    CppCompilerContext::CppCompilerContext(
            const QString&        newObjectFile,
            const QByteArray&     newSourceData,
            const QList<QString>& newPchFiles,
            const QList<QString>& newHeaderFiles
        ):CompilerContext(
            newObjectFile,
            newPchFiles
        ),impl(
            new CppCompilerContext::Private(newSourceData, newHeaderFiles)
        ) {}


    CppCompilerContext::CppCompilerContext(const CppCompilerContext& other):CompilerContext(other) {
        impl = other.impl;
    }


    CppCompilerContext::~CppCompilerContext() {}


    const QByteArray& CppCompilerContext::sourceData() const {
        return impl->sourceData();
    }


    unsigned long CppCompilerContext::byteOffset() const {
        return impl->byteOffset();
    }


    unsigned long CppCompilerContext::lineNumber() const {
        return impl->lineNumber();
    }


    unsigned long CppCompilerContext::columnNumber() const {
        return impl->columnNumber();
    }


    void CppCompilerContext::append(const std::uint8_t data) {
        impl->append(data);
    }


    void CppCompilerContext::append(const QByteArray& data) {
        impl->append(data);
    }


    CppCompilerContext& CppCompilerContext::operator=(const CppCompilerContext& other) {
        CompilerContext::operator=(other);
        impl = other.impl;

        return *this;
    }


    CppCompilerContext& CppCompilerContext::operator<<(char data) {
        impl->append(static_cast<std::uint8_t>(data));
        return *this;
    }


    CppCompilerContext& CppCompilerContext::operator<<(const char* data) {
        impl->append(QByteArray(data));
        return *this;
    }


    CppCompilerContext& CppCompilerContext::operator<<(const QByteArray& data) {
        impl->append(data);
        return *this;
    }


    CppCompilerContext& CppCompilerContext::operator<<(const QString& data) {
        impl->append(data.toLocal8Bit());
        return *this;
    }


    void CppCompilerContext::handleCompilerDiagnostic(const CppCompilerDiagnostic&) {}
}
