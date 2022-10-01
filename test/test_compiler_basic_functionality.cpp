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
* This file implements basic functionality tests for the \ref Cbe:CppCompiler, \ref Cbe::DynamicLibraryLinker and
* \ref Cbe::DynamicLibraryLoader classes.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QList>
#include <QFile>

#include <QDebug>

#include <cstdint>

#include <cbe_cpp_compiler.h>
#include <cbe_cpp_compiler_notifier.h>
#include <cbe_dynamic_library_linker.h>
#include <cbe_linker_notifier.h>
#include <cbe_dynamic_library_loader.h>
#include <cbe_loader_notifier.h>
#include <cbe_cpp_compiler_context.h>

#include "process_memory.h"
#include "test_compiler_basic_functionality.h"

/***********************************************************************************************************************
 * CompilerNotifier
 */

class CompilerNotifier:public Cbe::CppCompilerNotifier {
    public:
        CompilerNotifier();

        ~CompilerNotifier() override;

        void reset();

        bool callbacksProperlyOrdered() const;

        bool compilerStartedCalled() const;

        bool compilerFinishedCalled() const;

        const QList<Cbe::CppCompilerDiagnostic>& diagnostics() const;

        QSharedPointer<Cbe::CppCompilerContext> context() const;

        bool success() const;

        bool fatalError() const;

        void compilerStarted(QSharedPointer<Cbe::CppCompilerContext> context) final;

        void compilerFinished(QSharedPointer<Cbe::CppCompilerContext> context, bool success) final;

        void handleCompilerDiagnostic(const Cbe::CppCompilerDiagnostic& diagnostic) final;

        void fatalCompilerError(const QString& reason, bool generateCrashDiagnostics) final;


    private:
        QSharedPointer<Cbe::CppCompilerContext> activeContext;

        bool compilerWasStarted;
        bool compilerHasFinished;
        bool unexpectedEvent;
        bool successReported;
        bool fatalErrorReported;

        QList<Cbe::CppCompilerDiagnostic> reportedDiagnostics;
};


CompilerNotifier::CompilerNotifier() {
    reset();
}


CompilerNotifier::~CompilerNotifier() {}


void CompilerNotifier::reset() {
    activeContext.clear();

    compilerWasStarted = false;
    compilerHasFinished = false;
    unexpectedEvent = false;
    successReported = false;
    fatalErrorReported = false;

    reportedDiagnostics.clear();
}


bool CompilerNotifier::callbacksProperlyOrdered() const {
    return !unexpectedEvent;
}


bool CompilerNotifier::compilerStartedCalled() const {
    return compilerWasStarted;
}


bool CompilerNotifier::compilerFinishedCalled() const {
    return compilerHasFinished;
}


const QList<Cbe::CppCompilerDiagnostic>& CompilerNotifier::diagnostics() const {
    return reportedDiagnostics;
}


QSharedPointer<Cbe::CppCompilerContext> CompilerNotifier::context() const {
    return activeContext;
}


bool CompilerNotifier::success() const {
    return successReported;
}


bool CompilerNotifier::fatalError() const {
    return fatalErrorReported;
}


void CompilerNotifier::compilerStarted(QSharedPointer<Cbe::CppCompilerContext> context) {
    if (compilerWasStarted || compilerHasFinished || !reportedDiagnostics.isEmpty() || !activeContext.isNull()) {
        unexpectedEvent = true;
    } else {
        compilerWasStarted = true;
        activeContext = context;
    }
}


void CompilerNotifier::compilerFinished(QSharedPointer<Cbe::CppCompilerContext> context, bool success) {
    if (!compilerWasStarted || compilerHasFinished || context != activeContext || activeContext.isNull()) {
        unexpectedEvent = true;
    } else {
        compilerHasFinished = true;
        successReported = success;
    }
}


