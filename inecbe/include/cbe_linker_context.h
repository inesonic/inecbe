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
* This header defines the \ref Cbe::LinkerContext class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_LINKER_CONTEXT_H
#define CBE_LINKER_CONTEXT_H

#include <QString>
#include <QList>
#include <QSharedDataPointer>

#include <cstdint>

#include "cbe_common.h"

namespace Cbe {
    /**
     * Class that provides information about a linking operation.
     *
     * The class uses a pimpl implementation allowing you to pass the object, by value, with minimal overhead.
     */
    class CBE_PUBLIC_API LinkerContext {
        public:
            /**
             * Constructor
             *
             * \param[in] outputFile          The name of the linker output file.
             *
             * \param[in] newObjectFiles      A list of the object files to be linked.
             *
             * \param[in] newStaticLibraries  A list of the static libraries that should be linked against.
             *
             * \param[in] newDynamicLibraries A list of the dynamic libraries that should be linked against.
             */
            LinkerContext(
                const QString&        outputFile,
                const QList<QString>& newObjectFiles = QList<QString>(),
                const QList<QString>& newStaticLibraries = QList<QString>(),
                const QList<QString>& newDynamicLibraries = QList<QString>()
            );

            /**
             * Constructor
             *
             * \param[in] outputFile          The name of the linker output file.
             *
             * \param[in] objectFile          The name of the object file to be linked.
             *
             * \param[in] newStaticLibraries  A list of the static libraries that should be linked against.
             *
             * \param[in] newDynamicLibraries A list of the dynamic libraries that should be linked against.
             */
            LinkerContext(
                const QString&        outputFile,
                const QString&        objectFile,
                const QList<QString>& newStaticLibraries = QList<QString>(),
                const QList<QString>& newDynamicLibraries = QList<QString>()
            );

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            LinkerContext(const LinkerContext& other);

            virtual ~LinkerContext();

            /**
             * Method you can use to set the name of the linker output file.
             *
             * \param[in] newOutputFile The new name for the linker output file.
             */
            void setOutputFile(const QString& newOutputFile);

            /**
             * Method you can use to obtain the name of the linker output file.
             *
             * \return Returns the name of the linker output file.
             */
            QString outputFile() const;

            /**
             * Method you can use to set the list of object files.
             *
             * \param[in] newObjectFiles The list of new object files to be linked together.
             */
            void setObjectFiles(const QList<QString>& newObjectFiles);

            /**
             * Method you can use to set the list of object files to contain a single entry.
             *
             * \param[in] newObjectFile The object file to be linked.
             */
            void setObjectFile(const QString& newObjectFile);

            /**
             * Method you can use to obtain a list of the object files to be linked.
             *
             * \return Returns a list of the object files to be linked.
             */
            QList<QString> objectFiles() const;

            /**
             * Method you can use to set the list of static libraries to be linked.
             *
             * \param[in] newStaticLibraries A list of the static libraries to be linked against.
             */
            void setStaticLibraries(const QList<QString>& newStaticLibraries);

            /**
             * Method you can use to obtain a list of the static libraries to be linked against.
             *
             * \return Returns a list of the static libraries to be linked against.
             */
            QList<QString> staticLibraries() const;

            /**
             * Method you can use to set the list of dynamic libraries to be linked.
             *
             * \param[in] newDynamicLibraries A list of the dynamic libraries to be linked against.
             */
            void setDynamicLibraries(const QList<QString>& newDynamicLibraries);

            /**
             * Method you can use to obtain a list of the dynamic libraries to be linked against.
             *
             * \return Returns a list of the dynamic libraries to be linked against.
             */
            QList<QString> dynamicLibraries() const;

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            LinkerContext& operator=(const LinkerContext& other);

            /**
             * Virtual method you can overload to receive notification when the linker is started.  Note that the
             * method may be called from a different thread than the one used to invoke the linker.
             *
             * The default implementation simply returns.
             */
            virtual void linkerStarted();

            /**
             * Virtual method you can overload to receive notification when the linker has finished.  Note that the
             * method may be called from a different thread than the one used to invoke the linker.
             *
             * The default implementation simply returns.
             *
             * \param[in] success Holds true if the linker completed successfully, returns false if an error is
             *                    reported.
             */
            virtual void linkerFinished(bool success);

            /**
             * Virtual method you can overload to receive notification of diagnostic messages from the linker.  Note
             * that the method may be called from a different thread than the one used to invoke the linker.
             *
             * The default implementation simply returns without performing any additional function.
             *
             * \param[in] diagnosticMessage The diagnostic message generated by the linker.
             */
            virtual void handleLinkerDiagnostic(const QString& diagnosticMessage);

        private:
            class CBE_PUBLIC_API Private;

            QSharedDataPointer<Private> impl;
    };
};

#endif
