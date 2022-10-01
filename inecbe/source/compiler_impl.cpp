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
* This file implements the private \ref CompilerImpl class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QSet>
#include <QThread>
#include <QScopedPointer>
#include <QTemporaryFile>
#include <QMutex>
#include <QMutexLocker>
#include <QDir>
#include <QCoreApplication>

#include <QDebug> // Debug

#include <memory>
#include <string>
#include <cstring>
#include <system_error>

#include "warnings.h"

SUPPRESS_LLVM_WARNINGS

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/DiagnosticIDs.h>
#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Basic/LangOptions.h>
#include <clang/Basic/TargetOptions.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/FrontendTool/Utils.h>
#include <clang/Lex/PreprocessorOptions.h>
#include <clang/Driver/Driver.h>
#include <clang/Driver/Compilation.h>
#include <clang/Driver/Job.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>
#include <clang/Frontend/PCHContainerOperations.h>
#include <clang/CodeGen/ObjectFilePCHContainerOperations.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Host.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/PassRegistry.h>
#include <llvm/ADT/APInt.h>

RESTORE_LLVM_WARNINGS

#include "cbe_source_range.h"
#include "cbe_compiler_context.h"
#include "cbe_job_queue.h"
#include "diagnostic_consumer.h"
#include "cbe_compiler_notifier.h"
#include "compiler_impl.h"

const unsigned long CompilerImpl::requiredStackSpace = 512 * 1024;

bool CompilerImpl::backendInitializationNeeded = true;
QSet<CompilerImpl*> CompilerImpl::compilers;

CompilerImpl::CompilerImpl(
        Cbe::CompilerNotifier* newNotifier
    ):jobQueue(
        new Cbe::SimpleJobQueue<Cbe::CompilerContext>()
    ),temporaryInputFile(
        QDir::tempPath() + "/Inesonic." + QCoreApplication::applicationName() + ".XXXXXX"
    ),mainFileEntry(
        std::error_code()
    ),currentNotifier(
        newNotifier
    ) {
    setStackSize(requiredStackSpace);
    configureCompilerBackend();

    currentSystemRoot = QString("/");

    generateDefaultSwitches = true;
    isTerminatingThread = false;
    currentDebugOutputEnabled = false;

    compilers.insert(this);
}


CompilerImpl::~CompilerImpl() {
    clearUserFriendlyCommandLine();
    waitComplete();
    compilers.remove(this);
}


void CompilerImpl::setDebugOutputEnabled(bool enableDebugOutput) {
    currentDebugOutputEnabled = enableDebugOutput;
}


bool CompilerImpl::debugOutputEnabled() const {
    return currentDebugOutputEnabled;
}


void CompilerImpl::setNotifier(Cbe::CompilerNotifier* newNotifier) {
    QMutexLocker mutexLocker(&compilerAccessMutex);
    currentNotifier = newNotifier;
}


Cbe::CompilerNotifier* CompilerImpl::notifier() const {
    return currentNotifier;
}


void CompilerImpl::setJobQueue(QSharedPointer<Cbe::JobQueue<Cbe::CompilerContext>> newJobQueue) {
    QMutexLocker mutexLocker(&compilerAccessMutex);
    jobQueue = newJobQueue;
}


bool CompilerImpl::isInternal() const {
    return true;
}


void CompilerImpl::setExecutableDirectory(const QString&) {}


QString CompilerImpl::executableDirectory() const {
    return QString();
}


QList<QString> CompilerImpl::compilerSwitches() {
    if (generateDefaultSwitches) {
        generateDefaultSwitches = false;
        currentSwitches = setDefaultSwitches();
    }

    return currentSwitches;
}


void CompilerImpl::setCompilerSwitches(const QList<QString>& newCompilerSwitches) {
    QMutexLocker mutexLocker(&compilerAccessMutex);

    currentSwitches = newCompilerSwitches;
    generateDefaultSwitches = false;

    compilerInstance.reset();
    diagnosticsEngine.reset();
}


