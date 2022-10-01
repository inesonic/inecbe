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
* This header defines the \ref LinkerImplInternal class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef LINKER_IMPL_INTERNAL_H
#define LINKER_IMPL_INTERNAL_H

#include <QString>
#include <QList>
#include <QThread>
#include <QVector>
#include <QSharedPointer>
#include <QMutex>

#include "cbe_common.h"
#include "cbe_job_queue.h"
#include "cbe_linker_context.h"
#include "cbe_linker_notifier.h"
#include "cbe_linker.h"

namespace Cbe {
    class LinkerNotifier;
}

/**
 * Underlying implementation of the \ref Cbe::Linker class.  This version implements the linker directly within the
 * CBE DLL.
 *
 * This class is coded to operate across Windows, Linux, and MacOS although the Windows COFF linker exhibits several bad
 * behaviours making the internal linker implementation unusable at this time.
 */
class CBE_PUBLIC_API LinkerImplInternal:public QThread {
    public:
        /**
         * Constructor
         *
         * \param[in] newNotifier A pointer to the notifier instance this linker should send notification data to.  A
         *                        null pointer will disable notifications.
         */
        LinkerImplInternal(Cbe::LinkerNotifier* newNotifier);

        ~LinkerImplInternal() override;

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
         * Method you can use to change the underlying job queue used to track jobs.  Note that, by default, a
         * \ref Cbe::SimpleJobQueue will be used.  You need only call this method if you wish to define your own
         * type of job queue.
         *
         * This method will block until all pending contexts have been processed by the linker.
         *
         * \param[in] newJobQueue The new job queue used to track jobs.  Any previously registered job queue will be
         *                        destroyed.
         */
        void setJobQueue(QSharedPointer<Cbe::JobQueue<Cbe::LinkerContext>> newJobQueue);

        /**
         * Method you can use to change the notifier receiving notifications from this linker.
         *
         * \param[in] newNotifier A pointer to the notifier instance this linker should send notification data to.
         *                        A null pointer will disable notifications.
         */
        void setNotifier(Cbe::LinkerNotifier* newNotifier);

        /**
         * Method you can use to determine the current notifier receiving notifications from this linker.
         *
         * \return Returns a pointer to the notifier.  A null pointer is returned if there is no notifier receiving
         *         notifications from the linker.
         */
        Cbe::LinkerNotifier* notifier() const;

        /**
         * Method you can call to determine if the linker is an internal function or an external executable.
         *
         * \return Returns true if the linker is an internal functions.  Returns false if the linker is an external
         *         executable.
         */
        bool isInternal() const;

        /**
         * Method you can use to set the executable directory.  Call to this function will be ignored if the linker is
         * an internal function.  If the linker is external, you must call this function prior to invoking the linker.
         *
         * \param[in] newExecutableDirectory The directory where the linker executable is expected to be found.
         */
        void setExecutableDirectory(const QString& newExecutableDirectory);

        /**
         * Method you can use to determine the current executable directory.
         *
         * \return Returns the path to the executable directory.  An empty string will be returned if the linker is an
         *         internal function.
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
         * Note that the switches are platform dependent and will be configured to a reasonable default value for each
         * platform.
         *
         * \return Returns a list of all the switches being passed to the underlying linker instance.
         */
        QList<QString> linkerSwitches();

        /**
         * Method you can use to change the list of switches being passed to the linker.  The list should exclude the
         * input file and output that are added independendy and managed through other mechanisms.
         *
         * Note that the switches are platform dependent and will be configured to a reasonable default value for each
         * platform.
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
         * Method you can use to obtain the current list of library search paths.
         *
         * \return Returns a list of the library search paths to be used by the linker.
         */
        QList<QString> librarySearchPaths() const;

        /**
         * Method you can use to change the current list of library search paths.
         *
         * \param[in] newLibrarySearchPaths the new list of library search paths.
         */
        void setLibrarySearchPaths(const QList<QString>& newLibrarySearchPaths);

        /**
         * Method you can use to obtain the current list of explicitly included static libraries.
         *
         * \return Returns a list of the static libraries that should be explicitly included into the build.
         */
        QList<QString> staticLibraries() const;

        /**
         * Method you can use to change the list of explicitly included static libraries.
         *
         * \param[in] newStaticLibraries the new list of static libraries.
         */
        void setStaticLibraries(const QList<QString>& newStaticLibraries);

        /**
         * Method you can use to obtain the current list of explicitly included dynamic libraries.
         *
         * \return Returns a list of the dynamic libraries that should be explicitly included into the build.
         */
        QList<QString> dynamicLibraries() const;

        /**
         * Method you can use to change the list of explicitly included dynamic libraries.
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
         * Method you can use to change the list of run-time search paths to be used by the linked object.  This method
         * provides capabilities similar to the --rpath switch on Linux and -rpath switch on MacOS.  The  call is
         * ignored on Windows.
         *
         * \param[in] newRunTimeSearchPaths The new list of run-time search paths.
         */
        void setRunTimeSearchPaths(const QList<QString>& newRunTimeSearchPaths);

        /**
         * Method that can be called to run the linker.
         *
         * \param[in] context The linker context to be executed.
         */
        void link(QSharedPointer<Cbe::LinkerContext> context);

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
         * Method that runs the linker back-end in the background.
         */
        void run() final;

