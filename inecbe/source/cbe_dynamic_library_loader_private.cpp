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
* This file implements the \ref Cbe::DynamicLibraryLoader::Private class.
***********************************************************************************************************************/

#include <QString>
#include <QLibrary>

#include <QDebug> // debug

#include "cbe_loader_notifier.h"
#include "cbe_dynamic_library_loader.h"
#include "cbe_dynamic_library_loader_private.h"

namespace Cbe {
    DynamicLibraryLoader::Private::Private(LoaderNotifier* newNotifier) {
        currentNotifier = newNotifier;
    }


    DynamicLibraryLoader::Private::~Private() {}


    void DynamicLibraryLoader::Private::setNotifier(LoaderNotifier* newNotifier) {
        currentNotifier = newNotifier;
    }


    LoaderNotifier* DynamicLibraryLoader::Private::notifier() const {
        return currentNotifier;
    }


    bool DynamicLibraryLoader::Private::load(const QString& libraryFilename) {
        bool success = true;

        if (currentLibrary.isLoaded()) {
            success = unload();
        }

        if (success) {
            currentLibrary.setFileName(libraryFilename);
            success = currentLibrary.load();
        }

        if (currentNotifier != nullptr) {
            if (success) {
                currentNotifier->libraryLoaded(libraryFilename);
            } else {
                QString errorReason = currentLibrary.errorString();
                currentNotifier->libraryLoadFailed(libraryFilename, errorReason);
            }
        } else {
            if (!success) {
                QString errorReason = currentLibrary.errorString();
                qDebug() << errorReason;
            }
        }

        return success;
    }


    bool DynamicLibraryLoader::Private::unload() {
        bool success = true;

        if (currentLibrary.isLoaded()) {
            QString libraryName = currentLibrary.fileName();

            if (currentNotifier != nullptr) {
                currentNotifier->aboutToUnload(libraryName);
            }

            success = currentLibrary.unload();

            if (currentNotifier != nullptr) {
                if (success) {
                    currentNotifier->libraryUnloaded(libraryName);
                } else {
                    currentNotifier->libraryUnloadFailed(libraryName);
                }
            }
        }

        return success;
    }


    QString DynamicLibraryLoader::Private::filename() const {
        QString result;

        if (currentLibrary.isLoaded()) {
            result = currentLibrary.fileName();
        }

        return result;
    }


    bool DynamicLibraryLoader::Private::isLoaded() const {
        return currentLibrary.isLoaded();
    }


    void* DynamicLibraryLoader::Private::resolve(const QString& symbolName) {
        void* result = nullptr;

        if (currentLibrary.isLoaded()) {
            result = reinterpret_cast<void*>(currentLibrary.resolve(symbolName.toLocal8Bit().data()));
        }

        return result;
    }
}