void CompilerImpl::setSystemRoot(const QString& newSystemRoot) {
    QMutexLocker mutexLocker(&compilerAccessMutex);

    currentSystemRoot = newSystemRoot;
    compilerInstance.reset();
    diagnosticsEngine.reset();
}


QString CompilerImpl::systemRoot() const {
    return currentSystemRoot;
}


QList<QString> CompilerImpl::headerSearchPaths() const {
    return currentHeaderSearchPaths;
}


void CompilerImpl::setHeaderSearchPaths(const QList<QString>& newHeaderSearchPaths) {
    QMutexLocker mutexLocker(&compilerAccessMutex);

    currentHeaderSearchPaths = newHeaderSearchPaths;
    compilerInstance.reset();
    diagnosticsEngine.reset();
}


QList<QString> CompilerImpl::headers() const {
    return currentHeaders;
}


void CompilerImpl::setHeaders(const QList<QString>& newHeaders) {
    QMutexLocker mutexLocker(&compilerAccessMutex);

    currentHeaders = newHeaders;
    compilerInstance.reset();
    diagnosticsEngine.reset();
}


QList<QString> CompilerImpl::precompiledHeaders() const {
    return currentPrecompiledHeaders;
}


void CompilerImpl::setPrecompiledHeaders(const QList<QString>& newPrecompiledHeaders) {
    QMutexLocker mutexLocker(&compilerAccessMutex);

    currentPrecompiledHeaders = newPrecompiledHeaders;
    compilerInstance.reset();
    diagnosticsEngine.reset();
}


QString CompilerImpl::resourceDirectory() const {
    return currentResourceDirectory;
}


void CompilerImpl::setResourceDirectory(const QString& newResourceDirectory) {
    QMutexLocker mutexLocker(&compilerAccessMutex);

    currentResourceDirectory = newResourceDirectory;
    compilerInstance.reset();
    diagnosticsEngine.reset();
}


QString CompilerImpl::gccToolchain() const {
    return currentGccToolchainPrefix;
}


void CompilerImpl::setGccToolchain(const QString& newGccToolchainPrefix) {
    QMutexLocker mutexLocker(&compilerAccessMutex);

    currentGccToolchainPrefix = newGccToolchainPrefix;
    compilerInstance.reset();
    diagnosticsEngine.reset();
}


QString CompilerImpl::targetTriple() const {
    return currentTargetTripleOverride;
}


void CompilerImpl::setTargetTriple(const QString& newTargetTriple) {
    QMutexLocker mutexLocker(&compilerAccessMutex);

    currentTargetTripleOverride = newTargetTriple;
    compilerInstance.reset();
    diagnosticsEngine.reset();
}


void CompilerImpl::compile(QSharedPointer<Cbe::CompilerContext> context) {
    jobQueueMutex.lock();
    jobQueue->enqueue(context);
    jobQueueMutex.unlock();

    // The background thread might check the queue right before we enqueue a new job and then attempt to terminate.  We
    // use the isTerminatingThread sentinel to work around this race condition.
    if (isTerminatingThread) {
        wait();
    }

    if (!isRunning()) {
        start();
    }
}


void CompilerImpl::waitComplete() {
    wait();
}


bool CompilerImpl::waitComplete(unsigned long maximumTimeInMilliseconds) {
    return wait(maximumTimeInMilliseconds);
}


bool CompilerImpl::active() const {
    return isRunning();
}


