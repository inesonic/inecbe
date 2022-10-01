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
* This file implements tests of the \ref Cbe::CppCompilerDiagnostic and \ref Cbe::CompilerDiagnostic class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QSharedPointer>

#include <cbe_cpp_compiler_context.h>
#include <cbe_cpp_source_range.h>
#include <cbe_cpp_compiler_diagnostic.h>

#include "test_cpp_compiler_diagnostic.h"

TestCppCompilerDiagnostic::TestCppCompilerDiagnostic() {}


TestCppCompilerDiagnostic::~TestCppCompilerDiagnostic() {}


void TestCppCompilerDiagnostic::testConstructorsDestructors() {
    QSharedPointer<Cbe::CppCompilerContext> context(new Cbe::CppCompilerContext("foo.o"));
    Cbe::CppSourceRange     sourceRange("foo.cpp", 0, 1, 1, 2, 2);
    Cbe::CppCompilerDiagnostic diagnostic1(
        context,
        Cbe::CppCompilerDiagnostic::Level::WARNING,
        1, // code
        "foo is not allowed",
        sourceRange
    );

    QVERIFY(diagnostic1.level() == Cbe::CppCompilerDiagnostic::Level::WARNING);
    QVERIFY(diagnostic1.code() == 1);
    QVERIFY(diagnostic1.message() == "foo is not allowed");
    QVERIFY(diagnostic1.context()->objectFile() == "foo.o");
    QVERIFY(diagnostic1.sourceRange().filename() == "foo.cpp");

    Cbe::CppCompilerDiagnostic diagnostic2(diagnostic1);

    QVERIFY(diagnostic2.level() == Cbe::CppCompilerDiagnostic::Level::WARNING);
    QVERIFY(diagnostic2.code() == 1);
    QVERIFY(diagnostic2.message() == "foo is not allowed");
    QVERIFY(diagnostic2.context()->objectFile() == "foo.o");
    QVERIFY(diagnostic2.sourceRange().filename() == "foo.cpp");
}


void TestCppCompilerDiagnostic::testAccessors() {
    QSharedPointer<Cbe::CppCompilerContext> context1(new Cbe::CppCompilerContext("foo.o"));
    Cbe::CppSourceRange     sourceRange1("foo.cpp", 0, 1, 1, 2, 2);
    Cbe::CppCompilerDiagnostic diagnostic1(
        context1,
        Cbe::CppCompilerDiagnostic::Level::WARNING,
        1, // code
        "foo is not allowed",
        sourceRange1
    );

    QVERIFY(diagnostic1.level() == Cbe::CppCompilerDiagnostic::Level::WARNING);
    QVERIFY(diagnostic1.code() == 1);
    QVERIFY(diagnostic1.message() == "foo is not allowed");
    QVERIFY(diagnostic1.context()->objectFile() == "foo.o");
    QVERIFY(diagnostic1.sourceRange().filename() == "foo.cpp");

    QSharedPointer<Cbe::CppCompilerContext> context2(new Cbe::CppCompilerContext("bar.o"));
    Cbe::CppSourceRange     sourceRange2("bar.cpp", 0, 1, 1, 2, 2);
    Cbe::CppCompilerDiagnostic diagnostic2(
        context2,
        Cbe::CppCompilerDiagnostic::Level::ERROR,
        2, // code
        "bar is also not allowed",
        sourceRange2
    );

    QVERIFY(diagnostic2.level() == Cbe::CppCompilerDiagnostic::Level::ERROR);
    QVERIFY(diagnostic2.code() == 2);
    QVERIFY(diagnostic2.message() == "bar is also not allowed");
    QVERIFY(diagnostic2.context()->objectFile() == "bar.o");
    QVERIFY(diagnostic2.sourceRange().filename() == "bar.cpp");
}


void TestCppCompilerDiagnostic::testAssignmentOperator() {
    QSharedPointer<Cbe::CppCompilerContext> context1(new Cbe::CppCompilerContext("foo.o"));
    Cbe::CppSourceRange     sourceRange1("foo.cpp", 0, 1, 1, 2, 2);
    Cbe::CppCompilerDiagnostic diagnostic1(
        context1,
        Cbe::CppCompilerDiagnostic::Level::WARNING,
        1, // code
        "foo is not allowed",
        sourceRange1
    );

    QVERIFY(diagnostic1.level() == Cbe::CppCompilerDiagnostic::Level::WARNING);
    QVERIFY(diagnostic1.code() == 1);
    QVERIFY(diagnostic1.message() == "foo is not allowed");
    QVERIFY(diagnostic1.context()->objectFile() == "foo.o");
    QVERIFY(diagnostic1.sourceRange().filename() == "foo.cpp");

    QSharedPointer<Cbe::CppCompilerContext> context2(new Cbe::CppCompilerContext("bar.o"));
    Cbe::CppSourceRange     sourceRange2("bar.cpp", 0, 1, 1, 2, 2);
    Cbe::CppCompilerDiagnostic diagnostic2(
        context2,
        Cbe::CppCompilerDiagnostic::Level::ERROR,
        2, // code
        "bar is also not allowed",
        sourceRange2
    );

    QVERIFY(diagnostic2.level() == Cbe::CppCompilerDiagnostic::Level::ERROR);
    QVERIFY(diagnostic2.code() == 2);
    QVERIFY(diagnostic2.message() == "bar is also not allowed");
    QVERIFY(diagnostic2.context()->objectFile() == "bar.o");
    QVERIFY(diagnostic2.sourceRange().filename() == "bar.cpp");

    diagnostic2 = diagnostic1;

    QVERIFY(diagnostic2.level() == Cbe::CppCompilerDiagnostic::Level::WARNING);
    QVERIFY(diagnostic2.code() == 1);
    QVERIFY(diagnostic2.message() == "foo is not allowed");
    QVERIFY(diagnostic2.context()->objectFile() == "foo.o");
    QVERIFY(diagnostic2.sourceRange().filename() == "foo.cpp");
}
