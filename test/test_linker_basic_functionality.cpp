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
* This file implements basic functionality tests for the \ref Cbe:Linker class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QList>
#include <QFile>

#include <cstdint>

#include <cbe_dynamic_library_linker.h>
#include <cbe_linker_notifier.h>
#include <cbe_linker_context.h>
#include <cbe_cpp_compiler.h>
#include <cbe_cpp_compiler_notifier.h>
#include <cbe_cpp_compiler_context.h>

#include "process_memory.h"

#include "process_memory.h"
#include "test_linker_basic_functionality.h"

/***********************************************************************************************************************
 * LinkerNotifier
 */

class LinkerNotifier:public Cbe::LinkerNotifier {
    public:
        LinkerNotifier();

        ~LinkerNotifier() override;

        void reset();

        bool callbacksProperlyOrdered() const;

        bool linkerStartedCalled() const;

        bool linkerFinishedCalled() const;

        const QList<QString>& diagnostics() const;

        QSharedPointer<Cbe::LinkerContext> context() const;

        bool success() const;

        void linkerStarted(QSharedPointer<Cbe::LinkerContext> context) final;

        void linkerFinished(QSharedPointer<Cbe::LinkerContext> context, bool success) final;

        void handleLinkerDiagnostic(QSharedPointer<Cbe::LinkerContext> context, const QString& diagnosticMessage) final;

    private:
        QSharedPointer<Cbe::LinkerContext> activeContext;

        bool linkerWasStarted;
        bool linkerHasFinished;
        bool unexpectedEvent;
        bool successReported;

        QList<QString> reportedDiagnostics;
};


LinkerNotifier::LinkerNotifier() {
    reset();
}


LinkerNotifier::~LinkerNotifier() {}


void LinkerNotifier::reset() {
    activeContext.clear();

    linkerWasStarted = false;
    linkerHasFinished = false;
    unexpectedEvent = false;
    successReported = false;

    reportedDiagnostics.clear();
}


bool LinkerNotifier::callbacksProperlyOrdered() const {
    return !unexpectedEvent;
}


bool LinkerNotifier::linkerStartedCalled() const {
    return linkerWasStarted;
}


bool LinkerNotifier::linkerFinishedCalled() const {
    return linkerHasFinished;
}


const QList<QString>& LinkerNotifier::diagnostics() const {
    return reportedDiagnostics;
}


QSharedPointer<Cbe::LinkerContext> LinkerNotifier::context() const {
    return activeContext;
}


bool LinkerNotifier::success() const {
    return successReported;
}


void LinkerNotifier::linkerStarted(QSharedPointer<Cbe::LinkerContext> context) {
    if (linkerWasStarted || linkerHasFinished || !reportedDiagnostics.isEmpty() || !activeContext.isNull()) {
        unexpectedEvent = true;
    } else {
        linkerWasStarted = true;
        activeContext = context;
    }
}


void LinkerNotifier::linkerFinished(QSharedPointer<Cbe::LinkerContext> context, bool success) {
    if (!linkerWasStarted || linkerHasFinished || context != activeContext || activeContext.isNull()) {
        unexpectedEvent = true;
    } else {
        linkerHasFinished = true;
        successReported = success;
    }
}


void LinkerNotifier::handleLinkerDiagnostic(
        QSharedPointer<Cbe::LinkerContext> context,
        const QString&                     diagnosticMessage
    ) {
    if (!linkerWasStarted || linkerHasFinished || activeContext.isNull()) {
        unexpectedEvent = true;
    } else if (context != activeContext) {
        unexpectedEvent = true;
    } else {
        reportedDiagnostics.push_back(diagnosticMessage);
    }
}

/***********************************************************************************************************************
 * LinkerContext
 */

class LinkerContext:public Cbe::LinkerContext {
    public:
        LinkerContext(
            const QString&        outputFile,
            const QList<QString>& newObjectFiles = QList<QString>(),
            const QList<QString>& newStaticLibraries = QList<QString>(),
            const QList<QString>& newDynamicLibraries = QList<QString>()
        );

        LinkerContext(
            const QString&        outputFile,
            const QString&        objectFile,
            const QList<QString>& newStaticLibraries = QList<QString>(),
            const QList<QString>& newDynamicLibraries = QList<QString>()
        );

