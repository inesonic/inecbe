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
* This header defines the \ref Cbe::CppSourceRange class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_CPP_SOURCE_RANGE_H
#define CBE_CPP_SOURCE_RANGE_H

#include <QString>
#include <QSharedPointer>

#include "cbe_common.h"
#include "cbe_source_range.h"

namespace Cbe {
    /**
     * Trivial class that represents a range of text within a source file.
     */
    class CBE_PUBLIC_API CppSourceRange:public SourceRange {
        public:
            /**
             * Constructor.
             *
             * Creates a default, invalid, source range.
             */
            CppSourceRange();

            /**
             * Constructor.
             *
             * \param[in] filename   The name of the file of interest.
             */
            CppSourceRange(const QString& filename);

            /**
             * Constructor.
             *
             * \param[in] byteOffset  The byte offset into the file where the error exists.
             *
             * \param[in] startLine   The starting, one based, line number in the source file.
             *
             * \param[in] startColumn The starting, zero based, column number in the source file.
             *
             * \param[in] endLine     The ending, one based, line number in the source file.
             *
             * \param[in] endColumn   The ending, zero based, column number in the source file.
             */
            CppSourceRange(
                unsigned byteOffset,
                unsigned startLine,
                unsigned startColumn,
                unsigned endLine,
                unsigned endColumn
            );

            /**
             * Constructor.
             *
             * \param[in] filename    The name of the file of interest.
             *
             * \param[in] byteOffset  The byte offset into the file where the error exists.
             *
             * \param[in] startLine   The starting, one based, line number in the source file.
             *
             * \param[in] startColumn The starting, zero based, column number in the source file.
             *
             * \param[in] endLine     The ending, one based, line number in the source file.
             *
             * \param[in] endColumn   The ending, zero based, column number in the source file.
             */
            CppSourceRange(
                const QString& filename,
                unsigned       byteOffset,
                unsigned       startLine,
                unsigned       startColumn,
                unsigned       endLine,
                unsigned       endColumn
            );

            /**
             * Constructor.
             *
             * \param[in] byteOffset The byte offset into the file where the error exists.
             *
             * \param[in] line       The one based, line number in the source file.
             *
             * \param[in] column     The zero based, column number in the source file.
             */
            CppSourceRange(unsigned byteOffset, unsigned line, unsigned column);

            /**
             * Constructor.
             *
             * \param[in] filename   The name of the file of interest.
             *
             * \param[in] byteOffset The byte offset into the file where the error exists.
             *
             * \param[in] line       The one based, line number in the source file.
             *
             * \param[in] column     The zero based, column number in the source file.
             */
            CppSourceRange(const QString& filename, unsigned byteOffset, unsigned line, unsigned column);

            /**
             * Copy constructor.
             *
             * \param[in] other the instance to be copied.
             */
            CppSourceRange(const CppSourceRange& other);

            ~CppSourceRange();

            /**
             * Method you can use to determine if the source range is valid.
             *
             * \return Returns true if the source range is valid.  Returns false if the source range is invalid.
             */
            bool isValid() const final;

            /**
             * Method you can use to determine if the source range is invalid.
             *
             * \return Returns true if the source range is invalid.  Returns false if the source range is valid.
             */
            bool isInvalid() const;

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

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this object.
             */
            CppSourceRange& operator=(const CppSourceRange& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare to this instance.
             *
             * \return Returns true if the instances point to the same range, returns false if the instances point to
             *         different ranges.
             */
            virtual bool operator==(const CppSourceRange& other) const final;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare to this instance.
             *
             * \return Returns true if the instances point to different ranges, returns false if the instances point to
             *         the same range.
             */
            bool operator!=(const CppSourceRange& other) const;

        private:
            class CBE_PUBLIC_API Private;

            QSharedPointer<Private> impl;
    };
};

#endif
