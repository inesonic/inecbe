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
* This file implements the \ref Cbe::CppCompilerDiagnostic class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>

#include "cbe_cpp_compiler_context.h"
#include "cbe_cpp_source_range.h"
#include "cbe_cpp_compiler_diagnostic_private.h"
#include "cbe_compiler_diagnostic.h"
#include "cbe_cpp_compiler_diagnostic.h"

namespace Cbe {
    CppCompilerDiagnostic::CppCompilerDiagnostic(
            QSharedPointer<CppCompilerContext> context,
            CppCompilerDiagnostic::Level       diagnosticLevel,
            CppCompilerDiagnostic::Code        diagnosticCode,
            const QString&                     diagnosticMessage,
            const CppSourceRange&              sourceRange
        ):CompilerDiagnostic(
            diagnosticLevel,
            diagnosticCode,
            diagnosticMessage
        ),impl(new CppCompilerDiagnostic::Private(
            context,
            sourceRange
        )) {}


    CppCompilerDiagnostic::CppCompilerDiagnostic(const CppCompilerDiagnostic& other):CompilerDiagnostic(other) {
        impl = other.impl;
    }


    CppCompilerDiagnostic::~CppCompilerDiagnostic() {}


    QSharedPointer<CppCompilerContext> CppCompilerDiagnostic::context() const {
        return impl->context();
    }


    const CppSourceRange& CppCompilerDiagnostic::sourceRange() const {
        return impl->sourceRange();
    }


    CppCompilerErrorCodes CppCompilerDiagnostic::diagnosticCode() const {
        return static_cast<CppCompilerErrorCodes>(code());
    }


    CppCompilerDiagnostic& CppCompilerDiagnostic::operator=(const CppCompilerDiagnostic& other) {
        CompilerDiagnostic::operator=(other);
        impl = other.impl;

        return *this;
    }
}
