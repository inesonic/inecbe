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
* This header defines the Cbe::CppCompilerContext::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_CPP_COMPILER_CONTEXT_PRIVATE_H
#define CBE_CPP_COMPILER_CONTEXT_PRIVATE_H

#include <QString>
#include <QList>
#include <QByteArray>
#include <QSharedData>

#include "cbe_common.h"
#include "cbe_compiler_context.h"
#include "cbe_cpp_compiler_context.h"

namespace Cbe {
    /**
     * Class that provides the implementation of the \ref CppCompilerContext class.
     *
     * The class uses a pimpl implementation allowing you to pass the object, by value, with minimal overhead.
     */
    class CBE_PUBLIC_API CppCompilerContext::Private:public QSharedData {
        public:
            /**
             * Constructor
             *
             * \param[in] newHeaderFiles A list of C++ header files to include into the build.
             */
            Private(const QList<QString>& newHeaderFiles);

            /**
             * Constructor
             *
             * \param[in] newSourceData  The source data used for the build.
             *
             * \param[in] newHeaderFiles A list of header files to include into the build.
             */
            Private(const QByteArray& newSourceData,const QList<QString>& newHeaderFiles);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            Private(const CppCompilerContext::Private& other);

            ~Private();

            /**
             * Method you can use to obtain access to the raw data contained in this class.
             *
             * \return Returns constant reference to a byte array holding the raw data.
             */
            const QByteArray& sourceData() const;

            /**
             * Method you can use to determine the current byte offset into the source data.
             *
             * \return Returns the byte offset into the source data.
             */
            unsigned long byteOffset() const;

            /**
             * Method you can use to determine the current line number in the included source code.  New data will be
             * appended starting at this position.
             *
             * \return Returns the current one-based line number.
             */
            unsigned long lineNumber() const;

            /**
             * Method you can use to determine the current column number in the included source code.  New data will be
             * appended starting at this position.
             *
             * \return Returns the current zero-based column number.
             */
            unsigned long columnNumber() const;

            /**
             * Method that can be called from derived classes to append source code data to the internal buffer.
             *
             * \param[in] data A single byte of data to be appended to the data buffer.
             */
            void append(const std::uint8_t data);

            /**
             * Method that can be called from derived classes to append source code data to the internal buffer.
             *
             * \param[in] data Pointer to an array of bytes holding the data to be appended.
             */
            void append(const QByteArray& data);

        private:
            /**
             * The current running line number.
             */
            unsigned long currentLineNumber;

            /**
             * The current running column number.
             */
            unsigned long currentColumnNumber;

            /**
             * The current list of C++ headers.
             */
            QList<QString> currentHeaderFiles;

            /**
             * The raw data buffer.
             */
            QByteArray rawData;
    };
};

#endif
