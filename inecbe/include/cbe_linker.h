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
* This header defines the \ref Cbe::Linker class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_LINKER_H
#define CBE_LINKER_H

#include <QString>
#include <QList>
#include <QSharedPointer>

#include "cbe_common.h"
#include "cbe_job_queue.h"
#include "cbe_linker_context.h"
#include "cbe_source_range.h"

namespace Cbe {
    class LinkerContext;
    class LinkerNotifier;

    CBE_PUBLIC_TEMPLATE_CLASS(Cbe::JobQueue<LinkerContext>)

    /**
     * Class that provides a wrapper for the LLVM linker.  You can use this class to invoke the linker directly from
     * your application.
     *
     * Rather than spawning an instance of the linker each time it's needed, the class implements a modified version of
     * the LLVM lld front-end.  The reasons for this approach is similar to the reasons used for embedding the clang++
     * compiler as a memory resident dynamic library.
     *
     * Like the \ref Cbe::Compiler class, this class uses a Pimpl design pattern to both allow passing by value as well
     * as to prevent lots of name space polution due to all the defines in headers needed for the LLVM backend. The
     * pimpl design pattern should also speed-up compilation somewhat.
     *
     * Note that the underlying linker implementation is expected to take care of including standard system libraries
     * required for normal operation.  You should not need to include library search paths such as /usr/lib nor
     * libraries such as the MacOS libSystem.dylib.
     */
    class CBE_PUBLIC_API Linker {
        public:
            /**
             * Value indicating the default MacOS SDK major version we're linking against.
             */
            static constexpr unsigned defaultMacOsSdkVersionMajor = 10;

            /**
             * Value indicating the default MacOS SDK minor version we're linking against.
             */
            static constexpr unsigned defaultMacOsSdkVersionMinor = 10;

            /**
             * Constructor
             *
             * \param[in] newNotifier A pointer to the notifier instance this linker should send notification data to.
             *                        A null pointer will disable notifications.
             */
            Linker(LinkerNotifier* newNotifier = nullptr);

            virtual ~Linker();

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
             * This method will block until all pending contexts have been processed by the linker.
             *
             * \param[in] newJobQueue The new job queue used to track jobs.  Any previously registered job queue will be
             *                        destroyed.
             */
            void setJobQueue(QSharedPointer<JobQueue<LinkerContext>> newJobQueue);

            /**
             * Convenience method you can use to change the underlying job queue used to track jobs.  Note that, by
             * default, a \ref Cbe::SimpleJobQueue will be used.  You need only call this method if you wish to define
             * your own type of job queue.
             *
             * This method will block until all pending contexts have been processed by the linker.
             *
             * \param[in] newJobQueue A pointer to the job queue used to track jobs.  Note that this class will take
             *                        ownership of the job queue.  Any previously registered job queue will be
             *                        destroyed.
             */
            void setJobQueue(JobQueue<LinkerContext>* newJobQueue);

            /**
             * Method you can use to change the notifier receiving notifications from this linker.
             *
             * \param[in] newNotifier A pointer to the notifier instance this linker should send notification data to.
             *                        A null pointer will disable notifications.
             */
            void setNotifier(LinkerNotifier* newNotifier);

            /**
             * Method you can use to determine the current notifier receiving notifications from this linker.
             *
             * \return Returns a pointer to the notifier.  A null pointer is returned if there is no notifier receiving
             *         notifications from the linker.
             */
            LinkerNotifier* notifier() const;

            /**
             * Method you can call to determine if the linker is an internal function or an external executable.
             *
             * \return Returns true if the linker is an internal functions.  Returns false if the linker is an external
             *         executable.
             */
            bool isInternal() const;

            /**
             * Method you can call to determine if the linker is an external executable or an internal function.
             *
             * \return Returns true if the linker is an external executable.  Returns false if the linker is an internal
             *         function.
             */
            bool isExternal() const;

            /**
             * Method you can use to set the executable directory.  Call to this function will be ignored if the linker
             * is an internal function.  If the linker is external, you must call this function prior to invoking the
             * linker.
             *
             * \param[in] newExecutableDirectory The directory where the linker executable is expected to be found.
             */
            void setExecutableDirectory(const QString& newExecutableDirectory);

