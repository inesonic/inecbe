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
* This file implements basic functionality tests for the \ref Cbe:DynamicLibraryLoader class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QList>
#include <QFile>
#include <QFileInfo>

#include <cstdint>

#include <cbe_dynamic_library_linker.h>
#include <cbe_linker_notifier.h>
#include <cbe_linker_context.h>
#include <cbe_cpp_compiler.h>
#include <cbe_cpp_compiler_notifier.h>
#include <cbe_cpp_compiler_context.h>
#include <cbe_loader_notifier.h>
#include <cbe_dynamic_library_loader.h>

#include "process_memory.h"
#include "test_dynamic_library_loader_basic_functionality.h"

/***********************************************************************************************************************
 * TestDynamicLibraryLoaderBasicFunctionality
 */

TestDynamicLibraryLoaderBasicFunctionality::TestDynamicLibraryLoaderBasicFunctionality() {}


TestDynamicLibraryLoaderBasicFunctionality::~TestDynamicLibraryLoaderBasicFunctionality() {}


void TestDynamicLibraryLoaderBasicFunctionality::initTestCase() {}


void TestDynamicLibraryLoaderBasicFunctionality::testBasicFunctionality() {
    Cbe::DynamicLibraryLoader loader;

    #if (defined(Q_OS_WIN))

        QString libraryFile = QFileInfo("test.dll").absoluteFilePath();

    #elif (defined(Q_OS_DARWIN))

        QString libraryFile = QFileInfo("test.dylib").absoluteFilePath();

    #elif (defined(Q_OS_LINUX))

        QString libraryFile = QFileInfo("test.so").absoluteFilePath();

    #else

        #error Unknown platform

    #endif

    generateDynamicLibrary(libraryFile);
    bool success = loader.load(libraryFile);
    QVERIFY(success);

    typedef int (*LibraryFunction)(int a, int b);

    LibraryFunction libraryFunction = reinterpret_cast<LibraryFunction>(loader.resolve("add"));
    QVERIFY(libraryFunction != nullptr);

    int result = (*libraryFunction)(3, 4);
    QVERIFY(result == 7);
}


void TestDynamicLibraryLoaderBasicFunctionality::generateDynamicLibrary(const QString& libraryFile) {
    #if (defined(Q_OS_WIN))

        QString objectFile = libraryFile + ".obj";

    #else

        QString objectFile = libraryFile + ".o";

    #endif

    QSharedPointer<Cbe::CppCompilerContext> compilerContext(new Cbe::CppCompilerContext(objectFile));

    #if (defined(Q_OS_WIN))

        *compilerContext << "extern \"C\" __declspec(dllexport) int add(int a, int b) {" << Cbe::endl;;

    #else

        *compilerContext << "extern \"C\" int add(int a, int b) {" << Cbe::endl;

    #endif

    *compilerContext << "    return a + b;" << Cbe::endl
                     << "}" << Cbe::endl;

    QFile object(compilerContext->objectFile());
    if (object.exists()) {
        bool success = object.remove();
        QVERIFY(success);
    }

    Cbe::CppCompiler compiler;
    compiler.compile(compilerContext);
    compiler.waitComplete();

    QFile dynamicLibrary(libraryFile);
    if (dynamicLibrary.exists()) {
        bool success = dynamicLibrary.remove();
        QVERIFY(success);
    }

    Cbe::DynamicLibraryLinker linker;

    #if (defined(Q_OS_WIN))

        linker.setExecutableDirectory(QString("C:\\opt\\llvm-5.0.1\\Debug\\bin\\"));

    #elif (defined(Q_OS_LINUX))

        linker.setExecutableDirectory(QString("/opt/llvm-5.0.1/bin/"));

    #endif

    QSharedPointer<Cbe::LinkerContext> linkerContext(new Cbe::LinkerContext(libraryFile, objectFile));
    linker.link(linkerContext);
    linker.waitComplete();
}
