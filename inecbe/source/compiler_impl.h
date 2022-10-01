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
* This header defines the \ref CompilerImpl class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef COMPILER_IMPL_H
#define COMPILER_IMPL_H

#include <QString>
#include <QList>
#include <QByteArray>
#include <QThread>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QTemporaryFile>
#include <QMutex>
#include <QVector>
#include <QSet>

#include "warnings.h"

SUPPRESS_LLVM_WARNINGS

#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/FileEntry.h>
#include <llvm/Support/ErrorOr.h>

RESTORE_LLVM_WARNINGS

#include "cbe_common.h"
#include "cbe_source_range.h"
#include "cbe_compiler_context.h"
#include "cbe_compiler_diagnostic.h"
#include "cbe_job_queue.h"
#include "cbe_compiler.h"

namespace Cbe {
    class CompilerNotifier;
}

namespace clang {
    class CompilerInstance;
}

/**
 * Underlying implementation for the \ref Cbe::Compiler class.
 */
class CBE_PUBLIC_API CompilerImpl:public QThread {
    public:
        /**
         * Constructor
         *
         * \param[in] newNotifier A pointer to the notifier instance this compiler should send notification data to.
         *                        A null pointer will disable notifications.
         */
        CompilerImpl(Cbe::CompilerNotifier* newNotifier);

        ~CompilerImpl() override;

        /**
         * Method you can use to enable or disable debug output.
         *
         * \param[in] enableDebugOutput If true, debug output will be enabled.  If false, debug output will be
         *                              disabled.
         */
        void setDebugOutputEnabled(bool enableDebugOutput);

        /**
         * Method you can use to determine if debug output is enabled.
         *
         * \return Returns true if debug output is enabled.  Returns false if debug output is disabled.
         */
        bool debugOutputEnabled() const;

        /**
         * Method you can use to change the notifier receiving notifications from this compiler.  This method will
             * block until all pending contexts have been processed by the compiler.
         *
         * \param[in] newNotifier A pointer to the notifier instance this compiler should send notification data to.
         *                        A null pointer will disable notifications.
         */
        void setNotifier(Cbe::CompilerNotifier* newNotifier);

        /**
         * Method you can use to determine the current notifier receiving notifications from this compiler.
         *
         * \return Returns a pointer to the notifier.  A null pointer is returned if there is no notifier receiving
         *         notifications from the compiler.
         */
        Cbe::CompilerNotifier* notifier() const;

        /**
         * Method you can use to change the underlying job queue used to track jobs.  Note that, by default, a
         * \ref Cbe::SimpleJobQueue will be used.  You need only call this method if you wish to define your own
         * type of job queue.
         *
         * This method will block until all pending contexts have been processed by the compiler.
         *
         * \param[in] newJobQueue The new job queue used to track jobs.  Any previously registered job queue will be
         *                        destroyed.
         */
        void setJobQueue(QSharedPointer<Cbe::JobQueue<Cbe::CompilerContext>> newJobQueue);

        /**
         * Method you can call to determine if the compiler is an internal function or an external executable.
         *
         * \return Returns true if the compiler is an internal functions.  Returns false if the compiler is an
         *         external executable.
         */
        bool isInternal() const;

        /**
         * Method you can use to set the executable directory.  Call to this function will be ignored if the
         * compiler is an internal function.  If the compiler is external, you must call this function prior to
         * invoking the compiler.
         *
         * This method may block until all pending contexts have been processed by the compiler.
         *
         * \param[in] newExecutableDirectory The directory where the compiler executable is expected to be found.
         */
        void setExecutableDirectory(const QString& newExecutableDirectory);

        /**
         * Method you can use to determine the current executable directory.
         *
         * \return Returns the path to the executable directory.  An empty string will be returned if the compiler
         *         is an internal function.
         */
        QString executableDirectory() const;

        /**
         * Method you can use to obtain a list of command line switches being passed to the compiler engine.  The
         * list will exclude the input file and output file.
         *
         * Note that the switches are platform dependent and will be configured to a reasonable default value for
         * each platform.
         *
         * \return Returns a list of all the switches being passed to the underlying clang compiler instance.
         */
        QList<QString> compilerSwitches();