void CompilerNotifier::handleCompilerDiagnostic(const Cbe::CppCompilerDiagnostic& diagnostic) {
    if (!compilerWasStarted || compilerHasFinished || activeContext.isNull()) {
        unexpectedEvent = true;
    } else if (diagnostic.context() != activeContext) {
        unexpectedEvent = true;
    } else {
        QString message = diagnostic.message();
        qDebug() << message;
        reportedDiagnostics.push_back(diagnostic);
    }
}


void CompilerNotifier::fatalCompilerError(const QString&, bool) {
    fatalErrorReported = true;
}

/***********************************************************************************************************************
 * CompilerContext
 */

class CompilerContext:public Cbe::CppCompilerContext {
    public:
        CompilerContext(
            const QString&        newObjectFile,
            const QList<QString>& newPchFiles = QList<QString>(),
            const QList<QString>& newHeaderFiles = QList<QString>()
        );

        CompilerContext(
            const QString&        newObjectFile,
            const QByteArray&     newSourceData,
            const QList<QString>& newPchFiles = QList<QString>(),
            const QList<QString>& newHeaderFiles = QList<QString>()
        );

        ~CompilerContext() override;

        void reset();

        bool compilerStartedCalled() const;

        bool compilerFinishedCalled() const;

        bool callbacksProperlyOrdered() const;

        const QList<Cbe::CppCompilerDiagnostic>& diagnostics() const;

        bool success() const;

        bool fatalError() const;

        void compilerStarted() final;

        void compilerFinished(bool success) final;

        void handleCompilerDiagnostic(const Cbe::CppCompilerDiagnostic& diagnostic) final;

    private:
        bool compilerWasStarted;
        bool compilerHasFinished;
        bool unexpectedEvent;
        bool successReported;
        bool fatalErrorReported;

        QList<Cbe::CppCompilerDiagnostic> reportedDiagnostics;
};


CompilerContext::CompilerContext(
        const QString&        newObjectFile,
        const QList<QString>& newPchFiles,
        const QList<QString>& newHeaderFiles
    ):Cbe::CppCompilerContext(
        newObjectFile,
        newPchFiles,
        newHeaderFiles
    ) {
    reset();
}


CompilerContext::CompilerContext(
        const QString&        newObjectFile,
        const QByteArray&     newSourceData,
        const QList<QString>& newPchFiles,
        const QList<QString>& newHeaderFiles
    ):Cbe::CppCompilerContext(
        newObjectFile,
        newSourceData,
        newPchFiles,
        newHeaderFiles
    ) {
    reset();
}


CompilerContext::~CompilerContext() {}


void CompilerContext::reset() {
    compilerWasStarted = false;
    compilerHasFinished = false;
    unexpectedEvent = false;
    successReported = false;
    fatalErrorReported = false;

    reportedDiagnostics.clear();
}


bool CompilerContext::callbacksProperlyOrdered() const {
    return !unexpectedEvent;
}


bool CompilerContext::compilerStartedCalled() const {
    return compilerWasStarted;
}


bool CompilerContext::compilerFinishedCalled() const {
    return compilerHasFinished;
}


const QList<Cbe::CppCompilerDiagnostic>& CompilerContext::diagnostics() const {
    return reportedDiagnostics;
}


bool CompilerContext::success() const {
    return successReported;
}


bool CompilerContext::fatalError() const {
    return fatalErrorReported;
}


void CompilerContext::compilerStarted() {
    if (compilerWasStarted || compilerHasFinished || !reportedDiagnostics.isEmpty()) {
        unexpectedEvent = true;
    } else {
        compilerWasStarted = true;
    }
}


void CompilerContext::compilerFinished(bool success) {
    if (!compilerWasStarted || compilerHasFinished) {
        unexpectedEvent = true;
    } else {
        compilerHasFinished = true;
        successReported = success;
    }
}


