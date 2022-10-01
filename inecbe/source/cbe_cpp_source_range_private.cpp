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
* This file implements the \ref Cbe::CppSourceRange::Private class.
***********************************************************************************************************************/

#include <QString>

#include "cbe_cpp_source_range.h"
#include "cbe_cpp_source_range_private.h"

namespace Cbe {
    CppSourceRange::Private::Private() {
        currentStartLine   = 0;
        currentStartColumn = 0;
        currentEndLine     = 0;
        currentEndColumn   = 0;
    }


    CppSourceRange::Private::Private(unsigned startLine, unsigned startColumn, unsigned endLine, unsigned endColumn) {
        currentStartLine   = startLine;
        currentStartColumn = startColumn;
        currentEndLine     = endLine;
        currentEndColumn   = endColumn;
    }


    CppSourceRange::Private::Private(unsigned line, unsigned column) {
        currentStartLine   = line;
        currentStartColumn = column;
        currentEndLine     = line;
        currentEndColumn   = column;
    }


    CppSourceRange::Private::~Private() {}


    bool CppSourceRange::Private::isValid() const {
        return currentStartLine > 0                                                                 &&
               currentEndLine > 0                                                                   &&
               (currentStartLine < currentEndLine                                              ||
                (currentStartLine == currentEndLine && currentStartColumn <= currentEndColumn)    )    ;
    }


    unsigned CppSourceRange::Private::startLineNumber() const {
        return currentStartLine;
    }


    unsigned CppSourceRange::Private::startColumnNumber() const {
        return currentStartColumn;
    }


    unsigned CppSourceRange::Private::endLineNumber() const {
        return currentEndLine;
    }


    unsigned CppSourceRange::Private::endColumnNumber() const {
        return currentEndColumn;
    }
}
