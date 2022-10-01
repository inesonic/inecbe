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
* This file implements tests of the Cbe::CppSourceRange class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QList>

#include <cbe_cpp_source_range.h>

#include "test_cpp_source_range.h"

TestCppSourceRange::TestCppSourceRange() {}


TestCppSourceRange::~TestCppSourceRange() {}


void TestCppSourceRange::testConstructorsDestructors() {
    Cbe::CppSourceRange sourceRange1;
    QVERIFY(sourceRange1.isInvalid());

    Cbe::CppSourceRange sourceRange2(0, 1, 2);
    QVERIFY(sourceRange2.isValid());
    QVERIFY(sourceRange2.filename().isEmpty());
    QVERIFY(sourceRange2.byteOffset() == 0);
    QVERIFY(sourceRange2.startLineNumber() == 1);
    QVERIFY(sourceRange2.endLineNumber() == 1);
    QVERIFY(sourceRange2.startColumnNumber() == 2);
    QVERIFY(sourceRange2.endColumnNumber() == 2);

    Cbe::CppSourceRange sourceRange3(0, 1, 2, 3, 4);
    QVERIFY(sourceRange3.isValid());
    QVERIFY(sourceRange3.filename().isEmpty());
    QVERIFY(sourceRange3.byteOffset() == 0);
    QVERIFY(sourceRange3.startLineNumber() == 1);
    QVERIFY(sourceRange3.startColumnNumber() == 2);
    QVERIFY(sourceRange3.endLineNumber() == 3);
    QVERIFY(sourceRange3.endColumnNumber() == 4);

    Cbe::CppSourceRange sourceRange4(QString("test_file.h"));
    QVERIFY(sourceRange4.isValid());
    QVERIFY(sourceRange4.filename() == QString("test_file.h"));
    QVERIFY(sourceRange4.startLineNumber() == 1);
    QVERIFY(sourceRange4.startColumnNumber() == 0);
    QVERIFY(sourceRange4.endLineNumber() == static_cast<unsigned>(-1));
    QVERIFY(sourceRange4.endColumnNumber() == static_cast<unsigned>(-1));

    Cbe::CppSourceRange sourceRange5(QString("test_file.h"), 0, 1, 2);
    QVERIFY(sourceRange5.isValid());
    QVERIFY(sourceRange5.filename() == QString("test_file.h"));
    QVERIFY(sourceRange5.byteOffset() == 0);
    QVERIFY(sourceRange5.startLineNumber() == 1);
    QVERIFY(sourceRange5.endLineNumber() == 1);
    QVERIFY(sourceRange5.startColumnNumber() == 2);
    QVERIFY(sourceRange5.endColumnNumber() == 2);

    Cbe::CppSourceRange sourceRange6(QString("test_file.h"), 0, 1, 2, 3, 4);
    QVERIFY(sourceRange6.isValid());
    QVERIFY(sourceRange6.filename() == QString("test_file.h"));
    QVERIFY(sourceRange6.byteOffset() == 0);
    QVERIFY(sourceRange6.startLineNumber() == 1);
    QVERIFY(sourceRange6.startColumnNumber() == 2);
    QVERIFY(sourceRange6.endLineNumber() == 3);
    QVERIFY(sourceRange6.endColumnNumber() == 4);

    Cbe::CppSourceRange sourceRange7 = sourceRange6;
    QVERIFY(sourceRange7.isValid());
    QVERIFY(sourceRange7.filename() == QString("test_file.h"));
    QVERIFY(sourceRange7.byteOffset() == 0);
    QVERIFY(sourceRange7.startLineNumber() == 1);
    QVERIFY(sourceRange7.startColumnNumber() == 2);
    QVERIFY(sourceRange7.endLineNumber() == 3);
    QVERIFY(sourceRange7.endColumnNumber() == 4);

    Cbe::CppSourceRange sourceRange8(sourceRange6);
    QVERIFY(sourceRange8.isValid());
    QVERIFY(sourceRange8.filename() == QString("test_file.h"));
    QVERIFY(sourceRange8.startLineNumber() == 1);
    QVERIFY(sourceRange8.startColumnNumber() == 2);
    QVERIFY(sourceRange8.endLineNumber() == 3);
    QVERIFY(sourceRange8.endColumnNumber() == 4);
}


