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
* This file implements the \ref DiagnosticConsumer class.
***********************************************************************************************************************/

#include <QString>

#include "warnings.h"

SUPPRESS_LLVM_WARNINGS

#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/DiagnosticIDs.h>
#include <clang/Basic/DiagnosticOptions.h>

RESTORE_LLVM_WARNINGS

#include "compiler_impl.h"
#include "diagnostic_consumer.h"

DiagnosticConsumer::DiagnosticConsumer(CompilerImpl* newCompilerImplementation) {
    currentCompiler = newCompilerImplementation;
}


DiagnosticConsumer::~DiagnosticConsumer() {}


void DiagnosticConsumer::setCompilerImplementation(CompilerImpl* newCompilerImplementation) {
    currentCompiler = newCompilerImplementation;
}


CompilerImpl* DiagnosticConsumer::compilerImplementation() const {
    return currentCompiler;
}


unsigned DiagnosticConsumer::numberErrors() const {
    return getNumErrors();
}


unsigned DiagnosticConsumer::numberWarnings() const {
    return getNumWarnings();
}


void DiagnosticConsumer::clear() {
    return clang::DiagnosticConsumer::clear();
}


void DiagnosticConsumer::HandleDiagnostic(
        clang::DiagnosticsEngine::Level diagnosticLevel,
        const clang::Diagnostic&        diagnosticInformation
    ) {
    clang::DiagnosticConsumer::HandleDiagnostic(diagnosticLevel, diagnosticInformation);
    if (currentCompiler != nullptr) {
        currentCompiler->reportDiagnostic(diagnosticLevel, diagnosticInformation);
    }
}
