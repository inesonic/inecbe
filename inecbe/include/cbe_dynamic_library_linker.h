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
* This header defines the \ref Cbe::DynamicLibraryLinker class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_DYNAMIC_LIBRARY_LINKER_H
#define CBE_DYNAMIC_LIBRARY_LINKER_H

#include <QString>
#include <QList>

#include "cbe_linker.h"

namespace Cbe {
    class LinkerNotifier;

    /**
     * Class that provides a thin wrapper on the \ref Cbe::Linker class.  You can use this class to link dynamic
     * libraries from your application.
     */
    class CBE_PUBLIC_API DynamicLibraryLinker:public Linker {
        public:
            /**
             * Constructor
             *
             * \param[in] newNotifier A pointer to the notifier instance this linker should send notification data to.
             *                        A null pointer will disable notifications.
             */
            DynamicLibraryLinker(LinkerNotifier* newNotifier = nullptr);

            ~DynamicLibraryLinker() override;

        protected:
            /**
             * Method that generates the default list of command line switches that would be issued to the linker
             * backend.
             *
             * \return Returns a default list of command line switches.  The command line switches should omit inputs,
             *         libraries, and the output file as these are added separately.
             */
            QList<QString> setDefaultSwitches() const final;

        private:
            /**
             * Method that generates lists of library directories and paths from a list of library files.
             *
             * \param[out] libraryDirectories A list to be populated with the library directries.
             *
             * \param[out] libraryFiles       A list of unique base filenames.
             *
             * \param[in]  libraryPaths       The library paths to be processed.
             *
             * \param[in]  removePrefix       If true, any "lib" prefix should be removed.
             *
             * \param[in]  removeExtensions   If true, file extensions will be removed.
             */
            static void processLibraryPaths(
                QList<QString>&       libraryDirectories,
                QList<QString>&       libraryFiles,
                const QList<QString>& libraryPaths,
                bool                  removePrefix,
                bool                  removeExtensions
            );
    };
};

#endif
