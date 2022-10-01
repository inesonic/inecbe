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
* This header defines the Cbe::SourceRange::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_SOURCE_RANGE_PRIVATE_H
#define CBE_SOURCE_RANGE_PRIVATE_H

#include <QString>

#include "cbe_common.h"
#include "cbe_source_range.h"

namespace Cbe {
    /**
     * Private implementation of the \ref Cbe::SourceRange class.
     */
    class CBE_PUBLIC_API SourceRange::Private {
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
             * \param[in] filename   The name of the file of interest.
             *
             * \param[in] byteOffset The byte offset into the file where the error exists.
             */
            Private(const QString& filename, unsigned byteOffset);

            ~Private();

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

        private:
            /**
             * The associated filename.
             */
            QString currentFilename;

            /**
             * The byte offset into the buffer.
             */
            unsigned currentByteOffset;
    };
};

#endif