void CompilerImpl::reportDiagnostic(
        clang::DiagnosticsEngine::Level diagnosticLevel,
        const clang::Diagnostic&        diagnosticInformation
    ) {
    Cbe::CompilerDiagnostic::Level reportedLevel;

    switch(diagnosticLevel) {
        case clang::DiagnosticsEngine::Note:    { reportedLevel = Cbe::CompilerDiagnostic::Level::NOTE;    break; }
        case clang::DiagnosticsEngine::Warning: { reportedLevel = Cbe::CompilerDiagnostic::Level::WARNING; break; }
        case clang::DiagnosticsEngine::Remark:  { reportedLevel = Cbe::CompilerDiagnostic::Level::REMARK;  break; }
        case clang::DiagnosticsEngine::Error:   { reportedLevel = Cbe::CompilerDiagnostic::Level::ERROR;   break; }
        case clang::DiagnosticsEngine::Fatal:   { reportedLevel = Cbe::CompilerDiagnostic::Level::ERROR;   break; }
        default:                                { reportedLevel = Cbe::CompilerDiagnostic::Level::UNKNOWN; break; }
    }

    clang::SourceManager&        sourceManager  = diagnosticInformation.getSourceManager();
    const clang::SourceLocation& sourceLocation = diagnosticInformation.getLocation();
    bool                         badBuffer      = true;
    const char*                  rawFilename    = sourceManager.getBufferName(sourceLocation, &badBuffer).data();
    unsigned                     byteOffset     = sourceManager.getFileOffset(sourceLocation);
    bool                         badLine        = true;
    unsigned                     lineNumber     = sourceManager.getExpansionLineNumber(sourceLocation, &badLine);
    bool                         badColumn      = true;
    unsigned                     columnNumber   = sourceManager.getExpansionColumnNumber(
        sourceLocation,
        &badColumn
    );

    unsigned numberRanges = diagnosticInformation.getNumRanges();
    unsigned rangeIndex   = 0;
    while (rangeIndex < numberRanges) {
        const clang::CharSourceRange& range = diagnosticInformation.getRange(rangeIndex);
        clang::SourceLocation sourceLocation = range.getBegin();
        QString locationString = QString::fromStdString(sourceLocation.printToString(sourceManager));
        qDebug() << locationString;

        ++rangeIndex;
    }

    if (badLine) {
        lineNumber = Cbe::Compiler::badLineNumber;
    }

    if (badColumn) {
        columnNumber = Cbe::Compiler::badColumnNumber;
    }

    QString filename;
    if (!badBuffer && QString(rawFilename) != temporaryInputFile.fileName()) {
        filename = QString(rawFilename);
    }

    llvm::SmallString<100> messageBuffer;
    diagnosticInformation.FormatDiagnostic(messageBuffer);
    QString diagnosticMessage = QString(messageBuffer.c_str());

    Cbe::CompilerDiagnostic::Code diagnosticCode = diagnosticInformation.getID();
    currentDiagnostics.append(
        DiagnosticData(
            reportedLevel,
            diagnosticCode,
            diagnosticMessage,
            filename,
            byteOffset,
            lineNumber,
            columnNumber
        )
    );
}


