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
* This header defines the \ref Cbe::CppCompiler class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_CPP_COMPILER_H
#define CBE_CPP_COMPILER_H

#include <QString>
#include <QList>

#include "cbe_common.h"
#include "cbe_compiler.h"

namespace Cbe {
    class CppCompilerNotifier;

    /**
     * Wrapper around the \ref Cbe::Compiler class that customizes the base compiler to build C++ source, generating an
     * object file suitable for linking.
     */
    class CBE_PUBLIC_API CppCompiler:public Compiler {
        public:
            /**
             * Constructor
             *
             * \param[in] newNotifier A pointer to the notifier instance this compiler should send notification data to.
             *                        A null pointer will disable notifications.
             */
            CppCompiler(CppCompilerNotifier* newNotifier = nullptr);

            ~CppCompiler() override;

            /**
             * Method you can use to change the notifier receiving notifications from this compiler.  This method will
             * block until all pending contexts have been processed by the compiler.
             *
             * \param[in] newNotifier A pointer to the notifier instance this compiler should send notification data to.
             *                        A null pointer will disable notifications.
             */
            void setNotifier(CppCompilerNotifier* newNotifier);

            /**
             * Method you can use to determine the current notifier receiving notifications from this compiler.
             *
             * \return Returns a pointer to the notifier.  A null pointer is returned if there is no notifier receiving
             *         notifications from the compiler.
             */
            CppCompilerNotifier* notifier() const;

        protected:
            /**
             * Method that generates the default list of command line switches that would be issued to the compiler.
             *
             * \return Returns a default list of command line switches.
             */
            QList<QString> setDefaultSwitches() const final;

            /**
             * Method that is called when the compiler is started.  You should overload this method in derived classes
             * to provide notification to the notifier class and the context.
             *
             * Note that this method may be called from a different thread than was used to invoke the compiler.
             *
             * \param[in] context The context that is being executed.
             */
            void compilerStarted(QSharedPointer<CompilerContext> context) override;

            /**
             * Method that is called when the compiler has completed processing a context.  You should overload this
             * method in derived classes to provide notification to the notifier class and the context.
             *
             * Note that this method may be called from a different thread than was used to invoke the compiler.
             *
             * \param[in] context    The context that is being executed.
             *
             * \param[in] successful Holds true of the compilation completed with no reported errors.  Holds false if an
             *                       error was detected.
             */
            void compilerFinished(QSharedPointer<CompilerContext> context, bool successful) override;

            /**
             * Method that reports errors and other diagnostics information, ideally to a \ref Cbe::CompilerNotifier
             * class.  Classes should overload this method to provide proper reporting or errors, warnings, and similar
             * notifications.
             *
             * \param[in] notifier          A pointer to the notifier that should receive the notification.  Note that
             *                              this method will be called even if the supplied notifier pointer is null.
             *
             * \param[in] context           A shared pointer to the context the compiler is actively processing.  All
             *                              errors and location information will apply to this context.
             *
             * \param[in] diagnosticLevel   A value from the enumeration \ref CompilerDiagnostic::Level indicating the
             *                              type of diagnostic being reported.
             *
             * \param[in] diagnosticId      The compiler specific ID associated with the diagnostic.
             *
             * \param[in] diagnosticMessage A text based error message associated with the diagnostic.
             *
             * \param[in] filename          The filename where the error was found.  An empty string indicates that the
             *                              error was found in the data stored in the context.
             *
             * \param[in] byteOffset        A zero based offset into the source data where the error was detected.
             *
             * \param[in] lineNumber        A one based line number into the source data where the error was detected.
             *                              This value is only meaningful if the buffer contains textual information.
             *
             * \param[in] columnNumber      A zero based column number into the source data where the error was detected.
             *                              This value is only meaningful if the buffer contains textual information.
             */
            void processDiagnostic(
                CompilerNotifier*               notifier,
                QSharedPointer<CompilerContext> context,
                Cbe::CompilerDiagnostic::Level  diagnosticLevel,
                unsigned                        diagnosticId,
                const QString&                  diagnosticMessage,
                const QString&                  filename,
                unsigned                        byteOffset,
                unsigned                        lineNumber,
                unsigned                        columnNumber
            ) override;
    };
};

#endif
