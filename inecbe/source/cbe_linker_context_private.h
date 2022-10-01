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
* This header defines the Cbe::LinkerContext::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_LINKER_CONTEXT_PRIVATE_H
#define CBE_LINKER_CONTEXT_PRIVATE_H

#include <QString>
#include <QList>
#include <QByteArray>
#include <QSharedData>

#include "cbe_common.h"
#include "cbe_linker_context.h"

namespace Cbe {
    /**
     * Private implementation of the \ref Cbe::LinkerContext class.
     */
    class CBE_PUBLIC_API LinkerContext::Private:public QSharedData {
        public:
            /**
             * Constructor
             *
             * \param[in] outputFile          The name of the linker output file.
             *
             * \param[in] newObjectFiles         A list of the object files to be linked.
             *
             * \param[in] newStaticLibraries  A list of the static libraries that should be linked against.
             *
             * \param[in] newDynamicLibraries A list of the dynamic libraries that should be linked against.
             */
            Private(
                const QString&        outputFile,
                const QList<QString>& newObjectFiles,
                const QList<QString>& newStaticLibraries,
                const QList<QString>& newDynamicLibraries
            );

            /**
             * Constructor
             *
             * \param[in] outputFile          The name of the linker output file.
             *
             * \param[in] newObjectFile       The name of the object file to be linked.
             *
             * \param[in] newStaticLibraries  A list of the static libraries that should be linked against.
             *
             * \param[in] newDynamicLibraries A list of the dynamic libraries that should be linked against.
             */
            Private(
                const QString&        outputFile,
                const QString&        newObjectFile,
                const QList<QString>& newStaticLibraries,
                const QList<QString>& newDynamicLibraries
            );

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            Private(const LinkerContext::Private& other);

            ~Private();

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
             * Method you can use to obtain a list of the object files to be linked.
             *
             * \return Returns a list of the object files to be linked.
             */
            const QList<QString>& objectFiles() const;

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
            const QList<QString>& staticLibraries() const;

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
            const QList<QString>& dynamicLibraries() const;

        private:
            /**
             * The name of the generated output file.
             */
            QString currentOutputFile;

            /**
             * The list of object files to be linked.
             */
            QList<QString> currentObjectFiles;

            /**
             * The list of static libraries to be linked against.
             */
            QList<QString> currentStaticLibraries;

            /**
             * The list of dynamic libraries to be linked against.
             */
            QList<QString> currentDynamicLibraries;
    };
};

#endif
