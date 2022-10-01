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
* This header defines the \ref Cbe::DynamicLibraryLoader::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_DYNAMIC_LIBRARY_LOADER_PRIVATE_H
#define CBE_DYNAMIC_LIBRARY_LOADER_PRIVATE_H

#include <QString>
#include <QLibrary>

#include "cbe_common.h"
#include "cbe_loader_notifier.h"
#include "cbe_dynamic_library_loader.h"

namespace Cbe {
    /**
     * Class that provides the implementation of the \ref Cbe::DynamicLibraryLoader class.
     */
    class CBE_PUBLIC_API DynamicLibraryLoader::Private {
        public:
            /**
             * Constructor
             *
             * \param[in] newNotifier A pointer to the notifier instance this loader should send notification data to.
             *                        A null pointer will disable notifications.
             */
            Private(LoaderNotifier* newNotifier);

            virtual ~Private();

            /**
             * Method you can use to change the notifier receiving notifications from this loader.
             *
             * \param[in] newNotifier A pointer to the notifier instance this loader should send notification data to.
             *                        A null pointer will disable notifications.
             */
            void setNotifier(LoaderNotifier* newNotifier);

            /**
             * Method you can use to determine the current notifier receiving notifications from this loader.
             *
             * \return Returns a pointer to the notifier.  A null pointer is returned if there is no notifier receiving
             *         notifications from the loader.
             */
            LoaderNotifier* notifier() const;

            /**
             * Method you can call to explicitly load a library.  You can only load one instance of the library at a
             * time.  Calling this method while a library is loaded will cause the code to attempt to unload the library
             * before loading the newly specified library.
             *
             * \param[in] libraryFilename The name of the dynamic library to be loaded.  Note that a relative file path
             *                            will cause the loader to search for the dynamic library using the system
             *                            paths.  For this reason you are strongly advised to specify an absolute path
             *                            to the library.
             *
             * \return Returns true on success, returns false on failure.
             */
            bool load(const QString& libraryFilename);

            /**
             * Method you can call to explicitly unload a library.
             *
             * \return Returns true on success, returns false on error.
             */
            bool unload();

            /**
             * Method you can use to determine the filename of the currently loaded library.
             *
             * \return Returns the filename of the currently loaded library.  Returns an empty string if no library is
             *         loaded.
             */
            QString filename() const;

            /**
             * Method you can call to determine if a library is loaded.
             *
             * \return Returns true if the library is loaded.  Returns false if the library is not loaded.
             */
            bool isLoaded() const;

            /**
             * Method you can use to resove a symbol, by name.  The name must match the name of the symbol as
             * represented in the library.  To avoid having to deal with platform dependent name manging, you are
             * strongly encouraged to "extern "C"" any function or globals that you intend to reference through this
             * resolution method.
             *
             * \param[in] symbolName The name of the symbol to be resolved.
             *
             * \return Returns a void pointer to the desired symbol.  You should cast this symbol to the appropriate
             *         type.
             */
            void* resolve(const QString& symbolName);

        private:
            /**
             * The underlying QLibrary instance that we marshall.
             */
            QLibrary currentLibrary;

            /**
             * The notifier to receive notification of loading events.
             */
            LoaderNotifier* currentNotifier;
    };
};

#endif