    protected:
        /**
         * Pure virtual method that generates the default list of command line switches that would be issued to the
         * linker backend.  Overload this method in derived classes to customize the linker behaviour.
         *
         * \return Returns a default list of command line switches.  The command line switches should omit inputs,
         *         libraries, and the output file as these are added separately.
         */
        virtual QList<QString> setDefaultSwitches() const = 0;

        /**
         * Pure virtual method that is called to propagate the linker started notification to the interface class.
         *
         * \param[in] context The context that is being executed.
         */
        virtual void linkerStarted(QSharedPointer<Cbe::LinkerContext> context) = 0;

        /**
         * Pure virtual method that is called to propagate the linker finished notification to the interface class.
         *
         * \param[in] context The context that is being executed.
         *
         * \param[in] success Holds true if the linker completed successfully, returns false if an error is
         *                    reported.
         */
        virtual void linkerFinished(QSharedPointer<Cbe::LinkerContext> context, bool success) = 0;

        /**
         * Pure virtual method that is called to propagate diagnostic information to the interface class.
         *
         * \param[in] context           The context that is being executed.
         *
         * \param[in] diagnosticMessage The diagnostic message generated by the linker.
         */
        virtual void handleLinkerDiagnostic(
            QSharedPointer<Cbe::LinkerContext> context,
            const QString&                     diagnosticMessage
        ) = 0;

    private:
        /**
         * Method that adds a group of files or paths, with associated switches to a list of switches.
         *
         * \param[in] switchList List of switches to add new entries to.
         *
         * \param[in] parameters Additional parameters, each parameter will be coupled with a switch.
         *
         * \param[in] switchText The switch to prepend in front of each parameter.
         *
         * \param[in] extension  An optional extension added to the parameters.  If supplied, the extension should
         *                       include a leading period, if supplied.
         *
         * \param[in] prefix     An optional prefix added to the parameters.
         *
         * \return returns a reference to the switch list.
         */
        QList<QString> addOptions(
            QList<QString>&       switchList,
            const QList<QString>& parameters,
            const QString&        switchText,
            const QString&        extension = QString(),
            const QString&        prefix = QString()
        );

        /**
         * Method that sets up the external linker command line passed to the lld link method.
         *
         * \param[in] context The context that is being processed.
         */
        void buildUserFriendlyCommandLine(QSharedPointer<Cbe::LinkerContext> context);

        /**
         * Method that cleans up memory used by the user friendly linker switches.
         */
        void clearUserFriendlyCommandLine();

        /**
         * Shared pointer to the linker's job queue.
         */
        QSharedPointer<Cbe::JobQueue<Cbe::LinkerContext>> jobQueue;

        /**
         * Mutex used to protect the job queue during enqueue and dequeue operations.
         */
        QMutex jobQueueMutex;

        /**
         * Mutex used to keep linker invocations safe across threads.
         */
        QMutex linkerAccessMutex;

        /**
         * Flag used by the linker background thread to indicate that it's in the process of terminating.
         */
        volatile bool isTerminatingThread;

        /**
         * Flag indicating if debug output should be enabled.
         */
        bool currentDebugOutputEnabled;

        /**
         * The linker context that is actively being processed by the linker.  Used to assist with error reporting.
         */
        QSharedPointer<Cbe::LinkerContext> activeContext;

        /**
         * Flag that indicates if a set of default command line switches needs to be generated for the linker.
         */
        bool generateDefaultSwitches;

        /**
         * The current list of linker switches.  The list will exclude object files, search paths, libraries, or the
         * name of the output file as these will be managed separately.
         */
        QList<QString> currentSwitches;

        /**
         * The current system root directory.
         */
        QString currentSystemRoot;

        /**
         * The current list of library search paths.
         */
        QList<QString> currentLibrarySearchPaths;

        /**
         * The current list of static libraries to be linked against the objects.
         */
        QList<QString> currentStaticLibraries;

        /**
         * The current list of dynamic libraries to be linked against the objects.
         */
        QList<QString> currentDynamicLibraries;

        /**
         * The current list of system libraries.
         */
        QList<QString> currentSystemLibraries;

        /**
         * The input object files to be fed to the linker backend.
         */
        QList<QString> currentInputs;

        /**
         * The output file where the linked output will be written.
         */
        QString currentOutput;

        /**
         * Vector holding the user's command line switches.  The linker maintains a lot of string values by reference
         * forcing us to maintain persistent copies of the data.
         */
        QVector<char*> currentLinkerSwitches;

        /**
         * The notifier that receives information about the link operation.
         */
        Cbe::LinkerNotifier* currentNotifier;

        #if (defined(__APPLE__))

            /**
             * The current MacOS major version number.
             */
            unsigned currentMacOsMajorVersionNumber;

            /**
             * The current MacOS minor version number.
             */
            unsigned currentMacOsMinorVersionNumber;

        #endif

        #if (defined(__APPLE__) || defined(__linux__))

            /**
             * The list of run-time search paths.  This translates to --rpath switch entries on Linux, -rpath switch
             * entries on MacOS.  The value is not used on Windows.
             */
            QList<QString> currentRunTimeSearchPaths;

        #endif
};

#endif
