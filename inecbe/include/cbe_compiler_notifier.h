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
* This header defines the \ref Cbe::CompilerNotifier class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_COMPILER_NOTIFIER_H
#define CBE_COMPILER_NOTIFIER_H

#include <QString>

#include "cbe_common.h"
#include "cbe_compiler.h"

namespace Cbe {
    /**
     * Virtual base class that can be used to intercept notifications from a \ref Cbe::Compiler class.  The class
     * provides a number of virtual methods you can overload to receive status notifications from the compiler.
     */
    class CBE_PUBLIC_API CompilerNotifier {
        friend class Compiler;

        public:
            CompilerNotifier();

            virtual ~CompilerNotifier();

            /**
             * Method you can use to obtain a pointer to the compiler instance providing notifications to this class.
             *
             * \return Returns a pointer to the associated /ref Cbe::Compiler class.
             */
            Compiler* compiler() const;

            /**
             * Virtual method that is called when a fatal error is detected.  Note that the method may be called from a
             * different thread than the once used to invoke the compiler.
             *
             * The default implementation simply returns.
             *
             * \param[in] reason                   A string indicating the reason for the fatal error.
             *
             * \param[in] generateCrashDiagnostics If true, the compiler backend suggested generating crash diagnostics.
             */
            virtual void fatalCompilerError(const QString& reason, bool generateCrashDiagnostics);

        private:
            Compiler* currentCompiler;
    };
};

#endif
