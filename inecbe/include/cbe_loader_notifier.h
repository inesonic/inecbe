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
* This header defines the \ref Cbe::LoaderNotifier class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_LOADER_NOTIFIER_H
#define CBE_LOADER_NOTIFIER_H

#include <QString>

#include "cbe_common.h"

namespace Cbe {
    class DynamicLibraryLoader;

    /**
     * Virtual base class that can be used to intercept notifications from a \ref Cbe::DynamicLibraryLoader class.  The
     * class provides a number of virtual methods you can overload to receive status notifications from the loader.
     */
    class CBE_PUBLIC_API LoaderNotifier {
        friend class DynamicLibraryLoader;

        public:
            LoaderNotifier();

            virtual ~LoaderNotifier();

            /**
             * Method you can use to obtain a pointer to the loader instance providing notifications to this class.
             *
             * \return Returns a pointer to the associated /ref Cbe::Loader class.
             */
            DynamicLibraryLoader* loader() const;

            /**
             * Method that is called each time a library is successfully loaded.  The default implementation simply
             * returns.
             *
             * \param[in] libraryFilename The name of the newly loaded library.
             */
            virtual void libraryLoaded(const QString& libraryFilename);

            /**
             * Method that is called whenever an attempt to load a library fails.  The default implementation simply
             * returns.
             *
             * \param[in] libraryFilename The name of the library that we failed to load.
             *
             * \param[in] errorReason     An error message indicating why the library could not be loaded.
             */
            virtual void libraryLoadFailed(const QString& libraryFilename, const QString& errorReason);

            /**
             * Method that is called when a library is about to be unloaded.  The default implementation simply returns.
             *
             * \param[in] libraryFilename The name of the library that is about to be unloaded.
             */
            virtual void aboutToUnload(const QString& libraryFilename);

            /**
             * Method that is called when a library is successfully unloaded.  The default implementation simply
             * returns.
             *
             * \param[in] libraryFilename The name of the library that was just unloaded.
             */
            virtual void libraryUnloaded(const QString& libraryFilename);

            /**
             * Method that is called when a library could not be unloaded.  The default implementation simply returns.
             *
             * \param[in] libraryFilename The name of the library that we could not unload.
             */
            virtual void libraryUnloadFailed(const QString& libraryFilename);

        private:
            DynamicLibraryLoader* currentLoader;
    };
};

#endif
