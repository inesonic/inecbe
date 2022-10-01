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
* This file implements the \ref Cbe::DynamicLibraryLinker class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QSet>
#include <QFileInfo>

#include "cbe_linker_notifier.h"
#include "cbe_linker.h"
#include "cbe_dynamic_library_linker.h"

namespace Cbe {
    DynamicLibraryLinker::DynamicLibraryLinker(LinkerNotifier* newNotifier):Linker(newNotifier) {}


    DynamicLibraryLinker::~DynamicLibraryLinker() {}


    QList<QString> DynamicLibraryLinker::setDefaultSwitches() const {
        QList<QString> switches;

        #if (defined(_WIN32) || defined(_WIN64))

            // For bad or worse, we don't use the /defaultlib:libcpmt switch.  Instead we include that library in the
            // list of standard libraries and link it in explicitly.

            switches << "/errorlimit:0"
                     << "/dynamicbase"
                     << "/nxcompat"
                     << "/nodefaultlib"
                     << "/dll"
                     << "/manifest:embed";

            if (debugOutputEnabled()) {
                switches << "/verbose";
            }

        #elif (defined(__APPLE__))

            QList<QString> systemLibraries = Linker::systemLibraries();
            switches << "-dynamiclib";

            QList<QString> libraryFiles;
            QList<QString> libraryDirectories;

            processLibraryPaths(libraryDirectories, libraryFiles, systemLibraries, true, true);

            for (  QList<QString>::const_iterator directoryIterator    = libraryDirectories.constBegin(),
                                                  directoryEndIterator = libraryDirectories.constEnd()
                 ; directoryIterator != directoryEndIterator
                 ; ++directoryIterator
                ) {
                switches << QString("-L%1").arg(*directoryIterator);
            }

            for (  QList<QString>::const_iterator fileIterator    = libraryFiles.constBegin(),
                                                  fileEndIterator = libraryFiles.constEnd()
                 ; fileIterator != fileEndIterator
                 ; ++fileIterator
                ) {
                switches << QString("-l%1").arg(*fileIterator);
            }

        #elif (defined(__linux__))

            QList<QString> systemLibraries = Linker::systemLibraries();
            switches << "--hash-style=gnu"
                     << "--eh-frame-hdr"
//                     << "-m" << "elf_x86_64"
                     << "--dynamic-linker";

            for (  QList<QString>::const_iterator systemLibraryIterator    = systemLibraries.constBegin(),
                                                  systemLibraryEndIterator = systemLibraries.constEnd()
                 ; systemLibraryIterator != systemLibraryEndIterator
                 ; ++systemLibraryIterator
                ) {
                switches << *systemLibraryIterator;
            }

            switches << "--shared";

        #else

            #error Unknown platform

        #endif

        return switches;
    }


    void DynamicLibraryLinker::processLibraryPaths(
            QList<QString>&       libraryDirectories,
            QList<QString>&       libraryFiles,
            const QList<QString>& libraryPaths,
            bool                  removePrefix,
            bool                  removeExtensions
        ) {
        QSet<QString> ld;
        QSet<QString> lf;

        for (  QList<QString>::const_iterator pathsIterator    = libraryPaths.constBegin(),
                                              pathsEndIterator = libraryPaths.constEnd()
             ; pathsIterator != pathsEndIterator
             ; ++pathsIterator
            ) {
            QFileInfo path(*pathsIterator);
            if (path.isFile()) {
                QString directory = path.canonicalPath();
                QString filename  = removeExtensions ? path.completeBaseName() : path.fileName();

                if (removePrefix && filename.startsWith(QString("lib"))) {
                    filename = filename.mid(3);
                }

                ld.insert(directory);
                lf.insert(filename);
            }
        }

        libraryDirectories = ld.values();
        libraryFiles       = lf.values();
    }
}