        /**
         * Method you can use to change the list of command line switches being passed to the compiler.  The list
         * should exclude the input file and output that are added independendy and managed through other
         * mechanisms.  Switches should match those used on the clang++ command line noting that the "-x c++" switch
         * should be included to indicate C++ input source.
         *
         * Note that the switches are platform dependent and will be configured to a reasonable default value for
         * each platform.
         *
         * This method will block until all pending contexts have been processed by the compiler.
         *
         * \param[in] newCompilerSwitches the updated list of compiler switches.
         */
        void setCompilerSwitches(const QList<QString>& newCompilerSwitches);

        /**
         * Method you can use to set the system root directory used to locate header and PCH files.
         *
         * This method will block until all pending contexts have been processed by the compiler.
         *
         * \param[in] newSystemRoot The new system root directory.
         */
        void setSystemRoot(const QString& newSystemRoot);

        /**
         * Method you can use to obitan the current system root directory used to locate header and PCH files.
         *
         * \return Returns the current system root directory.
         */
        QString systemRoot() const;

        /**
         * Method you can use to obtain the current list of header search paths.
         *
         * \return Returns a list of the header search paths to be used by the compiler.
         */
        QList<QString> headerSearchPaths() const;

        /**
         * Method you can use to change the current list of header search paths.
         *
         * This method will block until all pending contexts have been processed by the compiler.
         *
         * \param[in] newHeaderSearchPaths the new list of header search paths.
         */
        void setHeaderSearchPaths(const QList<QString>& newHeaderSearchPaths);

        /**
         * Method you can use to obtain the current list of explicitly included headers.  These headers will be
         * included in front of headers provided by the compiler context.
         *
         * \return Returns a list of the headers that should be explicitly included into the build.
         */
        QList<QString> headers() const;

        /**
         * Method you can use to change the list of explicitly included headers.  These headers will be included in
         * front of headers provided by the compiler context.
         *
         * This method will block until all pending contexts have been processed by the compiler.
         *
         * \param[in] newHeaders the new list of headers.
         */
        void setHeaders(const QList<QString>& newHeaders);

        /**
         * Method you can use to obtain the current list of explicitly included precompiled headers.  These
         * precompiled headers will be included in front of headers provided by the compiler context.
         *
         * \return Returns a list of the precompiled headers that should be explicitly included into the build.
         */
        QList<QString> precompiledHeaders() const;

        /**
         * Method you can use to change the list of explicitly included precompiled headers.  These precompiled
         * headers will be included in front of headers provided by the compiler context.
         *
         * This method will block until all pending contexts have been processed by the compiler.
         *
         * \param[in] newPrecompiledHeaders the new list of precompiled headers.
         */
        void setPrecompiledHeaders(const QList<QString>& newPrecompiledHeaders);

        /**
         * Method you can use to obtain the current resource directory.
         *
         * \return Returns the current resource directory.
         */
        QString resourceDirectory() const;

        /**
         * Method you can use to change the current resource directory.
         *
         * This method will block until all pending contexts have been processed by the compiler.
         *
         * \param[in] newResourceDirectory The new resource directory to be used.
         */
        void setResourceDirectory(const QString& newResourceDirectory);

        /**
         * Method you can use to determine the GCC standard library prefix.  Specifying that option will cause
         * the compiler to use GCC's c++ includes, libraries, etc. rather than CLANGs.
         *
         * \return Returns the current resource directory.
         */
        QString gccToolchain() const;

        /**
         * Method you can use to change the GCC toolchain prefix directory.  Setting this parameter to an empty
         * string will cause the LLVM/CLANG backends to use CLANG's libraries rather than GCC.
         *
         * This method will block until all pending contexts have been processed by the compiler.
         *
         * \param[in] newGccToolchainPrefix The new directory for the GCC toolchain.
         */
        void setGccToolchain(const QString& newGccToolchainPrefix);

        /**
         * Method you can use to obtain the current target triple override.
         *
         * \return Returns the current target triple.  An empty string indicates the compiler will guess a correct
         *         target triple for the platform.
         */
        QString targetTriple() const;

