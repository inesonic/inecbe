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
* This header defines the \ref DiagnosticConsumer class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CPP_DIAGNOSTIC_CONSUMER_H
#define CPP_DIAGNOSTIC_CONSUMER_H

#include <QString>

#include <clang/Basic/Diagnostic.h>

class CompilerImpl;

/**
 * This class is a custom diagnostic consumer used to forward compiler errors to the application.  The class hides
 * behind the \ref CompilerImpl class and forwards error messages to that class.
 *
 * The class implicitely assumes that its scope is always less than the scope of the associated \ref CompilerImpl
 * instance.
 */
class DiagnosticConsumer:public clang::DiagnosticConsumer {
    public:
        /**
         * Constructor
         *
         * \param[in] newCompilerImplementation The associated compiler implementation to forward errors to.
         */
        DiagnosticConsumer(CompilerImpl* newCompilerImplementation = nullptr);

        ~DiagnosticConsumer() override;

        /**
         * Method you can call to set the compiler implementation to forward messages to.
         *
         * \param[in] newCompilerImplementation The compiler implementation to forward error data to.
         */
        void setCompilerImplementation(CompilerImpl* newCompilerImplementation);

        /**
         * Method you can use to obtain a pointer to the current compiler implementation.
         *
         * \return Returns a pointer to the compiler implementation we are forwarding messages to.
         */
        CompilerImpl* compilerImplementation() const;

        /**
         * Method you can call to obtain the number of reported errors.
         *
         * \return Returns the number of reported errors.
         */
        unsigned numberErrors() const;

        /**
         * Method you can call to obtain the the number of reported warnings.
         *
         * \return Returns the number of reported warnings.
         */
        unsigned numberWarnings() const;

        /**
         * Method you can call to clear any cached error status information.
         */
        void clear() final;

        /**
         * Method called by the compiler to report errors, warnings, etc.
         *
         * \param[in] diagnosticLevel       The diagnostic level.
         *
         * \param[in] diagnosticInformation The information associated with the diagnostic.
         */
        void HandleDiagnostic(
            clang::DiagnosticsEngine::Level diagnosticLevel,
            const clang::Diagnostic&        diagnosticInformation
        ) final;

    private:
        CompilerImpl* currentCompiler;
};

#endif