void TestCppSourceRange::testValidInvalid() {
    Cbe::CppSourceRange sourceRange1;
    QVERIFY(sourceRange1.isInvalid());
    QVERIFY(!sourceRange1.isValid());

    Cbe::CppSourceRange sourceRange2(0, 1, 2);
    QVERIFY(!sourceRange2.isInvalid());
    QVERIFY(sourceRange2.isValid());

    Cbe::CppSourceRange sourceRange3(10, 3, 4, 1, 2); // Should be invalid, start is after end.
    QVERIFY(sourceRange3.isInvalid());
    QVERIFY(!sourceRange3.isValid());

    Cbe::CppSourceRange sourceRange4(10, 3, 4, 3, 2); // Should be invalid, start is after end but on the same line.
    QVERIFY(sourceRange4.isInvalid());
    QVERIFY(!sourceRange4.isValid());

    Cbe::CppSourceRange sourceRange5(10, 1, 2, 1, 3); // Should be valid.
    QVERIFY(!sourceRange5.isInvalid());
    QVERIFY(sourceRange5.isValid());
}


void TestCppSourceRange::testAccessors() {
    Cbe::CppSourceRange sourceRange1(0, 1, 2, 3, 4);
    QVERIFY(sourceRange1.isValid());
    QVERIFY(sourceRange1.filename().isEmpty());
    QVERIFY(sourceRange1.byteOffset() == 0);
    QVERIFY(sourceRange1.startLineNumber() == 1);
    QVERIFY(sourceRange1.startColumnNumber() == 2);
    QVERIFY(sourceRange1.endLineNumber() == 3);
    QVERIFY(sourceRange1.endColumnNumber() == 4);

    Cbe::CppSourceRange sourceRange2(QString("test_file2.h"), 10, 5, 6, 7, 8);
    QVERIFY(sourceRange2.isValid());
    QVERIFY(sourceRange2.filename() == QString("test_file2.h"));
    QVERIFY(sourceRange2.byteOffset() == 10);
    QVERIFY(sourceRange2.startLineNumber() == 5);
    QVERIFY(sourceRange2.startColumnNumber() == 6);
    QVERIFY(sourceRange2.endLineNumber() == 7);
    QVERIFY(sourceRange2.endColumnNumber() == 8);
}


void TestCppSourceRange::testAssignmentOperator() {
    Cbe::CppSourceRange sourceRange1;
    QVERIFY(sourceRange1.isInvalid());
    QVERIFY(sourceRange1.filename() != QString("test_file3.h"));
    QVERIFY(sourceRange1.startLineNumber() != 1);
    QVERIFY(sourceRange1.startColumnNumber() != 2);
    QVERIFY(sourceRange1.endLineNumber() != 3);
    QVERIFY(sourceRange1.endColumnNumber() != 4);

    Cbe::CppSourceRange sourceRange2(QString("test_file3.h"), 0, 1, 2, 3, 4);
    QVERIFY(sourceRange2.isValid());
    QVERIFY(sourceRange2.byteOffset() == 0);
    QVERIFY(sourceRange2.startLineNumber() == 1);
    QVERIFY(sourceRange2.startColumnNumber() == 2);
    QVERIFY(sourceRange2.endLineNumber() == 3);
    QVERIFY(sourceRange2.endColumnNumber() == 4);

    sourceRange1 = sourceRange2;
    QVERIFY(sourceRange1.isValid());
    QVERIFY(sourceRange1.byteOffset() == 0);
    QVERIFY(sourceRange1.startLineNumber() == 1);
    QVERIFY(sourceRange1.startColumnNumber() == 2);
    QVERIFY(sourceRange1.endLineNumber() == 3);
    QVERIFY(sourceRange1.endColumnNumber() == 4);
}


void TestCppSourceRange::testComparisonOperator() {
    Cbe::CppSourceRange sourceRange1;
    Cbe::CppSourceRange sourceRange2(0, 1, 2, 3, 4);
    Cbe::CppSourceRange sourceRange3(0, 5, 2, 3, 4);
    Cbe::CppSourceRange sourceRange4(0, 1, 5, 3, 4);
    Cbe::CppSourceRange sourceRange5(0, 1, 2, 5, 4);
    Cbe::CppSourceRange sourceRange6(0, 1, 2, 3, 5);
    Cbe::CppSourceRange sourceRange7(0, 1, 2, 3, 4);

    QVERIFY((sourceRange1 == sourceRange2) == false);
    QVERIFY((sourceRange1 != sourceRange2) == true);

    QVERIFY((sourceRange2 == sourceRange3) == false);
    QVERIFY((sourceRange2 != sourceRange3) == true);

    QVERIFY((sourceRange2 == sourceRange4) == false);
    QVERIFY((sourceRange2 != sourceRange4) == true);

    QVERIFY((sourceRange2 == sourceRange5) == false);
    QVERIFY((sourceRange2 != sourceRange5) == true);

    QVERIFY((sourceRange2 == sourceRange6) == false);
    QVERIFY((sourceRange2 != sourceRange6) == true);

    QVERIFY((sourceRange2 == sourceRange7) == true);
    QVERIFY((sourceRange2 != sourceRange7) == false);
}
