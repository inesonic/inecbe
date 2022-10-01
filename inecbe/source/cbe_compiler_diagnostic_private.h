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
* This header defines the Cbe::CompilerDiagnostic::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_COMPILER_DIAGNOSTIC_PRIVATE_H
#define CBE_COMPILER_DIAGNOSTIC_PRIVATE_H

#include <QString>
#include <QSharedPointer>

#include "cbe_common.h"
#include "cbe_compiler_diagnostic.h"

namespace Cbe {
    /**
     * Class that provides the implementation of the \ref Cbe::CompilerDiagnostic class.
     */
    class CBE_PUBLIC_API CompilerDiagnostic::Private {
        public:
            /**
             * Constructor.
             *
             * \param[in] diagnosticLevel   A value from the enumeration \ref CompilerDiagnostic::Level indicating the
             *                              type of diagnostic being reported.
             *
             * \param[in] diagnosticCode    The compiler specific code associated with the diagnostic.
             *
             * \param[in] diagnosticMessage A text based error message associated with the diagnostic.
             */
            Private(
                CompilerDiagnostic::Level diagnosticLevel,
                CompilerDiagnostic::Code  diagnosticCode,
                const QString&            diagnosticMessage
            );

            ~Private();

            /**
             * Method you can use to obtain the diagnostic level associated with this diagnostic.
             *
             * \return Returns the diagnostic level associated with this diagnostic.
             */
            CompilerDiagnostic::Level level() const;

            /**
             * Method you can use to obtain the diagnostic code.
             *
             * \return Returns the diagnostic code reported by the compiler.  Note that the diagnostic code is compiler
             *         specific.
             */
            CompilerDiagnostic::Code code() const;

            /**
             * Method you can use to obtain the message associated with the diagnostic.
             *
             * \return Returns the message associated with the diagnostic.
             */
            QString message() const;

        private:
            /**
             * The compiler diagnostic level.
             */
            CompilerDiagnostic::Level currentLevel;

            /**
             * Compiler diagnostic code.
             */
            CompilerDiagnostic::Code currentCode;

            /**
             * The diagnostic message.
             */
            QString currentMessage;
    };
};

#endif
