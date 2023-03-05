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

TEMPLATE = lib

########################################################################################################################
# Basic build characteristics
#

QT += core
CONFIG += shared c++14

DEFINES += CBE_BUILD
DEFINES += CBE_EXTERNAL_LINKER

########################################################################################################################
# Custom C++ compiler type supporting -fno-rtti switch.
#
#   We need this on at least MacOS if we inherit from any of the CLang or LLVM classes as they were built without RTTI
#   support.  While we *could* avoid RTTI everywhere, there are a few places where we use dynamic_cast.
#

unix {
    cpp_no_rtti.name = cpp_no_rtti
    cpp_no_rtti.input = SOURCES_NO_RTTI
    cpp_no_rtti.dependency_type = TYPE_C
    cpp_no_rtti.variable_out = OBJECTS
    cpp_no_rtti.output = ${QMAKE_VAR_OBJECTS_DIR}${QMAKE_FILE_IN_BASE}$${first(QMAKE_EXT_OBJ)}
    cpp_no_rtti.commands = $${QMAKE_CXX} $(CXXFLAGS) $${QMAKE_CXXFLAGS} -fno-rtti $(INCPATH) -c ${QMAKE_FILE_IN} \
                           -o ${QMAKE_FILE_OUT}

    QMAKE_EXTRA_COMPILERS += cpp_no_rtti
}

########################################################################################################################
# Public includes
#

INCLUDEPATH += include
API_HEADERS = include/cbe_common.h \
              include/cbe_source_range.h \
              include/cbe_job_queue.h \
              include/cbe_compiler.h \
              include/cbe_compiler_notifier.h \
              include/cbe_compiler_context.h \
              include/cbe_compiler_diagnostic.h \
              include/cbe_cpp_source_range.h \
              include/cbe_cpp_compiler_notifier.h \
              include/cbe_cpp_compiler_context.h \
              include/cbe_cpp_compiler_diagnostic.h \
              include/cbe_cpp_compiler.h \
              include/cbe_linker.h \
              include/cbe_linker_notifier.h \
              include/cbe_linker_context.h \
              include/cbe_dynamic_library_linker.h \
              include/cbe_dynamic_library_loader.h \
              include/cbe_loader_notifier.h

########################################################################################################################
# Source files
#

SOURCES = source/cbe_compiler.cpp \
          source/cbe_compiler_private.cpp \
          source/compiler_impl.cpp \
          source/cbe_compiler_notifier.cpp \
          source/cbe_compiler_context.cpp \
          source/cbe_compiler_context_private.cpp \
          source/cbe_compiler_diagnostic.cpp \
          source/cbe_compiler_diagnostic_private.cpp \
          source/cbe_cpp_compiler.cpp \
          source/cbe_cpp_source_range.cpp \
          source/cbe_cpp_source_range_private.cpp \
          source/cbe_cpp_compiler_notifier.cpp \
          source/cbe_cpp_compiler_context.cpp \
          source/cbe_cpp_compiler_context_private.cpp \
          source/cbe_cpp_compiler_diagnostic.cpp \
          source/cbe_cpp_compiler_diagnostic_private.cpp \
          source/cbe_source_range.cpp \
          source/cbe_source_range_private.cpp \
          source/cbe_linker.cpp \
          source/cbe_linker_private.cpp \
          source/cbe_linker_notifier.cpp \
          source/cbe_linker_context.cpp \
          source/cbe_linker_context_private.cpp \
          source/cbe_dynamic_library_linker.cpp \
          source/cbe_dynamic_library_loader.cpp \
          source/cbe_dynamic_library_loader_private.cpp \
          source/cbe_loader_notifier.cpp

win32 {
    SOURCES += source/diagnostic_consumer.cpp
}

unix {
    SOURCES_NO_RTTI = source/diagnostic_consumer.cpp
}

########################################################################################################################
# Private includes
#

INCLUDEPATH += source
PRIVATE_HEADERS = source/warnings.h \
                  source/cbe_compiler_private.h \
                  source/compiler_impl.h \
                  source/cbe_compiler_context_private.h \
                  source/cbe_compiler_diagnostic_private.h \
                  source/cbe_cpp_compiler_context_private.h \
                  source/cbe_cpp_compiler_diagnostic_private.h \
                  source/diagnostic_consumer.h \
                  source/cbe_source_range_private.h \
                  source/cbe_cpp_source_range_private.h \
                  source/cbe_linker_private.h \
                  source/cbe_dynamic_library_loader_private.h \
                  source/linker_impl.h \
                  source/cbe_linker_context_private.h

########################################################################################################################
# Deal with multiple linker implementations
#

contains(DEFINES, CBE_EXTERNAL_LINKER) {
    SOURCES += source/linker_impl_external.cpp
    PRIVATE_HEADERS += source/linker_impl_external.h
} else {
    SOURCES += source/linker_impl_internal.cpp
    PRIVATE_HEADERS += source/linker_impl_internal.h
}

########################################################################################################################
# Setup headers and installation
#

HEADERS = $$API_HEADERS $$PRIVATE_HEADERS

########################################################################################################################
# LLVM Libraries
#

include("../third_party/llvm.pri")

########################################################################################################################
# Build error code table as a final step.
#

BUILD_CBE_ERROR_CODES_ROOT=$$shadowed($$PWD)/../build_cbe_error_codes/

CONFIG(debug, debug|release) {
    unix {
        macx {
            BUILD_CBE_ERROR_CODES_DIR= \
                $${BUILD_CBE_ERROR_CODES_ROOT}/build/debug/build_cbe_error_codes.app/Contents/MacOS/
        } else {
            BUILD_CBE_ERROR_CODES_DIR=$${BUILD_CBE_ERROR_CODES_ROOT}/build/debug/build_cbe_error_codes
        }

        BUILD_CBE_ERROR_CODES=$${BUILD_CBE_ERROR_CODES_DIR}/build_cbe_error_codes
    }

    win32 {
        BUILD_CBE_ERROR_CODES=$${BUILD_CBE_ERROR_CODES_ROOT}/build/Debug/build_cbe_error_codes
    }
} else {
    unix {
        macx {
            BUILD_CBE_ERROR_CODES_DIR= \
                $${BUILD_CBE_ERROR_CODES_ROOT}/build/release/build_cbe_error_codes.app/Contents/MacOS/
        } else {
            BUILD_CBE_ERROR_CODES_DIR=$${BUILD_CBE_ERROR_CODES_ROOT}/build/release/build_cbe_error_codes
        }

        BUILD_CBE_ERROR_CODES=$${BUILD_CBE_ERROR_CODES_DIR}/build_cbe_error_codes
    }

    win32 {
        BUILD_CBE_ERROR_CODES=$${BUILD_CBE_ERROR_CODES_ROOT}/build/Release/build_cbe_error_codes
    }
}

unix {
    QMAKE_POST_LINK = mkdir -p include ; $${BUILD_CBE_ERROR_CODES} include/cbe_cpp_compiler_error_codes.h
}

win32 {
    QMAKE_POST_LINK = MKDIR "include" && $${BUILD_CBE_ERROR_CODES} include/cbe_cpp_compiler_error_codes.h
}

########################################################################################################################
# Locate build intermediate and output products
#

TARGET = inecbe

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