        /**
         * Method you can use to change the target triple.
         *
         * \param[in] newTargetTriple The new target triple to be used by the compiler.  An empty string will cause the
         *                            compiler to guess the correct target triple for the system.
         */
        void setTargetTriple(const QString& newTargetTriple);

        /**
         * Convenience method that can be called to run the compiler on a context.
         *
         * \param[in] context The compile context to be executed.
         */
        void compile(QSharedPointer<Cbe::CompilerContext> context);

        /**
         * Method that can be called to stall this thread until the compiler finishes.
         */
        void waitComplete();

        /**
         * Method that can be called to stall this thread until the compiler finishes.
         *
         * \param[in] maximumTimeInMilliseconds The maximum time to wait, in milliseconds.
         *
         * \return Returns true on success, returns false if the compiler timed out.
         */
        bool waitComplete(unsigned long maximumTimeInMilliseconds);

        /**
         * Method you can call to determine if the compiler is running in the background.
         *
         * \return Returns true if the compiler is active.  Returns false if the compiler is not active.
         */
        bool active() const;

        /**
         * Method that marshalls diagnostics to the interface.
         *
         * \param[in] diagnosticLevel       The diagnostic level.
         *
         * \param[in] diagnosticInformation The information associated with the diagnostic.
         */
        void reportDiagnostic(
            clang::DiagnosticsEngine::Level diagnosticLevel,
            const clang::Diagnostic&        diagnosticInformation
        );

    protected:
        /**
         * Method that runs the compiler back-end in the background.
         */
        void run() final;

    protected:
        /**
         * Pure virtual method that generates the default list of command line switches that would be issued to the
         * compiler.  Overload this method in derived classes to make specific compilers for C++, ANSI-C, AST, etc.
         *
         * \return Returns a default list of command line switches.  The command line switches should omit the
         *         headers, PCH files, header search paths, system root, input file, and output file.  These are
         *         added separately.
         */
        virtual QList<QString> setDefaultSwitches() const = 0;

        /**
         * Pure virtual method that is called when the compiler is started.  You should overload this method in
         * derived classes to provide notification to the notifier class and the context.
         *
         * Note that this method may be called from a different thread than was used to invoke the compiler.
         *
         * \param[in] context The context that is being executed.
         */
        virtual void compilerStarted(QSharedPointer<Cbe::CompilerContext> context) = 0;

        /**
         * Pure virtual method that is called when the compiler has completed processing a context.  You should
         * overload this method in derived classes to provide notification to the notifier class and the context.
         *
         * Note that this method may be called from a different thread than was used to invoke the compiler.
         *
         * \param[in] context    The context that is being executed.
         *
         * \param[in] successful Holds true of the compilation completed with no reported errors.  Holds false if an
         *                       error was detected.
         */
        virtual void compilerFinished(QSharedPointer<Cbe::CompilerContext> context, bool successful) = 0;

        /**
         * Pure virtual method that reports errors and other diagnostics information, ideally to a
         * \ref Cbe::CompilerNotifier class.  Classes should overload this method to provide proper reporting or
         * errors, warnings, and similar notifications.
         *
         * \param[in] notifier          A pointer to the notifier that should receive the notification.  Note that
         *                              this method will be called even if the supplied notifier pointer is null.
         *
         * \param[in] context           A shared pointer to the context the compiler is actively processing.  All
         *                              errors and location information will apply to this context.
         *
         * \param[in] diagnosticLevel   A value from the enumeration \ref Cbe::CompilerDiagnostic::Level indicating the
         *                              type of diagnostic being reported.
         *
         * \param[in] diagnosticCode    The compiler specific ID associated with the diagnostic.
         *
         * \param[in] diagnosticMessage A text based error message associated with the diagnostic.
         *
         * \param[in] filename          The filename where the error was found.  An empty string indicates that the
         *                              error was found in the data stored in the context.
         *
         * \param[in] byteOffset        A zero based offset into the source data where the error was detected.
         *
         * \param[in] lineNumber        A one based line number into the source data where the error was detected.
         *                              This value is only meaningful if the buffer contains textual information.
         *
         *
         * \param[in] columnNumber      A zero based column number into the source data where the error was detected.
         *                              This value is only meaningful if the buffer contains textual information.
         */
        virtual void processDiagnostic(
            Cbe::CompilerNotifier*               notifier,
            QSharedPointer<Cbe::CompilerContext> context,
            Cbe::CompilerDiagnostic::Level       diagnosticLevel,
            Cbe::CompilerDiagnostic::Code        diagnosticCode,
            const QString&                       diagnosticMessage,
            const QString&                       filename,
            unsigned                             byteOffset,
            unsigned                             lineNumber,
            unsigned                             columnNumber
        ) = 0;