void CompilerImpl::run() {
    QMutexLocker mutexLocker(&compilerAccessMutex);

    do {
        jobQueueMutex.lock();
        activeContext = jobQueue->dequeue();
        isTerminatingThread = activeContext.isNull();
        jobQueueMutex.unlock();

        if (!isTerminatingThread) {
            bool success;
            if (compilerInstance.isNull()) {
                success = reconfigureCompiler();
            } else {
                compilerInstance->getDiagnosticClient().clear();
                compilerInstance->getDiagnostics().Reset();
                success = true;
            }

            if (success) {
                // Source files are ultimately managed under the compiler invocation by the clang::SourceCodeManager.
                // This class uses the clang::FileManager class to perform the actual read operations.  An implicit
                // assumption throughout the CLang code is that all source comes from either a named pipe or a real
                // file.  We can not hand an instance of clang::FrontendInputFile with a memory buffer to the compiler
                // and have it accept it.
                //
                // Hacking the compiler to accept a buffer in a FIF (as the underlying compiler implementation calls
                // it), would be extremely ugly as the assumption of a file devices is very heavily baked into the
                // structure.
                //
                // To work around this constraint, we forcibly create a source code manager and file manager here before
                // the compiler creates one and then use the method clang::SourceCodeManager::overrideFileContents
                // method to insert a memory buffer.  This works because the underlying compiler only creates the
                // file amanager and source code managers if they do not already exist.  Luckily the CLang code relies
                // on a std::unique_ptr to provide garbage collection allowing us to repeatedly recreate these classes
                // to embed new data each time we invoke the compiler.

                currentDiagnostics.clear();

                compilerInstance->getFrontendOpts().OutputFile = activeContext->objectFile().toStdString();

                clang::SourceManager& sourceManager = compilerInstance->getSourceManager();
                const char*           sourceBuffer  = activeContext->sourceData().data();
                sourceManager.overrideFileContents(
                    mainFileEntry.get(),
                    llvm::MemoryBuffer::getMemBuffer(sourceBuffer)
                );

                compilerStarted(activeContext);

                success = clang::ExecuteCompilerInvocation(compilerInstance.data());

                unsigned numberDiagnostics = static_cast<unsigned>(currentDiagnostics.size());
                unsigned diagnosticIndex   = 0;
                while (diagnosticIndex < numberDiagnostics) {
                    unsigned              nextDiagnosticIndex = diagnosticIndex + 1;
                    const DiagnosticData& diagnostic          = currentDiagnostics.at(diagnosticIndex);
                    QString               filename            = diagnostic.filename();
                    unsigned              lineNumber          = diagnostic.lineNumber();
                    unsigned              columnNumber        = diagnostic.columnNumber();
                    unsigned long         byteOffset          = diagnostic.byteOffset();

                    if (lineNumber == Cbe::Compiler::badLineNumber || columnNumber == Cbe::Compiler::badColumnNumber) {
                        if (nextDiagnosticIndex < numberDiagnostics) {
                            const DiagnosticData& nextDiagnostic = currentDiagnostics.at(nextDiagnosticIndex);
                            if (nextDiagnostic.diagnosticLevel() == Cbe::CompilerDiagnostic::Level::NOTE &&
                                nextDiagnostic.lineNumber() != Cbe::Compiler::badLineNumber              &&
                                nextDiagnostic.columnNumber() != Cbe::Compiler::badColumnNumber             ) {
                                filename     = nextDiagnostic.filename();
                                lineNumber   = nextDiagnostic.lineNumber();
                                columnNumber = nextDiagnostic.columnNumber();
                                byteOffset   = nextDiagnostic.byteOffset();

                                processDiagnostic(
                                    currentNotifier,
                                    activeContext,
                                    diagnostic.diagnosticLevel(),
                                    diagnostic.diagnosticCode(),
                                    diagnostic.diagnosticMessage(),
                                    filename,
                                    byteOffset,
                                    lineNumber,
                                    columnNumber
                                );

                                diagnosticIndex = nextDiagnosticIndex + 1;
                            } else {
                                diagnosticIndex = nextDiagnosticIndex;
                            }
                        } else {
                            diagnosticIndex = nextDiagnosticIndex;
                        }
                    } else {
                        processDiagnostic(
                            currentNotifier,
                            activeContext,
                            diagnostic.diagnosticLevel(),
                            diagnostic.diagnosticCode(),
                            diagnostic.diagnosticMessage(),
                            filename,
                            byteOffset,
                            lineNumber,
                            columnNumber
                        );

                        diagnosticIndex = nextDiagnosticIndex;
                    }
                }

                if (currentDebugOutputEnabled) {
                    for (diagnosticIndex=0 ; diagnosticIndex<numberDiagnostics ; ++diagnosticIndex) {
                        const DiagnosticData& diagnosticData = currentDiagnostics.at(diagnosticIndex);
                        qDebug() << tr("%1 %2 (%3): %4")
                                    .arg(diagnosticData.lineNumber(),6)
                                    .arg(diagnosticData.columnNumber(), 4)
                                    .arg(diagnosticData.filename(), diagnosticData.diagnosticMessage());
                    }
                }

                compilerFinished(activeContext, success);
            }

            activeContext.clear();
        }
    } while (!isTerminatingThread);
}


