##-*-makefile-*-########################################################################################################
# Copyright 2016 - 2022 Inesonic, LLC
# 
# This file is licensed under two licenses.
#
# Inesonic Commercial License, Version 1:
#   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
#   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
#   strictly prohibited.
#
# GNU Public License, Version 2:
#   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
#   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
#   version.
#   
#   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
#   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
#   details.
#   
#   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
#   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
########################################################################################################################

########################################################################################################################
# Basic build characteristics
#

TEMPLATE = app
QT += core testlib
CONFIG += testcase c++14

HEADERS = process_memory.h \
          test_cpp_source_range.h \
          test_cpp_compiler_context.h \
          test_linker_context.h \
          test_cpp_compiler_diagnostic.h \
          test_compiler_basic_functionality.h \
          test_linker_basic_functionality.h \
          test_dynamic_library_loader_basic_functionality.h

SOURCES = process_memory.cpp \
          test_inecbe.cpp \
          test_cpp_source_range.cpp \
          test_cpp_compiler_context.cpp \
          test_linker_context.cpp \
          test_cpp_compiler_diagnostic.cpp \
          test_compiler_basic_functionality.cpp \
          test_linker_basic_functionality.cpp \
          test_dynamic_library_loader_basic_functionality.cpp

win32 {
    DEFINES += PSAPI_VERSION=1
}

########################################################################################################################
# inecbe library:
#

CBE_BASE = $${OUT_PWD}/../inecbe/
INCLUDEPATH = $${PWD}/../inecbe/include/

unix {
    CONFIG(debug, debug|release) {
        LIBS += -L$${CBE_BASE}/build/debug/ -linecbe
        PRE_TARGETDEPS += $${CBE_BASE}/build/debug/libinecbe.so
    } else {
        LIBS += -L$${CBE_BASE}/build/release/ -linecbe
        PRE_TARGETDEPS += $${CBE_BASE}/build/release/libinecbe.so
    }
}

win32 {
    CONFIG(debug, debug|release) {
        LIBS += $${CBE_BASE}/build/Debug/inecbe.lib
        PRE_TARGETDEPS += $${CBE_BASE}/build/Debug/inecbe.lib
    } else {
        LIBS += $${CBE_BASE}/build/Release/inecbe.lib
        PRE_TARGETDEPS += $${CBE_BASE}/build/Release/inecbe.lib
    }
}

########################################################################################################################
# Libraries
#

include("../third_party/llvm.pri")

INCLUDEPATH += $${BOOST_INCLUDE}

########################################################################################################################
# Locate build intermediate and output products
#

TARGET = test_inecbe

CONFIG(debug, debug|release) {
    unix:DESTDIR = build/debug
    win32:DESTDIR = build/Debug
} else {
    unix:DESTDIR = build/release
    win32:DESTDIR = build/Release
}

OBJECTS_DIR = $${DESTDIR}/objects
MOC_DIR = $${DESTDIR}/moc
RCC_DIR = $${DESTDIR}/rcc
UI_DIR = $${DESTDIR}/ui