    private:
        /**
         * Value holding the required stack for the compiler thread.
         */
        static const unsigned long requiredStackSpace;

        /**
         * Class used to store diagnostic information.
         */
        class DiagnosticData {
            public:
                /**
                 * Constructor
                 *
                 * \param[in] diagnosticLevel   The diagnostic level.
                 *
                 * \param[in] diagnosticCode    The diagnostic code.
                 *
                 * \param[in] diagnosticMessage The diagnostic message.
                 *
                 * \param[in] filename          The filename of the file where the error was reported.
                 *
                 * \param[in] byteOffset        The byte offset into the file.
                 *
                 * \param[in] lineNumber        The line number into the file.
                 *
                 * \param[in] columnNumber      The column number into the file.
                 */
                DiagnosticData(
                        Cbe::CompilerDiagnostic::Level diagnosticLevel,
                        Cbe::CompilerDiagnostic::Code  diagnosticCode,
                        const QString&                 diagnosticMessage,
                        const QString&                 filename,
                        unsigned long                  byteOffset,
                        unsigned                       lineNumber,
                        unsigned                       columnNumber
                    ):impl(
                        new Private(
                            diagnosticLevel,
                            diagnosticCode,
                            diagnosticMessage,
                            filename,
                            byteOffset,
                            lineNumber,
                            columnNumber
                        )
                    ) {}

                /**
                 * Copy constructor
                 *
                 * \param[in] other The value to be copied.
                 */
                DiagnosticData(const DiagnosticData& other):impl(other.impl) {}

                /**
                 * Method that returns the diagnostic level.
                 *
                 * \return Returns teh diagnostic level.
                 */
                inline Cbe::CompilerDiagnostic::Level diagnosticLevel() const {
                    return impl->diagnosticLevel();
                }

                /**
                 * Method that returns the diagnostic code.
                 *
                 * \return Returns the diagnostic code.
                 */
                inline Cbe::CompilerDiagnostic::Code diagnosticCode() const {
                    return impl->diagnosticCode();
                }

                /**
                 * Method that returns the diagnostic message.
                 *
                 * \return Returns the diagnostic message.
                 */
                inline const QString& diagnosticMessage() const {
                    return impl->diagnosticMessage();
                }

                /**
                 * Method that returns the file associated with the diagnostic.
                 *
                 * \return Returns the file associated with
                 */
                inline const QString& filename() const {
                    return impl->filename();
                }

                /**
                 * Method that returns the byte offset into the file tied to the diagnostic.
                 *
                 * \return Returns the byte offset into the file associated with the diagnostic.
                 */
                inline unsigned long byteOffset() const {
                    return impl->byteOffset();
                }

                /**
                 * Method that returns the line number into the file where the diagnostic is reported.
                 *
                 * \return Returns the line number associated with the diagnostic.
                 */
                inline unsigned lineNumber() const {
                    return impl->lineNumber();
                }

                /**
                 * Method that returns the column index into the file where the diagnostic is reported.
                 *
                 * \return Returns the column index associated with the diagnostic.
                 */
                inline unsigned columnNumber() const {
                    return impl->columnNumber();
                }

                /**
                 * Assignment operator
                 *
                 * \param[in] other The instance to assign to this instance.
                 *
                 * \return Returns a reference to this instance.
                 */
                inline DiagnosticData& operator=(const DiagnosticData& other) {
                    impl = other.impl;
                    return *this;
                }

            private:
                /**
                 * Private implementation.
                 */
                class Private {
                    private:
                        /**
                         * The current diagnostic level.
                         */
                        Cbe::CompilerDiagnostic::Level currentDiagnosticLevel;

                        /**
                         * The current diagnostic code.
                         */
                        Cbe::CompilerDiagnostic::Code  currentDiagnosticCode;

