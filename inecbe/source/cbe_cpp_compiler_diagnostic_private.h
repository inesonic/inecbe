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
* This header defines the Cbe::CppCompilerDiagnostic::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_CPP_COMPILER_DIAGNOSTIC_PRIVATE_H
#define CBE_CPP_COMPILER_DIAGNOSTIC_PRIVATE_H

#include <QString>
#include <QSharedPointer>

#include "cbe_common.h"
#include "cbe_cpp_compiler_context.h"
#include "cbe_cpp_source_range.h"
#include "cbe_cpp_compiler_diagnostic.h"

namespace Cbe {
    /**
     * Base class for compiler diagnostic data.
     */
    class CBE_PUBLIC_API CppCompilerDiagnostic::Private {
        public:
            /**
             * Constructor.
             *
             * \param[in] context     The compiler context tied to this diagnostic.
             *
             * \param[in] sourceRange The range in the source associated with the diagnostic.
             */
            Private(QSharedPointer<CppCompilerContext> context, const CppSourceRange& sourceRange);

            ~Private();

            /**
             * Method that return the context used to trigger this build.
             *
             * \return Returns a shared pointer to the compiler context.
             */
            QSharedPointer<CppCompilerContext> context() const;

            /**
             * Method that returns the source code range associated with the diagnostic.
             *
             * \return Returns the source range associated with the diagnostic.
             */
            const CppSourceRange& sourceRange() const;

        private:
            /**
             * The context tied to this diagnostic.
             */
            QSharedPointer<CppCompilerContext> currentContext;

            /**
             * The source range tied to this diagnostic.
             */
            CppSourceRange currentSourceRange;
    };
};

#endif
