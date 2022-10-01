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
* This header defines the \ref Cbe::SourceRange class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_SOURCE_RANGE_H
#define CBE_SOURCE_RANGE_H

#include <QString>
#include <QSharedPointer>

#include "cbe_common.h"

namespace Cbe {
    /**
     * Pure virtual base class that represents a range within the compiler source data.
     */
    class CBE_PUBLIC_API SourceRange {
        public:
            /**
             * Value used to indicate an invalid byte offset.
             */
            static constexpr unsigned invalidByteOffset = static_cast<unsigned>(-1);

            /**
             * Constructor.
             *
             * Creates a default, invalid, source range.
             */
            SourceRange();

            /**
             * Constructor.
             *
             * \param[in] filename   The name of the file of interest.
             *
             * \param[in] byteOffset The byte offset into the file where the error exists.
             */
            SourceRange(const QString& filename, unsigned byteOffset = invalidByteOffset);

            /**
             * Copy constructor.
             *
             * \param[in] other the instance to be copied.
             */
            SourceRange(const SourceRange& other);

            virtual ~SourceRange();

            /**
             * Method you can use to determine if the source range is valid.
             *
             * \return Returns true if the source range is valid.  Returns false if the source range is invalid.
             */
            virtual bool isValid() const = 0;

            /**
             * Method you can use to determine if the source range is invalid.
             *
             * \return Returns true if the source range is invalid.  Returns false if the source range is valid.
             */
            bool isInvalid() const;

            /**
             * Method you can use to determine the filename associated with the source range.  An empty filename will
             * be returned if the filename was not set when the class was created.
             *
             * \return Returns the filename associated with the source range.
             */
            QString filename() const;

            /**
             * Method you can use to obtain the byte offset into the source range.
             *
             * \return Returns the byte offset into the source range.
             */
            unsigned byteOffset() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this object.
             */
            SourceRange& operator=(const SourceRange& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare to this instance.
             *
             * \return Returns true if the instances point to the same range, returns false if the instances point to
             *         different ranges.
             */
            bool operator==(const SourceRange& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare to this instance.
             *
             * \return Returns true if the instances point to different ranges, returns false if the instances point to
             *         the same range.
             */
            bool operator!=(const SourceRange& other) const;

        private:
            class CBE_PUBLIC_API Private;

            QSharedPointer<Private> impl;
    };
};

#endif