                        /**
                         * The diagnostic message.
                         */
                        QString currentDiagnosticMessage;

                        /**
                         * The diagnostic filename.
                         */
                        QString currentFilename;

                        /**
                         * The byte offset into the file.
                         */
                        unsigned long currentByteOffset;

                        /**
                         * The line number into the file.
                         */
                        unsigned currentLineNumber;

                        /**
                         * The column index into the file.
                         */
                        unsigned currentColumnNumber;

                    public:
                        /**
                         * Constructor
                         *
                         * \param[in] diagnosticLevel   The diagnostic level.
                         *
                         * \param[in] diagnosticCode    The diagnostic code.
                         *
                         * \param[in] diagnosticMessage The diagnostic message.
                         *
                         * \param[in] filename          The filename of the file where the error was reported.
                         *
                         * \param[in] byteOffset        The byte offset into the file.
                         *
                         * \param[in] lineNumber        The line number into the file.
                         *
                         * \param[in] columnNumber      The column number into the file.
                         */
                        Private(
                                Cbe::CompilerDiagnostic::Level diagnosticLevel,
                                Cbe::CompilerDiagnostic::Code  diagnosticCode,
                                const QString&                 diagnosticMessage,
                                const QString&                 filename,
                                unsigned long                  byteOffset,
                                unsigned                       lineNumber,
                                unsigned                       columnNumber
                            ) {
                            currentDiagnosticLevel   = diagnosticLevel;
                            currentDiagnosticCode    = diagnosticCode;
                            currentDiagnosticMessage = diagnosticMessage;
                            currentFilename          = filename;
                            currentByteOffset        = byteOffset;
                            currentLineNumber        = lineNumber;
                            currentColumnNumber      = columnNumber;
                        }

                        /**
                         * Method that returns the diagnostic level.
                         *
                         * \return Returns teh diagnostic level.
                         */
                        inline Cbe::CompilerDiagnostic::Level diagnosticLevel() const {
                            return currentDiagnosticLevel;
                        }

                        /**
                         * Method that returns the diagnostic code.
                         *
                         * \return Returns the diagnostic code.
                         */
                        inline Cbe::CompilerDiagnostic::Code diagnosticCode() const {
                            return currentDiagnosticCode;
                        }

                        /**
                         * Method that returns the diagnostic message.
                         *
                         * \return Returns the diagnostic message.
                         */
                        inline const QString& diagnosticMessage() const {
                            return currentDiagnosticMessage;
                        }

                        /**
                         * Method that returns the file associated with the diagnostic.
                         *
                         * \return Returns the file associated with
                         */
                        inline const QString& filename() const {
                            return currentFilename;
                        }

                        /**
                         * Method that returns the byte offset into the file tied to the diagnostic.
                         *
                         * \return Returns the byte offset into the file associated with the diagnostic.
                         */
                        inline unsigned long byteOffset() const {
                            return currentByteOffset;
                        }

                        /**
                         * Method that returns the line number into the file where the diagnostic is reported.
                         *
                         * \return Returns the line number associated with the diagnostic.
                         */
                        inline unsigned lineNumber() const {
                            return currentLineNumber;
                        }

                        /**
                         * Method that returns the column index into the file where the diagnostic is reported.
                         *
                         * \return Returns the column index associated with the diagnostic.
                         */
                        inline unsigned columnNumber() const {
                            return currentColumnNumber;
                        }
                };

                QSharedPointer<Private> impl;
        };

        /**
         * Static method that is used as a handler for fatal errors in the LLVM/CLang compiler.
         *
         * \param[in] dummy                    A dummy pointer.
         *
         * \param[in] reason                   String indicating the reason for the fatal error.
         *
         * \param[in] generateCrashDiagnostics If true, we should generate crash diagnostics for this fatal error.
         */
        static void fatalErrorHandlerCallback(void* userData, const char* reason, bool generateCrashDiagnostics);

        /**
         * Method that is called to configure the compiler backend.
         */
        static void configureCompilerBackend();

