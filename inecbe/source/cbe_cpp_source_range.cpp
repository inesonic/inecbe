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
* This file implements the \ref Cbe::Cpp:SourceRange class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>

#include "cbe_cpp_source_range_private.h"
#include "cbe_source_range.h"
#include "cbe_cpp_source_range.h"

namespace Cbe {
    CppSourceRange::CppSourceRange():impl(new CppSourceRange::Private()) {}


    CppSourceRange::CppSourceRange(
            const QString& filename
        ):SourceRange(
            filename
        ),impl(new CppSourceRange::Private(
            1,
            0,
            static_cast<unsigned>(-1),
            static_cast<unsigned>(-1)
        )) {}


    CppSourceRange::CppSourceRange(
            unsigned byteOffset,
            unsigned startLine,
            unsigned startColumn,
            unsigned endLine,
            unsigned endColumn
        ):SourceRange(
            QString(),
            byteOffset
        ),impl(new CppSourceRange::Private(
            startLine,
            startColumn,
            endLine,
            endColumn
        )) {}


    CppSourceRange::CppSourceRange(
            const QString& filename,
            unsigned       byteOffset,
            unsigned       startLine,
            unsigned       startColumn,
            unsigned       endLine,
            unsigned       endColumn
        ):SourceRange(
            filename,
            byteOffset
        ),impl(new CppSourceRange::Private(
            startLine,
            startColumn,
            endLine,
            endColumn
        )) {}


    CppSourceRange::CppSourceRange(
            unsigned byteOffset,
            unsigned line,
            unsigned column
        ):SourceRange(
            QString(),
            byteOffset
        ),impl(new CppSourceRange::Private(
            line,
            column
        )) {}


    CppSourceRange::CppSourceRange(
            const QString& filename,
            unsigned       byteOffset,
            unsigned       line,
            unsigned       column
        ):SourceRange(
            filename,
            byteOffset
        ),impl(new CppSourceRange::Private(
            line,
            column
        )) {}


    CppSourceRange::CppSourceRange(const CppSourceRange& other):SourceRange(other) {
        impl = other.impl;
    }


    CppSourceRange::~CppSourceRange() {}


    bool CppSourceRange::isValid() const {
        return impl->isValid();
    }


    bool CppSourceRange::isInvalid() const {
        return !isValid();
    }


    unsigned CppSourceRange::startLineNumber() const {
        return impl->startLineNumber();
    }


    unsigned CppSourceRange::startColumnNumber() const {
        return impl->startColumnNumber();
    }


    unsigned CppSourceRange::endLineNumber() const {
        return impl->endLineNumber();
    }


    unsigned CppSourceRange::endColumnNumber() const {
        return impl->endColumnNumber();
    }


    CppSourceRange& CppSourceRange::operator=(const CppSourceRange& other) {
        SourceRange::operator=(other);
        impl = other.impl;

        return *this;
    }


    bool CppSourceRange::operator==(const CppSourceRange& other) const {
        return SourceRange::operator==(other)                               &&
               impl->startLineNumber() == other.impl->startLineNumber()     &&
               impl->startColumnNumber() == other.impl->startColumnNumber() &&
               impl->endLineNumber() == other.impl->endLineNumber()         &&
               impl->endColumnNumber() == other.impl->endColumnNumber()        ;
    }


    bool CppSourceRange::operator!=(const CppSourceRange& other) const {
        return !operator==(other);
    }
}
