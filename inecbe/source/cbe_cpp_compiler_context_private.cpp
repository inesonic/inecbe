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
* This file implements the \ref Cbe::CppCompilerContext::Private class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QByteArray>
#include <QSharedDataPointer>
#include <QSharedData>

#include "cbe_common.h"

#include "cbe_compiler_context.h"
#include "cbe_cpp_compiler_context.h"
#include "cbe_cpp_compiler_context_private.h"

namespace Cbe {
    CppCompilerContext::Private::Private(const QList<QString>& newHeaderFiles) {
        currentHeaderFiles  = newHeaderFiles;
        currentLineNumber   = 1;
        currentColumnNumber = 0;
    }


    CppCompilerContext::Private::Private(const QByteArray& newSourceData,const QList<QString>& newHeaderFiles) {
        rawData             = newSourceData;
        currentHeaderFiles  = newHeaderFiles;

        currentLineNumber = newSourceData.count('\n') + 1;
        if (currentLineNumber == 1) {
            currentColumnNumber = newSourceData.size();
        } else {
            currentColumnNumber = newSourceData.size() - newSourceData.lastIndexOf('\n') - 1;
        }
    }


    CppCompilerContext::Private::Private(const CppCompilerContext::Private& other):QSharedData(other) {
        rawData             = other.rawData;
        currentHeaderFiles  = other.currentHeaderFiles;
        currentLineNumber   = other.currentLineNumber;
        currentColumnNumber = other.currentColumnNumber;
    }


    CppCompilerContext::Private::~Private() {}


    const QByteArray& CppCompilerContext::Private::sourceData() const {
        return rawData;
    }


    unsigned long CppCompilerContext::Private::byteOffset() const {
        return rawData.size();
    }


    unsigned long CppCompilerContext::Private::lineNumber() const {
        return currentLineNumber;
    }


    unsigned long CppCompilerContext::Private::columnNumber() const {
        return currentColumnNumber;
    }


    void CppCompilerContext::Private::append(const std::uint8_t data) {
        if (data == '\n') {
            ++currentLineNumber;
            currentColumnNumber = 0;
        } else {
            ++currentColumnNumber;
        }

        rawData.push_back(static_cast<char>(data));
    }


    void CppCompilerContext::Private::append(const QByteArray& data) {
        unsigned long numberLines = data.count('\n');
        if (numberLines == 0) {
            currentColumnNumber += data.size();
        } else {
            currentLineNumber += numberLines;
            currentColumnNumber = data.size() - data.lastIndexOf('\n') - 1;
        }

        rawData.append(data);
    }
}