        /**
         * Method that adds a group of files or paths, with associated switches to a list of switches.
         *
         * \param[in] switchList List of switches to add new entries to.
         *
         * \param[in] parameters Additional parameters, each parameter will be coupled with a switch.
         *
         * \param[in] switchText The switch to prepend in front of each parameter.
         *
         * \return returns a reference to the switch list.
         */
        QList<QString> addOptions(
            QList<QString>&       switchList,
            const QList<QString>& parameters,
            const QString&        switchText
        );

        /**
         * Method that sets up the external compiler command line passed to the clang::driver::Compilation instance.
         *
         * \return Returns true on success.  Returns false on error.
         */
        bool buildUserFriendlyCommandLine();

        /**
         * Method that cleans up memory used by the user friendly compiler switches.
         */
        void clearUserFriendlyCommandLine();

        /**
         * Method that is called to reconfigure the compiler.  This method should be called any time a compiler setting
         * is changed.
         *
         * \return Returns true on success, returns false on error.
         */
        bool reconfigureCompiler();

        /**
         * Shared pointer to the compiler's job queue.
         */
        QSharedPointer<Cbe::JobQueue<Cbe::CompilerContext>> jobQueue;

        /**
         * Mutex used to protect the job queue during enqueue and dequeue operations.
         */
        QMutex jobQueueMutex;

        /**
         * Flag used by the compiler background thread to indicate that it's in the process of terminating.
         */
        volatile bool isTerminatingThread;

        /**
         * The compiler context that is actively being processed by the compiler.  Used to assist with error reporting.
         */
        QSharedPointer<Cbe::CompilerContext> activeContext;

        /**
         * Method that holds the persistent compiler instance.
         */
        QScopedPointer<clang::CompilerInstance> compilerInstance;

        /**
         * Method that holds a persistent instance of the diagnostic engine used to report status.
         */
        QScopedPointer<clang::DiagnosticsEngine> diagnosticsEngine;

        /**
         * Mutex used to keep compiler invocations safe across threads.
         */
        QMutex compilerAccessMutex;

        /**
         * A temporary file used to trick the compiler into not erroring out before we can get it property configured.
         * The temporary file will be empty and remain alive from the first compilation attempt until the class instance
         * is destroyed.
         */
        QTemporaryFile temporaryInputFile;

        /**
         * Buffer holding the current temporary input filename.
         */
        QByteArray temporaryInputFilename;

        /**
         * Set pointing to all the defined instances of \ref CompilerImpl.  This set is used for fatal error
         * handling.
         */
        static QSet<CompilerImpl*> compilers;

        /**
         * The compiler's main file instance.
         */
        llvm::ErrorOr<const clang::FileEntry*> mainFileEntry;

        /**
         * Boolean used to indicate if \ref CompilerImpl::setDefaultSwitches needs to be called to obtain a list of
         * default switches.
         */
        bool generateDefaultSwitches;

        /**
         * Flag indicating if debug output should be enabled.
         */
        bool currentDebugOutputEnabled;

        /**
         * The current list of compiler switches.
         */
        QList<QString> currentSwitches;

        /**
         * The current system root directory.
         */
        QString currentSystemRoot;

        /**
         * The current list of header search paths.
         */
        QList<QString> currentHeaderSearchPaths;

        /**
         * The current list of header files to be included into the build.
         */
        QList<QString> currentHeaders;

        /**
         * The current list of precompiled headers to be included into the build.
         */
        QList<QString> currentPrecompiledHeaders;

        /**
         * The current resource directory.
         */
        QString currentResourceDirectory;

        /**
         * The current GCC toolchain prefix.
         */
        QString currentGccToolchainPrefix;

        /**
         * The current target triple override
         */
        QString currentTargetTripleOverride;

        /**
         * Vector holding the user's command line switches.  The compiler maintains a lot of string values by reference
         * forcing us to maintain persistent copies of the data.
         */
        QVector<char*> currentUserCompilerSwitches;

        /**
         * Flag used to determine if the LLVM backend needs to be initialized.
         */
        static bool backendInitializationNeeded;

        /**
         * The notifier that receives information about the compilation operation.
         */
        Cbe::CompilerNotifier* currentNotifier;

        /**
         * The current list of reported diagnostics.
         */
        QList<DiagnosticData> currentDiagnostics;
};

#endif
