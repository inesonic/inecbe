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
* This header defines the \ref Cbe::CompilerContext class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_COMPILER_CONTEXT_H
#define CBE_COMPILER_CONTEXT_H

#include <QString>
#include <QList>
#include <QByteArray>
#include <QSharedDataPointer>

#include <cstdint>

#include "cbe_common.h"

namespace Cbe {
    /**
     * Class that provides information about a build of a single source implementation.
     *
     * The class uses a pimpl implementation allowing you to pass the object, by value, with minimal overhead.
     */
    class CBE_PUBLIC_API CompilerContext {
        public:
            /**
             * Constructor
             *
             * \param[in] newObjectFile The name of the object file to be generated.
             *
             * \param[in] newPchFiles   A list of PCH files to include into the build.
             */
            CompilerContext(const QString& newObjectFile, const QList<QString>& newPchFiles = QList<QString>());

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            CompilerContext(const CompilerContext& other);

            virtual ~CompilerContext();

            /**
             * Method you can use to specify the name of the object file the compiler should generate.
             *
             * \param[in] newObjectFile The object file to be linked.
             */
            void setObjectFile(const QString& newObjectFile);

            /**
             * Method you can use to obtain the name of the object file the compiler should generate.
             *
             * \return Returns a list of the object files to be linked.
             */
            QString objectFile() const;

            /**
             * Method you can use to obtain access to the raw data contained in this class.
             *
             * \return Returns constant reference to a byte array holding the raw data.
             */
            virtual const QByteArray& sourceData() const = 0;

            /**
             * Method you can use to determine the current byte offset into the source data.
             *
             * \return Returns the byte offset into the source data.
             */
            virtual unsigned long byteOffset() const = 0;

            /**
             * Method that can be called from derived classes to append source code data to the internal buffer.
             *
             * \param[in] data A single byte of data to be appended to the data buffer.
             */
            virtual void append(const std::uint8_t data) = 0;

            /**
             * Method that can be called from derived classes to append source code data to the internal buffer.
             *
             * \param[in] data Pointer to an array of bytes holding the data to be appended.
             */
            virtual void append(const QByteArray& data) = 0;

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            CompilerContext& operator=(const CompilerContext& other);

            /**
             * Virtual method you can overload to receive notification when the compiler is started.  Note that the
             * method may be called from a different thread than the one used to invoke the compiler.
             *
             * The default implementation simply returns.
             */
            virtual void compilerStarted();

            /**
             * Virtual method you can overload to receive notification when the compiler has finished.  Note that the
             * method may be called from a different thread than the one used to invoke the compiler.
             *
             * The default implementation simply returns.
             *
             * \param[in] success Holds true if the compiler completed successfully, returns false if an error is
             *                    reported.
             */
            virtual void compilerFinished(bool success);

        private:
            class CBE_PUBLIC_API Private;

            QSharedDataPointer<Private> impl;
    };
};

#endif