void CompilerImpl::fatalErrorHandlerCallback(void* /*userData */, const char* reason, bool generateCrashDiagnostics) {
    QSet<CompilerImpl*>::iterator it = compilers.begin();
    QSet<CompilerImpl*>::iterator end = compilers.end();

    while (it != end) {
        Cbe::CompilerNotifier* notifier = (*it)->currentNotifier;

        if (notifier != nullptr) {
            notifier->fatalCompilerError(QString::fromLocal8Bit(reason), generateCrashDiagnostics);
        }

        ++it;
    }
}


void CompilerImpl::configureCompilerBackend() {
    if (backendInitializationNeeded) {
        llvm::InitializeAllTargets();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmPrinters();
        llvm::InitializeAllAsmParsers();

        #if (defined(LINK_POLLY_INTO_TOOLS))

            llvm::PassRegistry& passRegistry = *llvm::PassRegistry::getPassRegistry();
            polly::initializePollyPasses(passRegistry);

        #endif

        llvm::install_fatal_error_handler(CompilerImpl::fatalErrorHandlerCallback);

        backendInitializationNeeded = false;
    }
}


QList<QString> CompilerImpl::addOptions(
        QList<QString>&       switchList,
        const QList<QString>& parameters,
        const QString&        switchText
    ) {
    for(QList<QString>::const_iterator it=parameters.constBegin(),end=parameters.constEnd() ; it!=end ; ++it) {
        switchList << switchText << *it;
    }

    return switchList;
}


bool CompilerImpl::buildUserFriendlyCommandLine() {
    clearUserFriendlyCommandLine();

    if (generateDefaultSwitches) {
        generateDefaultSwitches = false;
        currentSwitches = setDefaultSwitches();
    }

    QList<QString> switches;
    switches << "";
    switches += currentSwitches;

    if (!currentGccToolchainPrefix.isEmpty()) {
        switches << QString("--gcc-toolchain=\"%1\"").arg(currentGccToolchainPrefix);
    }

    addOptions(switches, currentHeaderSearchPaths, QString("-I"));

    addOptions(switches, currentPrecompiledHeaders, QString("-include-pch"));
    addOptions(switches, currentHeaders, QString("-include"));

    bool success = true;
    if (!temporaryInputFile.isOpen()) {
        success = temporaryInputFile.open();
    }

    if (success) {
        temporaryInputFilename = temporaryInputFile.fileName().toLocal8Bit();
        for (QList<QString>::const_iterator it=switches.begin(),end=switches.end() ; it!=end ; ++it) {
            currentUserCompilerSwitches.push_back(strdup(it->toLocal8Bit().constData()));
        }

        // clang driver apparently needs a filename when it sets up the compilation even if we just override it
        // afterwords.  We add this here to make clang happy.
        currentUserCompilerSwitches.push_back(strdup(temporaryInputFilename.constData()));
    }

    return success;
}


void CompilerImpl::clearUserFriendlyCommandLine() {
    QVector<char*>::iterator it  = currentUserCompilerSwitches.begin();
    QVector<char*>::iterator end = currentUserCompilerSwitches.end();

    while (it != end) {
        std::free(*it);
        ++it;
    }

    currentUserCompilerSwitches.clear();
}