        ~LinkerContext() override;

        void reset();

        bool linkerStartedCalled() const;

        bool linkerFinishedCalled() const;

        bool callbacksProperlyOrdered() const;

        const QList<QString>& diagnostics() const;

        bool success() const;

        void linkerStarted() final;

        void linkerFinished(bool success) final;

        void handleLinkerDiagnostic(const QString& diagnostic) final;

    private:
        bool linkerWasStarted;
        bool linkerHasFinished;
        bool unexpectedEvent;
        bool successReported;

        QList<QString> reportedDiagnostics;
};


LinkerContext::LinkerContext(
        const QString&        outputFile,
        const QList<QString>& newObjectFiles,
        const QList<QString>& newStaticLibraries,
        const QList<QString>& newDynamicLibraries
    ):Cbe::LinkerContext(
        outputFile,
        newObjectFiles,
        newStaticLibraries,
        newDynamicLibraries
    ) {
    reset();
}


LinkerContext::LinkerContext(
        const QString&        outputFile,
        const QString&        objectFile,
        const QList<QString>& newStaticLibraries,
        const QList<QString>& newDynamicLibraries
    ):Cbe::LinkerContext(
        outputFile,
        objectFile,
        newStaticLibraries,
        newDynamicLibraries
    ) {
    reset();
}


LinkerContext::~LinkerContext() {}


void LinkerContext::reset() {
    linkerWasStarted = false;
    linkerHasFinished = false;
    unexpectedEvent = false;
    successReported = false;

    reportedDiagnostics.clear();
}


bool LinkerContext::callbacksProperlyOrdered() const {
    return !unexpectedEvent;
}


bool LinkerContext::linkerStartedCalled() const {
    return linkerWasStarted;
}


bool LinkerContext::linkerFinishedCalled() const {
    return linkerHasFinished;
}


const QList<QString>& LinkerContext::diagnostics() const {
    return reportedDiagnostics;
}


bool LinkerContext::success() const {
    return successReported;
}


void LinkerContext::linkerStarted() {
    if (linkerWasStarted || linkerHasFinished || !reportedDiagnostics.isEmpty()) {
        unexpectedEvent = true;
    } else {
        linkerWasStarted = true;
    }
}


void LinkerContext::linkerFinished(bool success) {
    if (!linkerWasStarted || linkerHasFinished) {
        unexpectedEvent = true;
    } else {
        linkerHasFinished = true;
        successReported = success;
    }
}


void LinkerContext::handleLinkerDiagnostic(const QString& diagnostic) {
    if (!linkerWasStarted || linkerHasFinished) {
        unexpectedEvent = true;
    } else {
        reportedDiagnostics.push_back(diagnostic);
    }
}

/***********************************************************************************************************************
 * TestLinkerBasicFunctionality
 */

TestLinkerBasicFunctionality::TestLinkerBasicFunctionality() {}


TestLinkerBasicFunctionality::~TestLinkerBasicFunctionality() {}


void TestLinkerBasicFunctionality::initTestCase() {}


void TestLinkerBasicFunctionality::testSinglePass() {
    #if (defined(Q_OS_WIN))

        QString objectFile     = "test.obj";
        QString dynamicLibrary = "test.dll";

    #elif (defined(Q_OS_DARWIN))

        QString objectFile     = "test.o";
        QString dynamicLibrary = "test.dylib";

    #elif (defined(Q_OS_LINUX))

        QString objectFile     = "test.o";
        QString dynamicLibrary = "test.so";

    #else

        #error Unknown platform

    #endif

    generateObject(objectFile);

    LinkerNotifier notifier;
    QSharedPointer<::LinkerContext> context(new ::LinkerContext(dynamicLibrary, objectFile));

    Cbe::DynamicLibraryLinker linker(&notifier);

    #if (defined(Q_OS_WIN))

        linker.setExecutableDirectory(QString("C:\\opt\\llvm-5.0.1\\Debug\\bin\\"));

    #elif (defined(Q_OS_LINUX))

        linker.setExecutableDirectory(QString("/opt/llvm-5.0.1/bin/"));

    #endif

    linker.link(context);
    linker.waitComplete();

    QVERIFY( notifier.callbacksProperlyOrdered());
    QVERIFY( notifier.linkerStartedCalled());
    QVERIFY( notifier.linkerFinishedCalled());
    QVERIFY( notifier.success());
    QVERIFY( notifier.diagnostics().isEmpty());

    QVERIFY(context->callbacksProperlyOrdered());
    QVERIFY(context->linkerStartedCalled());
    QVERIFY(context->linkerFinishedCalled());
    QVERIFY(context->success());
    QVERIFY(context->diagnostics().isEmpty());

    QFile outputFile(dynamicLibrary);
    QVERIFY(outputFile.exists());
}


