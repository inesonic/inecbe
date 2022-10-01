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
* This file implements basic functionality tests for the \ref Cbe:LinkerContext class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QList>

#include <cbe_linker_context.h>

#include "test_linker_context.h"

TestLinkerContext::TestLinkerContext() {}


TestLinkerContext::~TestLinkerContext() {}


void TestLinkerContext::testConstructorsAndDestructors() {
    Cbe::LinkerContext context1(
        "output.so",
        QList<QString>() << "object1.o" << "object2.o",
        QList<QString>() << "static1.a" << "static2.a",
        QList<QString>() << "dynamic1.dylib" << "dynamic2.dylib"
    );

    QVERIFY(context1.outputFile() == "output.so");

    QVERIFY(context1.objectFiles().size() == 2);
    QVERIFY(context1.objectFiles().at(0) == "object1.o");
    QVERIFY(context1.objectFiles().at(1) == "object2.o");

    QVERIFY(context1.staticLibraries().size() == 2);
    QVERIFY(context1.staticLibraries().at(0) == "static1.a");
    QVERIFY(context1.staticLibraries().at(1) == "static2.a");

    QVERIFY(context1.dynamicLibraries().size() == 2);
    QVERIFY(context1.dynamicLibraries().at(0) == "dynamic1.dylib");
    QVERIFY(context1.dynamicLibraries().at(1) == "dynamic2.dylib");

    Cbe::LinkerContext context2(
        "output.so",
        "object.o",
        QList<QString>() << "static1.a" << "static2.a",
        QList<QString>() << "dynamic1.dylib" << "dynamic2.dylib"
    );

    QVERIFY(context2.outputFile() == "output.so");

    QVERIFY(context2.objectFiles().size() == 1);
    QVERIFY(context2.objectFiles().at(0) == "object.o");

    QVERIFY(context2.staticLibraries().size() == 2);
    QVERIFY(context2.staticLibraries().at(0) == "static1.a");
    QVERIFY(context2.staticLibraries().at(1) == "static2.a");

    QVERIFY(context2.dynamicLibraries().size() == 2);
    QVERIFY(context2.dynamicLibraries().at(0) == "dynamic1.dylib");
    QVERIFY(context2.dynamicLibraries().at(1) == "dynamic2.dylib");

    Cbe::LinkerContext context3(context2);

    QVERIFY(context3.outputFile() == "output.so");

    QVERIFY(context3.objectFiles().size() == 1);
    QVERIFY(context3.objectFiles().at(0) == "object.o");

    QVERIFY(context3.staticLibraries().size() == 2);
    QVERIFY(context3.staticLibraries().at(0) == "static1.a");
    QVERIFY(context3.staticLibraries().at(1) == "static2.a");

    QVERIFY(context3.dynamicLibraries().size() == 2);
    QVERIFY(context3.dynamicLibraries().at(0) == "dynamic1.dylib");
    QVERIFY(context3.dynamicLibraries().at(1) == "dynamic2.dylib");
}


void TestLinkerContext::testOutputFileAccessors() {
    Cbe::LinkerContext context("output.so");

    QVERIFY(context.outputFile() == "output.so");

    context.setOutputFile("output2.so");
    QVERIFY(context.outputFile() == "output2.so");
}


void TestLinkerContext::testObjectFileAccessors() {
    Cbe::LinkerContext context("output.so");

    QVERIFY(context.objectFiles().isEmpty());

    context.setObjectFile("object.o");
    QVERIFY(context.objectFiles().size() == 1);
    QVERIFY(context.objectFiles().at(0) == "object.o");

    context.setObjectFiles(QList<QString>() << "object1.o" << "object2.o");
    QVERIFY(context.objectFiles().size() == 2);
    QVERIFY(context.objectFiles().at(0) == "object1.o");
    QVERIFY(context.objectFiles().at(1) == "object2.o");
}


void TestLinkerContext::testStaticLibraryAccessors() {
    Cbe::LinkerContext context("output.so");

    QVERIFY(context.staticLibraries().isEmpty());

    context.setStaticLibraries(QList<QString>() << "static1.a" << "static2.a");
    QVERIFY(context.staticLibraries().size() == 2);
    QVERIFY(context.staticLibraries().at(0) == "static1.a");
    QVERIFY(context.staticLibraries().at(1) == "static2.a");
}


void TestLinkerContext::testDynamicLibraryAccessors() {
    Cbe::LinkerContext context("output.so");

    QVERIFY(context.dynamicLibraries().isEmpty());

    context.setDynamicLibraries(QList<QString>() << "dynamic1.so" << "dynamic2.so");
    QVERIFY(context.dynamicLibraries().size() == 2);
    QVERIFY(context.dynamicLibraries().at(0) == "dynamic1.so");
    QVERIFY(context.dynamicLibraries().at(1) == "dynamic2.so");
}


void TestLinkerContext::testAssignmentOperators() {
    Cbe::LinkerContext context1(
        "output.exe",
        "object.obj",
        QList<QString>() << "static.lib",
        QList<QString>() << "dynamic.dll"
    );

    QVERIFY(context1.outputFile() == "output.exe");

    QVERIFY(context1.objectFiles().size() == 1);
    QVERIFY(context1.objectFiles().at(0) == "object.obj");

    QVERIFY(context1.staticLibraries().size() == 1);
    QVERIFY(context1.staticLibraries().at(0) == "static.lib");

    QVERIFY(context1.dynamicLibraries().size() == 1);
    QVERIFY(context1.dynamicLibraries().at(0) == "dynamic.dll");

    Cbe::LinkerContext context2("output2.exe");

    QVERIFY(context2.outputFile() == "output2.exe");

    QVERIFY(context2.objectFiles().isEmpty());
    QVERIFY(context2.staticLibraries().isEmpty());
    QVERIFY(context2.staticLibraries().isEmpty());

    context2 = context1;

    QVERIFY(context2.outputFile() == "output.exe");

    QVERIFY(context2.objectFiles().size() == 1);
    QVERIFY(context2.objectFiles().at(0) == "object.obj");

    QVERIFY(context2.staticLibraries().size() == 1);
    QVERIFY(context2.staticLibraries().at(0) == "static.lib");

    QVERIFY(context2.dynamicLibraries().size() == 1);
    QVERIFY(context2.dynamicLibraries().at(0) == "dynamic.dll");

    context1.setOutputFile("output3.exe");
    context1.setObjectFile("foo.obj");
    context1.setStaticLibraries(QList<QString>() << "static2.lib");
    context1.setDynamicLibraries(QList<QString>() << "dynamic2.dll");

    QVERIFY(context2.outputFile() == "output.exe");

    QVERIFY(context2.objectFiles().size() == 1);
    QVERIFY(context2.objectFiles().at(0) == "object.obj");

    QVERIFY(context2.staticLibraries().size() == 1);
    QVERIFY(context2.staticLibraries().at(0) == "static.lib");

    QVERIFY(context2.dynamicLibraries().size() == 1);
    QVERIFY(context2.dynamicLibraries().at(0) == "dynamic.dll");
}