bool CompilerImpl::reconfigureCompiler() {
    llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diagnosticIds(new clang::DiagnosticIDs());
    llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diagnosticOptions = new clang::DiagnosticOptions();

    DiagnosticConsumer* diagnosticConsumer = new DiagnosticConsumer(this);
    diagnosticsEngine.reset(new clang::DiagnosticsEngine(diagnosticIds, &*diagnosticOptions, diagnosticConsumer));

    std::string driverPath = "clang++"; // The driver class uses this to determine if we're ANSI-C or C++ mode.
    std::string triple =   currentTargetTripleOverride.isEmpty()
                         ? llvm::sys::getDefaultTargetTriple()
                         : currentTargetTripleOverride.toStdString();

    std::unique_ptr<clang::driver::Compilation> compilation;

    bool success = buildUserFriendlyCommandLine();
    if (success) {
        clang::driver::Driver driver(driverPath, triple, *diagnosticsEngine);

        char**   compilerOptions = currentUserCompilerSwitches.data();
        unsigned numberSwitches  = static_cast<unsigned>(currentUserCompilerSwitches.size());
        compilation.reset(driver.BuildCompilation(llvm::makeArrayRef(compilerOptions, numberSwitches)));
        success = (compilation && !compilation->containsError());
    }

    if (success) {
        const clang::driver::JobList& jobList = compilation->getJobs();
        unsigned jobListSize = static_cast<unsigned>(jobList.size());
        (void) jobListSize;
        assert(jobListSize >= 1);

        const clang::driver::Command&   command   = *jobList.begin();
        const llvm::opt::ArgStringList& arguments = command.getArguments();

        compilerInstance.reset(new clang::CompilerInstance());

        std::shared_ptr<clang::PCHContainerOperations> pchOperations = compilerInstance->getPCHContainerOperations();
        pchOperations->registerWriter(
            std::unique_ptr<clang::PCHContainerWriter>(new clang::ObjectFilePCHContainerWriter())
        );
        pchOperations->registerReader(
            std::unique_ptr<clang::PCHContainerReader>(new clang::ObjectFilePCHContainerReader())
        );

        success = clang::CompilerInvocation::CreateFromArgs(
            compilerInstance->getInvocation(),
            llvm::opt::ArgStringList(arguments.begin() + 1, arguments.end()),
            *diagnosticsEngine
        );

        compilerInstance->getInvocation().getFrontendOpts().LLVMArgs.clear();
    }

    if (success) {
        // The compiler invocation will include the "-disable-free" switch.  This switch causes the compiler to avoid
        // freeing memory constructs which, in turn, will cause memory leaks.  For a stand-alone compiler, the memory
        // leaks can be ignored because the OS will clean up for us.  For this application, the memory leaks rapidly
        // become an issue.  Rather than remove the "-disable-free", we override it here.

        compilerInstance->getFrontendOpts().DisableFree = 0;
        compilerInstance->getCodeGenOpts().DisableFree = 0;

        compilerInstance->getHeaderSearchOpts().Sysroot = currentSystemRoot.toStdString();

        if (currentResourceDirectory.isEmpty()) {
            compilerInstance->getHeaderSearchOptsPtr()->UseBuiltinIncludes = 0;
        } else {
            compilerInstance->getHeaderSearchOptsPtr()->UseBuiltinIncludes = 1;
            compilerInstance->getHeaderSearchOptsPtr()->ResourceDir = currentResourceDirectory.toStdString();
        }

        diagnosticConsumer = new DiagnosticConsumer(this);
        compilerInstance->createDiagnostics(diagnosticConsumer, true);
        success = compilerInstance->hasDiagnostics();
    }

    if (success) {
        if (!compilerInstance->hasFileManager()) {
            compilerInstance->createFileManager();
        }

        clang::FileManager& fileManager = compilerInstance->getFileManager();

        if (!compilerInstance->hasSourceManager()) {
            compilerInstance->createSourceManager(fileManager);
        }

        clang::SourceManager& sourceManager = compilerInstance->getSourceManager();

        mainFileEntry = fileManager.getFile(llvm::StringRef(temporaryInputFilename.constData()), true);
        assert(mainFileEntry);

        clang::FileID mainFileId = sourceManager.createFileID(
            mainFileEntry.get(),
            clang::SourceLocation(),
            clang::SrcMgr::C_User
        );
        sourceManager.setMainFileID(mainFileId);
    }

    return success;
}
