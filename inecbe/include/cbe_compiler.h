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
* This header defines the \ref Cbe::Compiler class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_COMPILER_H
#define CBE_COMPILER_H

#include <QString>
#include <QList>
#include <QByteArray>
#include <QSharedPointer>

#include "cbe_common.h"
#include "cbe_source_range.h"
#include "cbe_compiler_context.h"
#include "cbe_compiler_diagnostic.h"
#include "cbe_job_queue.h"

namespace Cbe {
    class CompilerNotifier;

    CBE_PUBLIC_TEMPLATE_CLASS(Cbe::JobQueue<CompilerContext>)

    /**
     * Class that provides a wrapper for the CLang compiler.  You can use this class as a common base class for the
     * various compiler options provided by the CLang backend.
     *
     * The class is inspired by the CLang source.  Rather than spawning an instance of clang++, the class actually
     * implements its own version of the CLang front end.  This approach was taken for several key reasons:
     *
     *     * Speed:
     *       - The clang executable in fact spawns itself with a modified set of build switches.  The clang executable
     *         also optionally spawns a linker to dynamically bind the generated object to any static or shared
     *         libraries.  Given how often the compiler is to be triggered, we want to reduce the overhead required to
     *         perform compilation and/or linking.
     *
     *     * Memory resident data:
     *       - We want to avoid pushing data into temporary files whenever possible.  Goal of this class is to allow
     *         both the source code and the generated object to remain memory resident.
     *
     *     * Control:
     *       - The CLANG compiler will invoke several executables behind the scenes, including a linker.  In some cases,
     *         if a third party linker, such as the Microsoft linker, is present, CLang will invoke that linker rather
     *         than the linker supplied with the application.  We want to avoid this situation as it could leave to
     *         unexpected results.
     *
     * This class uses a Pimpl design pattern to both allow passing by value as well as to prevent lots of name space
     * polution due to all the defines in headers needed for the LLVM and CLang backends.  The pimpl design pattern
     * should also speed-up compilation somewhat.
     */
    class CBE_PUBLIC_API Compiler {
        friend class CompilerNotifier;

        public:
            /**
             * Value used to indicate an invalid line number.
             */
            static constexpr unsigned badLineNumber = static_cast<unsigned>(-1);

            /**
             * Value used to indicate an invalid column number.
             */
            static constexpr unsigned badColumnNumber = static_cast<unsigned>(-1);

            /**
             * Constructor
             *
             * \param[in] newNotifier A pointer to the notifier instance this compiler should send notification data to.
             *                        A null pointer will disable notifications.
             */
            Compiler(CompilerNotifier* newNotifier = nullptr);

            virtual ~Compiler();

            /**
             * Method you can use to enable or disable debug output.
             *
             * \param[in] enableDebugOutput If true, debug output will be enabled.  If false, debug output will be
             *                              disabled.
             */
            void setDebugOutputEnabled(bool enableDebugOutput = true);

            /**
             * Method you can use to disable or enable debug output.
             *
             * \param[in] disableDebugOutput If true, debug output will be disbled.  If false, debug output will be
             *                               enabled.
             */
            void setDebugOutputDisabled(bool disableDebugOutput = true);

            /**
             * Method you can use to determine if debug output is enabled.
             *
             * \return Returns true if debug output is enabled.  Returns false if debug output is disabled.
             */
            bool debugOutputEnabled() const;

            /**
             * Method you can use to determine if debug output is disabled.
             *
             * \return Returns true if debug output is disabled.  Returns false if debug output is enabled.
             */
            bool debugOutputDisabled() const;

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
            void setJobQueue(QSharedPointer<JobQueue<CompilerContext>> newJobQueue);

            /**
             * Convenience method you can use to change the underlying job queue used to track jobs.  Note that, by
             * default, a \ref Cbe::SimpleJobQueue will be used.  You need only call this method if you wish to define
             * your own type of job queue.
             *
             * This method will block until all pending contexts have been processed by the compiler.
             *
             * \param[in] newJobQueue A pointer to the job queue used to track jobs.  Note that this class will take
             *                        ownership of the job queue.  Any previously registered job queue will be
             *                        destroyed.
             */
            void setJobQueue(JobQueue<CompilerContext>* newJobQueue);

            /**
             * Method you can call to determine if the compiler is an internal function or an external executable.
             *
             * \return Returns true if the compiler is an internal functions.  Returns false if the compiler is an
             *         external executable.
             */
            bool isInternal() const;

            /**
             * Method you can call to determine if the compiler is an external executable or an internal function.
             *
             * \return Returns true if the compiler is an external executable.  Returns false if the compiler is an
             *         internal function.
             */
            bool isExternal() const;

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
            QList<QString> compilerSwitches() const;

            /**
             * Method you can use to change the list of command line switches being passed to the compiler.  The list
             * should exclude the input file and output that are added independendy and managed through other
             * mechanisms.
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
             * Method that can be called to run the compiler on a context.
             *
             * \param[in] context The compile context to be executed.
             */
            void compile(QSharedPointer<CompilerContext> context);

            /**
             * Convenience method that can be called to run the compiler on a context.
             *
             * \param[in] context The compile context to be executed.  This method will take ownership of the context.
             *
             * \return Returns true if the context was queued and the compiler was successfully started, returns false
             *         if the compiler could not be successfully started.
             */
            void compile(CompilerContext* context);

            /**
             * Method that can be called to stall this thread until the compiler finishes all outstanding contexts.
             */
            void waitComplete();

            /**
             * Method that can be called to stall this thread until the compiler finishes all outstanding contexts.
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

        protected:
            /**
             * Method you can use to change the notifier receiving notifications from this compiler.  This method will
             * block until all pending contexts have been processed by the compiler.
             *
             * \param[in] newNotifier A pointer to the notifier instance this compiler should send notification data to.
             *                        A null pointer will disable notifications.
             */
            void setNotifier(CompilerNotifier* newNotifier);

            /**
             * Method you can use to determine the current notifier receiving notifications from this compiler.
             *
             * \return Returns a pointer to the notifier.  A null pointer is returned if there is no notifier receiving
             *         notifications from the compiler.
             */
            CompilerNotifier* notifier() const;

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
            virtual void compilerStarted(QSharedPointer<CompilerContext> context) = 0;

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
            virtual void compilerFinished(QSharedPointer<CompilerContext> context, bool successful) = 0;

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
             * \param[in] diagnosticLevel   A value from the enumeration \ref CompilerDiagnostic::Level indicating the
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
             * \param[in] columnNumber      A zero based column number into the source data where the error was detected.
             *                              This value is only meaningful if the buffer contains textual information.
             */
            virtual void processDiagnostic(
                CompilerNotifier*               notifier,
                QSharedPointer<CompilerContext> context,
                CompilerDiagnostic::Level       diagnosticLevel,
                CompilerDiagnostic::Code        diagnosticCode,
                const QString&                  diagnosticMessage,
                const QString&                  filename,
                unsigned                        byteOffset,
                unsigned                        lineNumber,
                unsigned                        columnNumber
            ) = 0;

        private:
            class CBE_PUBLIC_API Private;

            QSharedPointer<Private> impl;
    };
};

#endif