void TestLinkerBasicFunctionality::testForMemoryLeaks() {
    // We run the linker multiple times and measure the memory utilization before we start and after we finish.  We can
    // then calculate an average memory lost per run.  Ideally the per run memory lost should be at or very close to
    // very close to zero.

    #if (defined(Q_OS_WIN))

        QString objectFile     = "test.obj";
        QString dynamicLibrary = "test.dll";

    #elif (defined(Q_OS_DARWIN))

        QString objectFile     = "test.o";
        QString dynamicLibrary = "test.dylib";

    #elif (defined(Q_OS_LINUX))

        QString objectFile     = "test.o";
        QString dynamicLibrary = "test.so";

    #else

        #error Unknown platform

    #endif

    generateObject(objectFile);

    LinkerNotifier notifier;
    Cbe::DynamicLibraryLinker linker(&notifier);

    #if (defined(Q_OS_WIN))

        linker.setExecutableDirectory(QString("C:\\opt\\llvm-5.0.1\\Debug\\bin\\"));

    #elif (defined(Q_OS_LINUX))

        linker.setExecutableDirectory(QString("/opt/llvm-5.0.1/bin/"));

    #endif

    unsigned     i;
    std::int64_t endingResidentMemory   = 0;
    std::int64_t startingResidentMemory = 0;

    startingResidentMemory = processMemory();

    QVERIFY(startingResidentMemory >= 0);

    for (i=0 ; i<numberLinkerIterations ; ++i) {
        qDebug() << i;

        notifier.reset();

        QSharedPointer<::LinkerContext> context(new ::LinkerContext(dynamicLibrary, objectFile));
        linker.link(context);
        linker.waitComplete();

        QVERIFY(notifier.callbacksProperlyOrdered());
        QVERIFY(notifier.linkerStartedCalled());
        QVERIFY(notifier.linkerFinishedCalled());
        QVERIFY(notifier.success());
        QVERIFY(notifier.diagnostics().isEmpty());

        QVERIFY(context->callbacksProperlyOrdered());
        QVERIFY(context->linkerStartedCalled());
        QVERIFY(context->linkerFinishedCalled());
        QVERIFY(context->success());
        QVERIFY(context->diagnostics().isEmpty());

        QFile outputFile(dynamicLibrary);
        QVERIFY(outputFile.exists());
    }

    endingResidentMemory = processMemory();
    QVERIFY(endingResidentMemory >= 0);

    double averageMemoryLeak = (1.0 * (endingResidentMemory - startingResidentMemory)) / numberLinkerIterations;
    qDebug() << "averageMemoryLeak = " << averageMemoryLeak;
    QVERIFY(averageMemoryLeak < 2048.0);
}


void TestLinkerBasicFunctionality::generateObject(const QString& objectFile) {
    QSharedPointer<Cbe::CppCompilerContext> context(new Cbe::CppCompilerContext(objectFile));

    #if (defined(Q_OS_WIN))

        *context << "extern \"C\" __declspec(dllexport) int add(int a, int b) {" << Cbe::endl;;

    #else

        *context << "extern \"C\" int add(int a, int b) {" << Cbe::endl;

    #endif

    *context << "    return a + b;" << Cbe::endl
             << "}" << Cbe::endl;

    QFile object(context->objectFile());
    if (object.exists()) {
        bool success = object.remove();
        QVERIFY(success);
    }

    Cbe::CppCompiler compiler;
    compiler.compile(context);
    compiler.waitComplete();
}
