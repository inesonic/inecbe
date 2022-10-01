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
* This file implements the \ref Cbe::CppCompiler class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QSharedPointer>

#include "cbe_cpp_compiler_diagnostic.h"
#include "cbe_compiler_context.h"
#include "cbe_cpp_compiler_context.h"
#include "cbe_compiler_notifier.h"
#include "cbe_cpp_compiler_notifier.h"
#include "cbe_compiler.h"
#include "cbe_cpp_compiler.h"

namespace Cbe {
    CppCompiler::CppCompiler(CppCompilerNotifier* newNotifier):Compiler(newNotifier) {}


    CppCompiler::~CppCompiler() {}


    void CppCompiler::setNotifier(CppCompilerNotifier* newNotifier) {
        Compiler::setNotifier(newNotifier);
    }


    CppCompilerNotifier* CppCompiler::notifier() const {
        return dynamic_cast<CppCompilerNotifier*>(Compiler::notifier());
    }


    QList<QString> CppCompiler::setDefaultSwitches() const {
        QList<QString> switches;

        switches << "-c"
                 << "-x" << "c++"
                 << "-std=c++14"
                 << "-O3";

        #if (defined(Q_OS_LINUX))

            switches << "-fPIC";

        #elif (defined(Q_OS_WIN))

            switches << "-fno-rtti";

        #elif (!defined(Q_OS_DARWIN))

            #error Unknown platform

        #endif

        return switches;
    }


    void CppCompiler::compilerStarted(QSharedPointer<CompilerContext> context) {
        CppCompilerNotifier*               cppNotifier = notifier();
        QSharedPointer<CppCompilerContext> cppContext  = context.dynamicCast<CppCompilerContext>();

        if (cppNotifier != nullptr) {
            cppNotifier->compilerStarted(cppContext);
        }

        cppContext->compilerStarted();
    }


    void CppCompiler::compilerFinished(QSharedPointer<CompilerContext> context, bool successful) {
        CppCompilerNotifier*               cppNotifier = notifier();
        QSharedPointer<CppCompilerContext> cppContext  = context.dynamicCast<CppCompilerContext>();

        cppContext->compilerFinished(successful);

        if (cppNotifier != nullptr) {
            cppNotifier->compilerFinished(cppContext, successful);
        }
    }


    void CppCompiler::processDiagnostic(
            CompilerNotifier*               notifier,
            QSharedPointer<CompilerContext> context,
            CompilerDiagnostic::Level       diagnosticLevel,
            CompilerDiagnostic::Code        diagnosticCode,
            const QString&                  diagnosticMessage,
            const QString&                  filename,
            unsigned                        byteOffset,
            unsigned                        lineNumber,
            unsigned                        columnNumber
        ) {
        QSharedPointer<CppCompilerContext> cppContext = context.dynamicCast<CppCompilerContext>();

        CppSourceRange sourceRange;
        if (filename.isEmpty()) {
            sourceRange = CppSourceRange(byteOffset, lineNumber, columnNumber);
        } else {
            sourceRange = CppSourceRange(filename, byteOffset, lineNumber, columnNumber);
        }

        CppCompilerDiagnostic diagnostic(
            cppContext,
            diagnosticLevel,
            diagnosticCode,
            diagnosticMessage,
            sourceRange
        );

        CppCompilerNotifier* cppNotifier = dynamic_cast<CppCompilerNotifier*>(notifier);
        if (cppNotifier != nullptr) {
            cppNotifier->handleCompilerDiagnostic(diagnostic);
        }

        cppContext->handleCompilerDiagnostic(diagnostic);
    }
}
