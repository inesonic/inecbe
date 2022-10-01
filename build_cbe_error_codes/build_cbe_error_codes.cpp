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
* This file provides a trivial executable that generates a header mapping CLANG and LLVM diagnostic codes to
* enumerations.  The program allows use to decouple the LLVM/CLANG headers from the rest of the build.
***********************************************************************************************************************/

#include <QString>
#include <QMap>
#include <QFile>
#include <QTextStream>

#include <iostream>

#include "warnings.h"
SUPPRESS_LLVM_WARNINGS
#include <clang/Basic/AllDiagnostics.h>
RESTORE_LLVM_WARNINGS

/**
 * Class that holds diagnostic data.
 */
class DiagnosticData {
    private:
        /**
         * The diagnostic ID.
         */
        unsigned diagnosticId;

        /**
         * The diagnostic enumerated value.
         */
        QString diagnosticEnumName;

        /**
         * The diagnostic description.
         */
        QString diagnosticDescription;

    public:
        DiagnosticData():diagnosticId(0) {}

        /**
         * Constructor
         *
         * \param[in] id          The diagnostic ID.
         *
         * \param[in] enumName    The numeration used by CLANG to flag this diagnostic.
         *
         * \param[in] description The description of the diagnostic.
         */
        DiagnosticData(
                unsigned       id,
                const QString& enumName,
                const QString& description
            ):diagnosticId(
                id
            ),diagnosticEnumName(
                enumName
            ),diagnosticDescription(
                description
            ) {}

        /**
         * Copy constructor
         *
         * \param[in] other The instance to assign to this instance.
         */
        DiagnosticData(
                const DiagnosticData& other
            ):diagnosticId(
                other.diagnosticId
            ),diagnosticEnumName(
                other.diagnosticEnumName
            ),diagnosticDescription(
                other.diagnosticDescription
            ) {}

        /**
         * Method you can use to obtain the diagnostic ID.
         *
         * \return Returns the diagnostic ID.
         */
        inline unsigned id() const {
            return diagnosticId;
        }

        /**
         * Method you can use to obtain the diagnostic enum name.
         *
         * \return Returns the diagnostic enum name.
         */
        const QString& enumName() const {
            return diagnosticEnumName;
        }

        /**
         * Method you can use to obtain a cleaned up enumeration name.
         *
         * \return Returns a cleaned up enumeration name.
         */
        QString cleanedEnumName() const {
            QString result = diagnosticEnumName.toUpper();

            replace(result, "ERR_", "ERROR_");
            replace(result, "WARN_", "WARNING_");

            return result;
        }

        /**
         * Method you can use to obtain the diagnostic description.
         *
         * \return Returns the diagnostic description.
         */
        const QString& description() const {
            return diagnosticDescription;
        }

        /**
         * Method you can use to assign a value to this instance.
         *
         * \param[in] other The instance to assign to this instance.
         *
         * \return Returns a reference to this instance.
         */
        DiagnosticData& operator=(const DiagnosticData& other) {
            diagnosticId          = other.diagnosticId;
            diagnosticEnumName    = other.diagnosticEnumName;
            diagnosticDescription = other.diagnosticDescription;

            return *this;
        }

    private:
        /**
         * Method that replace a string prefix.
         *
         * \param[in,out] s The string to be changed.
         *
         * \param[in]     o The old prefix to be replaced.
         *
         * \param[in]     n The new prefix to be replaced.
         */
        static void replace(QString& s, const QString& o, const QString& n) {
            if (s.startsWith(o)) {
                s = n + s.mid(o.length());
            }
        }

};

/**
 * Funciton that returns a mapping of diagnostic data instances by clang diagnostic ID.
 *
 * \return Returns a mapping of diagnostic data by clang diagnostic ID.
 */