void CompilerContext::handleCompilerDiagnostic(const Cbe::CppCompilerDiagnostic& diagnostic) {
    if (!compilerWasStarted || compilerHasFinished) {
        unexpectedEvent = true;
    } else if (diagnostic.context() != this) {
        unexpectedEvent = true;
    } else {
        reportedDiagnostics.push_back(diagnostic);
    }
}

/***********************************************************************************************************************
 * TestCompilerBasicFunctionality
 */

TestCompilerBasicFunctionality::TestCompilerBasicFunctionality() {}


TestCompilerBasicFunctionality::~TestCompilerBasicFunctionality() {}


void TestCompilerBasicFunctionality::initTestCase() {}


void TestCompilerBasicFunctionality::testSinglePass() {
    CompilerNotifier compilerNotifier;
    QSharedPointer<CompilerContext> context(new CompilerContext("test.o"));

    #if (defined(Q_OS_WIN))

        *context << "extern \"C\" __declspec(dllexport) int add(int a, int b) {" << Cbe::endl;;

    #else

        *context << "extern \"C\" int add(int a, int b) {" << Cbe::endl;

    #endif

    *context << "    return a + b;" << Cbe::endl
             << "}" << Cbe::endl;

    Cbe::CppCompiler compiler(&compilerNotifier);

    QList<QString> headers;
    QList<QString> headerSearchPaths;

    #if (defined(Q_OS_WIN))

        QString dynamicLibrary = QString("test.dll");

    #elif (defined(Q_OS_DARWIN))

        compiler.setSystemRoot(
            "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk"
        );

        compiler.setResourceDirectory("/opt/llvm-5.0.1/include/c++/v1/");

        headerSearchPaths << "/usr/local/include";
        QString dynamicLibrary = QString("test.dylib");

    #elif (defined(__linux__))

//        headerSearchPaths << "/usr/include"
//                          << "/usr/include/linux"
//                          << "/usr/lib/gcc/x86_64-redhat-linux/4.8.2/include/"
//                          << "/usr/local/include";

//        headers << "stddef.h"
//                << "stdio.h";

        QString dynamicLibrary = QString("/home/psmith/test.so");

    #else

        #error Unknown platform

    #endif

    compiler.setHeaderSearchPaths(headerSearchPaths);
    compiler.setHeaders(headers);

    QFile objectFile(context->objectFile());
    if (objectFile.exists()) {
        bool success = objectFile.remove();
        QVERIFY(success);
    }

    compiler.compile(context);
    compiler.waitComplete();

    QVERIFY(compilerNotifier.callbacksProperlyOrdered());
    QVERIFY(compilerNotifier.compilerStartedCalled());
    QVERIFY(compilerNotifier.compilerFinishedCalled());
    QVERIFY(compilerNotifier.success());
    QVERIFY(!compilerNotifier.fatalError());
    QVERIFY(compilerNotifier.diagnostics().isEmpty());

    QVERIFY(context->callbacksProperlyOrdered());
    QVERIFY(context->compilerStartedCalled());
    QVERIFY(context->compilerFinishedCalled());
    QVERIFY(context->success());
    QVERIFY(context->diagnostics().isEmpty());

    objectFile.setFileName(context->objectFile());
    QVERIFY(objectFile.exists());
}


