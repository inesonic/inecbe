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
* This header defines the Cbe::CppSourceRange::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_SOURCE_RANGE_PRIVATE_H
#define CBE_SOURCE_RANGE_PRIVATE_H

#include <QString>

#include "cbe_common.h"
#include "cbe_cpp_source_range.h"

namespace Cbe {
    /**
     * Private implementation of the \ref Cbe::CppSourceRange class.
     */
    class CBE_PUBLIC_API CppSourceRange::Private {
        public:
            /**
             * Constructor.
             *
             * Creates a default, invalid, source range.
             */
            Private();

            /**
             * Constructor.
             *
             * \param[in] startLine   The starting, one based, line number in the source file.
             *
             * \param[in] startColumn The starting, zero based, column number in the source file.
             *
             * \param[in] endLine     The ending, one based, line number in the source file.
             *
             * \param[in] endColumn   The ending, zero based, column number in the source file.
             */
            Private(unsigned startLine, unsigned startColumn, unsigned endLine, unsigned endColumn);

            /**
             * Constructor.
             *
             * \param[in] line   The one based, line number in the source file.
             *
             * \param[in] column The zero based, column number in the source file.
             */
            Private(unsigned line, unsigned column);

            ~Private();

            /**
             * Method you can use to determine if the source range is valid.
             *
             * \return Returns true if the source range is valid.  Returns false if the source range is invalid.
             */
            bool isValid() const;

            /**
             * Method you can use to determine the starting line number in the source range.  Line numbers are one
             * based.
             *
             * \return Returns the starting line number in the source range.
             */
            unsigned startLineNumber() const;

            /**
             * Method you can use to detemrine the starting column number in the source range.  Column numbers are zero
             * based.
             *
             * \return Returns the starting column number in the source range.
             */
            unsigned startColumnNumber() const;

            /**
             * Method you can use to determine the ending line number in the source range.  Line numbers are one based.
             *
             * \return Returns the ending line number in the source range.
             */
            unsigned endLineNumber() const;

            /**
             * Method you can use to detemrine the ending column number in the source range.  Column numbers are zero
             * based.
             *
             * \return Returns the ending column number in the source range.
             */
            unsigned endColumnNumber() const;

        private:
            /**
             * Starting line number.
             */
            unsigned currentStartLine;

            /**
             * Starting column number.
             */
            unsigned currentStartColumn;

            /**
             * Ending line number.
             */
            unsigned currentEndLine;

            /**
             * Ending column number.
             */
            unsigned currentEndColumn;
    };
};

#endif
