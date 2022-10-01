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
* This file is the main entry point for the inecbe unit tests.
***********************************************************************************************************************/

#include <QtTest/QtTest>

#include "test_cpp_source_range.h"
#include "test_cpp_compiler_context.h"
#include "test_linker_context.h"
#include "test_cpp_compiler_diagnostic.h"
#include "test_compiler_basic_functionality.h"
#include "test_linker_basic_functionality.h"
#include "test_dynamic_library_loader_basic_functionality.h"

#define TEST(_X) {                                                  \
    _X _x;                                                          \
    testStatus |= QTest::qExec(&_x, argumentCount, argumentValues); \
}

int main(int argumentCount, char** argumentValues) {
    int testStatus = 0;

    TEST(TestCppSourceRange);
    TEST(TestCppCompilerContext);
    TEST(TestLinkerContext);
    TEST(TestCppCompilerDiagnostic);
    TEST(TestCompilerBasicFunctionality);
    TEST(TestLinkerBasicFunctionality);
    TEST(TestDynamicLibraryLoaderBasicFunctionality);

    return testStatus;
}