static QMap<unsigned, DiagnosticData> generateDiagnosticData() {
    /*
     * clang defines diagnostics using a list of #defines placed in include files.  These defines are then mapped to
     * distinct enums with overlapping IDs.
     *
     * Internally, the compiler linearizes all thse distinct mappings into a single diagnostic ID although the mapping
     * is kept opaque making mapping the IDs returned with a diagnostic difficult to map to a specific enumerated value.
     *
     * To work around this, we reconstruct some of what clang does internally using the IDs defined in DiagnosticIDs.h
     * and the various include files.
     */

    unsigned diagnosticId;
    QMap<unsigned, DiagnosticData> diagnostics;
    #define DIAG(                                                                                                 \
            ENUM,                                                                                                 \
            FLAGS,                                                                                                \
            DEFAULT_MAPPING,                                                                                      \
            DESC,                                                                                                 \
            GROUP,                                                                                                \
            SFINAE,                                                                                               \
            NOWERROR,                                                                                             \
            SHOWINSYSHEADER,                                                                                      \
            SHOWINSYSMACRO,                                                                                       \
            DEFERRABLE,                                                                                           \
            CATEGORY                                                                                              \
        ) {                                                                                                       \
            DiagnosticData diagnostic(diagnosticId, QString::fromLocal8Bit(#ENUM), QString::fromLocal8Bit(DESC)); \
            assert(diagnostics.find(diagnosticId) == diagnostics.end());                                          \
            diagnostics.insert(diagnosticId, diagnostic);                                                         \
            ++diagnosticId;                                                                                       \
        }

    diagnosticId = static_cast<unsigned>(clang::diag::DIAG_START_AST);
    #include <clang/Basic/DiagnosticASTKinds.inc>

    diagnosticId = static_cast<unsigned>(clang::diag::DIAG_START_ANALYSIS);
    #include <clang/Basic/DiagnosticAnalysisKinds.inc>

    diagnosticId = static_cast<unsigned>(clang::diag::DIAG_START_COMMENT);
    #include <clang/Basic/DiagnosticCommentKinds.inc>

    diagnosticId = static_cast<unsigned>(clang::diag::DIAG_START_CROSSTU);
    #include <clang/Basic/DiagnosticCrossTUKinds.inc>

    diagnosticId = static_cast<unsigned>(clang::diag::DIAG_START_DRIVER);
    #include <clang/Basic/DiagnosticDriverKinds.inc>

    diagnosticId = static_cast<unsigned>(clang::diag::DIAG_START_FRONTEND);
    #include <clang/Basic/DiagnosticFrontendKinds.inc>

    diagnosticId = static_cast<unsigned>(clang::diag::DIAG_START_LEX);
    #include <clang/Basic/DiagnosticLexKinds.inc>

    diagnosticId = static_cast<unsigned>(clang::diag::DIAG_START_PARSE);
    #include <clang/Basic/DiagnosticParseKinds.inc>

    diagnosticId = static_cast<unsigned>(clang::diag::DIAG_START_SEMA);
    #include <clang/Basic/DiagnosticSemaKinds.inc>

    diagnosticId = static_cast<unsigned>(clang::diag::DIAG_START_SERIALIZATION);
    #include <clang/Basic/DiagnosticSerializationKinds.inc>

    diagnosticId = static_cast<unsigned>(clang::diag::DIAG_START_REFACTORING);
    #include <clang/Basic/DiagnosticRefactoringKinds.inc>

    return diagnostics;
}


int main(int argumentCount, char* argumentValues[]) {
    int exitStatus = 0;
    if (argumentCount != 2) {
        std::cerr << "*** Invalid number of arguments." << std::endl;
        exitStatus = 1;
    } else {
        QString filename = QString::fromLocal8Bit(argumentValues[1]);
        QFile outputFile(filename);

        if (!outputFile.open(QFile::OpenModeFlag::WriteOnly | QFile::OpenModeFlag::Text)) {
            std::cerr << "*** Unable to open file " << filename.toLocal8Bit().data() << std::endl;
            exitStatus = 1;
        } else {
            {
                QTextStream os(&outputFile);
                QByteArray  eol("\n");

                static QMap<unsigned, DiagnosticData> diagnostics = generateDiagnosticData();
                os << "/*-*-c++-*-***********************************************************************************"
                   << "**************************" << eol
                   << "* Copyright 2020 - 2022 Inesonic, LLC." << eol
                   << "* All rights reserved." << eol
                   << "**********************************************************************************************"
                   << "**********************//**" << eol
                   << "* \\file" << eol
                   << eol
                   << "* This file defines an enumeration mapping CLANG error codes to readable values.  The file "
                   << "avoids the needs to include" << eol
                   << "* a large number of CLANG/LLVM headers in external source." << eol
                   << "**********************************************************************************************"
                   << "*************************/" << eol
                   << eol
                   << "#ifndef CBE_COMPILER_ERROR_CODES_H" << eol
                   << "#define CBE_COMPILER_ERROR_CODES_H" << eol
                   << eol
                   << "namespace Cbe {" << eol
                   << "    enum class CppCompilerErrorCodes:unsigned {" << eol
                   << "        NO_ERROR = 0," << eol;

                unsigned lastId = diagnostics.last().id();
                for (  QMap<unsigned, DiagnosticData>::const_iterator it  = diagnostics.constBegin(),
                                                                      end = diagnostics.constEnd()
                     ; it != end
                     ; ++it
                    ) {
                    unsigned              id             = it.key();
                    const DiagnosticData& diagnosticData = it.value();

            //        os << "        // " << diagnosticData.description() << eol;
                    os << "        " << diagnosticData.cleanedEnumName() << " = " << id;

                    if (id != lastId) {
                        os << ","; // << eol;
                    }

                    os << eol;
                }

                os << "    };" << eol
                   << "}" << eol
                   << eol
                   << "#endif" << eol;
            }

            outputFile.close();
        }

        return exitStatus;
    }
}
