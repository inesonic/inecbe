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
* This file implements the \ref Cbe::DynamicLibraryLoader class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>

#include "cbe_loader_notifier.h"
#include "cbe_dynamic_library_loader_private.h"
#include "cbe_dynamic_library_loader.h"

namespace Cbe {
    DynamicLibraryLoader::DynamicLibraryLoader(
            LoaderNotifier* newNotifier
        ):impl(new DynamicLibraryLoader::Private(
            newNotifier
        )) {
        if (newNotifier != nullptr) {
            newNotifier->currentLoader = this;
        }
    }


    DynamicLibraryLoader::~DynamicLibraryLoader() {
        if (impl->notifier() != nullptr) {
            impl->notifier()->currentLoader = nullptr;
        }
    }


    void DynamicLibraryLoader::setNotifier(LoaderNotifier* newNotifier) {
        if (impl->notifier() != nullptr) {
            impl->notifier()->currentLoader = nullptr;
        }

        impl->setNotifier(newNotifier);

        if (newNotifier != nullptr) {
            newNotifier->currentLoader = this;
        }
    }


    LoaderNotifier* DynamicLibraryLoader::notifier() const {
        return impl->notifier();
    }


    bool DynamicLibraryLoader::load(const QString& libraryFilename) {
        return impl->load(libraryFilename);
    }


    bool DynamicLibraryLoader::unload() {
        return impl->unload();
    }


    QString DynamicLibraryLoader::filename() const {
        return impl->filename();
    }


    bool DynamicLibraryLoader::isLoaded() const {
        return impl->isLoaded();
    }


    bool DynamicLibraryLoader::isNotLoaded() const {
        return !isLoaded();
    }


    void* DynamicLibraryLoader::resolve(const QString& symbolName) const {
        return impl->resolve(symbolName);
    }
}