            /**
             * Method you can use to determine the current executable directory.
             *
             * \return Returns the path to the executable directory.  An empty string will be returned if the linker is
             *         an internal function.
             */
            QString executableDirectory() const;

            /**
             * Method you can use to set the executable name.  Calls to this function will be ignored if the linker is
             * an internal function.  Note that a default external linker will be used if this method is not called.
             *
             * \param[in] linkerExecutable The name of the linker executable to be used.
             */
            void setLinkerExecutable(const QString& linkerExecutable);

            /**
             * Method you can use to determine the name of the linker executable.  An empty string will be returned if
             * an internal linker is being used.
             *
             * \return Returns the name of the external linker executable being used.
             */
            QString linkerExecutable() const;

            /**
             * Method you can use to obtain a list of command line switches being passed to the linker.  The list will
             * exclude input files and libraries as well as the output file.
             *
             * Note that the switches are platform dependent and will be configured to a reasonable default value for
             * each platform.
             *
             * \return Returns a list of all the switches being passed to the underlying linker instance.
             */
            QList<QString> linkerSwitches() const;

            /**
             * Method you can use to change the list of switches being passed to the linker.  The list should exclude
             * the input file and output that are added independendy and managed through other mechanisms.
             *
             * Note that the switches are platform dependent and will be configured to a reasonable default value for
             * each platform.
             *
             * \param[in] newLinkerSwitches the updated list of linker switches.
             */
            void setLinkerSwitches(const QList<QString>& newLinkerSwitches);

            /**
             * Method you can use to set the system root directory used to locate libraries.  Note that this feature is
             * emulated on Windows.
             *
             * \param[in] newSystemRoot The new system root directory.
             */
            void setSystemRoot(const QString& newSystemRoot);

            /**
             * Method you can use to obitan the current system root directory used to locate libraries.  Note that this
             * feature is emulated on Windows.
             *
             * \return Returns the current system root directory.
             */
            QString systemRoot() const;

            /**
             * Method you can use to obtain the current list of application specific library search paths.
             *
             * \return Returns a list of the library search paths to be used by the linker.
             */
            QList<QString> librarySearchPaths() const;

            /**
             * Method you can use to change the current list of application specific library search paths.
             *
             * \param[in] newLibrarySearchPaths the new list of library search paths.
             */
            void setLibrarySearchPaths(const QList<QString>& newLibrarySearchPaths);

            /**
             * Method you can use to obtain the current list of explicitly included application specific static
             * libraries.
             *
             * \return Returns a list of the static libraries that should be explicitly included into the build.
             */
            QList<QString> staticLibraries() const;

            /**
             * Method you can use to change the list of explicitly included application specific static libraries.
             *
             * \param[in] newStaticLibraries the new list of static libraries.
             */
            void setStaticLibraries(const QList<QString>& newStaticLibraries);

            /**
             * Method you can use to obtain the current list of explicitly included application specific dynamic
             * libraries.
             *
             * \return Returns a list of the dynamic libraries that should be explicitly included into the build.
             */
            QList<QString> dynamicLibraries() const;

            /**
             * Method you can use to change the list of explicitly included application specific dynamic libraries.
             *
             * \param[in] newDynamicLibraries the new list of dynamic libraries.
             */
            void setDynamicLibraries(const QList<QString>& newDynamicLibraries);

            /**
             * Method you can use to change the MacOS SDK version number we're linking against.
             *
             * \param[in] majorVersionNumber The new MacOS SDK major version number.
             *
             * \param[in] minorVersionNumber The new MacOS SDK minor version number.
             */
            void setMacOsSdkVersion(unsigned majorVersionNumber, unsigned minorVersionNumber);

            /**
             * Method you can use to obtain the MacOS SDK major version number we're linking against.
             *
             * \return Returns the MacOS SDK major version number.  A value of 0 is returned if this parameter is not
             *         used.
             */
            unsigned macOsSdkMajorVersion() const;

