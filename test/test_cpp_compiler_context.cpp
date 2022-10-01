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
* This file implements basic functionality tests for the \ref Cbe:CompilerContext and \ref Cbe::CppCompilerContext
* classes.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QList>

#include <cbe_cpp_compiler_context.h>

#include "test_cpp_compiler_context.h"

TestCppCompilerContext::TestCppCompilerContext() {}


TestCppCompilerContext::~TestCppCompilerContext() {}


void TestCppCompilerContext::testConstructorsAndDestructors() {
    Cbe::CppCompilerContext context1("output.o");
    QVERIFY(context1.objectFile() == "output.o");
    QVERIFY(context1.sourceData().isEmpty());
    QVERIFY(context1.byteOffset() == 0);
    QVERIFY(context1.lineNumber() == 1);
    QVERIFY(context1.columnNumber() == 0);

    Cbe::CppCompilerContext context2("output.o", QList<QString>() << "foo.pch", QList<QString>() << "foo.h");

    QVERIFY(context2.objectFile() == "output.o");

    QVERIFY(context2.byteOffset() == 0);
    QVERIFY(context2.lineNumber() == 1);
    QVERIFY(context2.columnNumber() == 0);

    QString testContents = "Lets\nMake\nGreat\nThings";
    Cbe::CppCompilerContext context3(
        "output.o",
        testContents.toLocal8Bit(),
        QList<QString>() << "foo.pch",
        QList<QString>() << "foo.h"
    );

    QVERIFY(context1.objectFile() == "output.o");

    QVERIFY(context3.byteOffset() == static_cast<unsigned>(testContents.toLocal8Bit().size()));
    QVERIFY(context3.lineNumber() == 4);
    QVERIFY(context3.columnNumber() == 6);

    Cbe::CppCompilerContext context4(context3);

    QVERIFY(context4.byteOffset() == static_cast<unsigned>(testContents.toLocal8Bit().size()));
    QVERIFY(context4.lineNumber() == 4);
    QVERIFY(context4.columnNumber() == 6);
}


void TestCppCompilerContext::testObjectFileAccessors() {
    Cbe::CppCompilerContext context("output1.o");
    QVERIFY(context.objectFile() == "output1.o");

    context.setObjectFile("output2.o");
    QVERIFY(context.objectFile() == "output2.o");
}


void TestCppCompilerContext::testBufferOperators() {
    QString testContents = "Lets\nMake\nGreat\nThings";
    Cbe::CppCompilerContext context1("output.o", testContents.toLocal8Bit());

    QVERIFY(context1.lineNumber() == 4);
    QVERIFY(context1.columnNumber() == 6);

    context1 << '!';
    QVERIFY(context1.lineNumber() == 4);
    QVERIFY(context1.columnNumber() == 7);

    context1 << '\n';
    QVERIFY(context1.lineNumber() == 5);
    QVERIFY(context1.columnNumber() == 0);

    QString testString = QString("Because now we have a great tool.");
    context1 << testString;
    QVERIFY(context1.lineNumber() == 5);
    QVERIFY(context1.columnNumber() == static_cast<unsigned>(testString.toLocal8Bit().size()));

    context1 << QByteArray("\n...");
    QVERIFY(context1.lineNumber() == 6);
    QVERIFY(context1.columnNumber() == 3);

    Cbe::CppCompilerContext context2("output.o", QString("1234567890").toLocal8Bit());
    QVERIFY(context2.byteOffset() == 10);
}


void TestCppCompilerContext::testAssignmentOperators() {
    QString testContents = "Lets\nMake\nGreat\nThings";
    Cbe::CppCompilerContext context1(
        "output.o",
        testContents.toLocal8Bit(),
        QList<QString>() << "foo.pch",
        QList<QString>() << "foo.h"
    );

    QVERIFY(context1.byteOffset() == static_cast<unsigned>(testContents.toLocal8Bit().size()));
    QVERIFY(context1.lineNumber() == 4);
    QVERIFY(context1.columnNumber() == 6);

    Cbe::CppCompilerContext context2("output.o");
    QVERIFY(context2.byteOffset() == 0);
    QVERIFY(context2.lineNumber() == 1);
    QVERIFY(context2.columnNumber() == 0);

    context2 = context1;

    QVERIFY(context2.byteOffset() == static_cast<unsigned>(testContents.toLocal8Bit().size()));
    QVERIFY(context2.lineNumber() == 4);
    QVERIFY(context2.columnNumber() == 6);

    context1 << '\n';
    QString testString = QString("Because now we have a great tool.");
    context1 << testString;

    QVERIFY(context1.lineNumber() == 5);
    QVERIFY(context1.columnNumber() == static_cast<unsigned>(testString.toLocal8Bit().size()));


    QVERIFY(context2.byteOffset() == static_cast<unsigned>(testContents.toLocal8Bit().size()));
    QVERIFY(context2.lineNumber() == 4);
    QVERIFY(context2.columnNumber() == 6);
}