void TestCompilerBasicFunctionality::testErrorReporting() {
    CompilerNotifier compilerNotifier;
    QSharedPointer<CompilerContext> context(new CompilerContext("test.o"));

    unsigned errorColumnNumber;

    #if (defined(Q_OS_WIN))

        *context << "extern \"C\" __declspec(dllexport) int foo add(int a, int b) {" << Cbe::endl;
        errorColumnNumber = 41;

    #else

        *context << "extern \"C\" int foo add(int a, int b) {" << Cbe::endl;
        errorColumnNumber = 19;

    #endif

    *context << "    return a + b;" << Cbe::endl
             << "}" << Cbe::endl;

    Cbe::CppCompiler compiler(&compilerNotifier);

    QList<QString> headers;
    QList<QString> headerSearchPaths;

    #if (defined(Q_OS_WIN))


    #elif (defined(Q_OS_DARWIN))

        compiler.setSystemRoot(
            "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk"
        );

        compiler.setResourceDirectory("/opt/llvm-5.0.1/include/c++/v1/");

        headerSearchPaths << "/usr/local/include";

    #elif (defined(__linux__))

//        headerSearchPaths << "/usr/include"
//                          << "/usr/include/linux"
//                          << "/usr/lib/gcc/x86_64-redhat-linux/4.8.2/include/"
//                          << "/usr/local/include";

//        headers << "stddef.h"
//                << "stdio.h";

    #else

        #error Unknown platform

    #endif

    compiler.setHeaderSearchPaths(headerSearchPaths);
    compiler.setHeaders(headers);

    QFile objectFile(context->objectFile());
    if (objectFile.exists()) {
        bool success = objectFile.remove();
        QVERIFY(success);
    }

    compiler.compile(context);
    compiler.waitComplete();

    QVERIFY(compilerNotifier.callbacksProperlyOrdered());
    QVERIFY(compilerNotifier.compilerStartedCalled());
    QVERIFY(compilerNotifier.compilerFinishedCalled());
    QVERIFY(!compilerNotifier.success());
    QVERIFY(!compilerNotifier.fatalError());
    QVERIFY(!compilerNotifier.diagnostics().isEmpty());

    QVERIFY(compilerNotifier.diagnostics().size() == 1);

    Cbe::CppCompilerDiagnostic diagnostic = compilerNotifier.diagnostics().at(0);

    QVERIFY(diagnostic.level() == Cbe::CppCompilerDiagnostic::Level::ERROR);
    QVERIFY(diagnostic.code() == 1263); // CLang specific error code
    QVERIFY(diagnostic.sourceRange().filename().isEmpty());
    QVERIFY(diagnostic.sourceRange().startLineNumber() == 1);
    QVERIFY(diagnostic.sourceRange().startColumnNumber() == errorColumnNumber);

    QVERIFY(context->callbacksProperlyOrdered());
    QVERIFY(context->compilerStartedCalled());
    QVERIFY(context->compilerFinishedCalled());
    QVERIFY(!context->success());
    QVERIFY(!context->diagnostics().isEmpty());

    QVERIFY(context->diagnostics().size() == 1);

    diagnostic = context->diagnostics().at(0);

    QVERIFY(diagnostic.level() == Cbe::CppCompilerDiagnostic::Level::ERROR);
    QVERIFY(diagnostic.code() == 1263); // CLang specific error code
    QVERIFY(diagnostic.sourceRange().filename().isEmpty());
    QVERIFY(diagnostic.sourceRange().startLineNumber() == 1);
    QVERIFY(diagnostic.sourceRange().startColumnNumber() == errorColumnNumber);
}


void TestCompilerBasicFunctionality::testForMemoryLeaks() {
    // We run the compiler multiple times and measure the memory utilization before we start and after we finish.  We
    // can then calculate an average memory lost per run.  Ideally the per run memory lost should be at or very close to
    // very close to zero.

    unsigned     i;
    std::int64_t endingResidentMemory   = 0;
    std::int64_t startingResidentMemory = 0;

    startingResidentMemory = processMemory();

    QVERIFY(startingResidentMemory >= 0);

    for (i=0 ; i<numberCompilerIterations ; ++i) {
        testSinglePass();
    }

    endingResidentMemory = processMemory();
    QVERIFY(endingResidentMemory >= 0);

    double averageMemoryLeak = (1.0 * (endingResidentMemory - startingResidentMemory)) / numberCompilerIterations;
    qDebug() << "averageMemoryLeak = " << averageMemoryLeak;
    QVERIFY(averageMemoryLeak < 42000.0);
}