            /**
             * Method you can use to obtain the MacOS SDK minor version number we're linking against.
             *
             * \return Returns the MacOS SDK minor version number.  A value of 0 is returned if this parameter is not
             *         used.
             */
            unsigned macOsSdkMinorVersion() const;

            /**
             * Method you can use to obtain the current list of run-time search paths to be used by the linked object.
             *
             * \return Returns a list of the run-time paths to be used by the linked object.  Note that this method will
             *         always return an empty list on Windows.
             */
            QList<QString> runTimeSearchPaths() const;

            /**
             * Method you can use to change the list of run-time search paths to be used by the linked object.  This
             * method provides capabilities similar to the --rpath switch on Linux and -rpath switch on MacOS.  The
             * call is ignored on Windows.
             *
             * \param[in] newRunTimeSearchPaths The new list of run-time search paths.
             */
            void setRunTimeSearchPaths(const QList<QString>& newRunTimeSearchPaths);

            /**
             * Method that can be called to run the linker.
             *
             * \param[in] context The linker context to be executed.
             */
            void link(QSharedPointer<LinkerContext> context);

            /**
             * Convenience method that can be called to run the linker.
             *
             * \param[in] context The linker context to be executed.
             */
            void link(LinkerContext* context);

            /**
             * Method that can be called to stall this thread until the linker finishes.
             */
            void waitComplete();

            /**
             * Method that can be called to stall this thread until the linker finishes.
             *
             * \param[in] maximumTimeInMilliseconds The maximum time to wait, in milliseconds.
             *
             * \return Returns true on success, returns false if the linker timed out.
             */
            bool waitComplete(unsigned long maximumTimeInMilliseconds);

            /**
             * Method you can call to determine if the linker is running in the background.
             *
             * \return Returns true if the linker is active.  Returns false if the linker is not active.
             */
            bool active() const;

            /**
             * Method you can use to specify the location and name of the system library.  The value is passed to the
             * linker.  You can use this method if you need to override the default settings.
             *
             * \param[in] systemLibraries List of non-default system libraries to be used.
             */
            void setSystemLibraries(const QList<QString>& systemLibraries);

            /**
             * Method you can use to determine the system libraries being passed to to the linker.
             *
             * \return Returns the current list of system libraries.
             */
            QList<QString> systemLibraries() const;

        protected:
            /**
             * Pure virtual method that generates the default list of command line switches that would be issued to the
             * linker backend.  Overload this method in derived classes to customize the linker behaviour.
             *
             * \return Returns a default list of command line switches.  The command line switches should omit inputs,
             *         libraries, and the output file as these are added separately.
             */
            virtual QList<QString> setDefaultSwitches() const;

            /**
             * Virtual method you can overload to receive notification when the linker is started.  Note that the
             * method may be called from a different thread than the one used to invoke the linker.
             *
             * The default implementation reports the state to the notifier and context.
             *
             * \param[in] context The context that is being executed.
             */
            virtual void linkerStarted(QSharedPointer<LinkerContext> context);

            /**
             * Virtual method you can overload to receive notification when the linker has finished.  Note that the
             * method may be called from a different thread than the one used to invoke the linker.
             *
             * The default implementation reports the state to the notifier and context.
             *
             * \param[in] context The context that is being executed.
             *
             * \param[in] success Holds true if the linker completed successfully, returns false if an error is
             *                    reported.
             */
            virtual void linkerFinished(QSharedPointer<LinkerContext> context, bool success);

            /**
             * Virtual method you can overload to receive notification of diagnostic messages from the linker.  Note
             * that the method may be called from a different thread than the one used to invoke the linker.
             *
             * The default implementation reports the state to the notifier and context.
             *
             * \param[in] context           The context that is being executed.
             *
             * \param[in] diagnosticMessage The diagnostic message generated by the linker.
             */
            virtual void handleLinkerDiagnostic(
                QSharedPointer<LinkerContext> context,
                const QString&                diagnosticMessage
            );

        private:
            class CBE_PUBLIC_API Private;

            QSharedPointer<Private> impl;
    };
};

#endif
