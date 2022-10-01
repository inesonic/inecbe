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
* This header defines a number of defines that can be used to disable compiler warnings generated by the LLVM and clang
* headers.  This file is included to supress warnings in third party libraries.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef WARNINGS_H
#define WARNINGS_H

/**
 * \def SUPPRESS_LLVM_WARNINGS
 *
 * You can use this macro to suppress warnings generated by the LLVM and CLANG headers.  Place the macro in the C++
 * implementation file before including any of the LLVM or CLANG headers.
 */

/**
 * \def RESTORE_LLVM_WARNINGS
 *
 * You can use this macro to suppress warnings generated by the LLVM and CLANG headers.  Place the macro in the C++
 * implementation file before including any of the LLVM or CLANG headers.
 */

#if (defined(_WIN32) || defined(_WIN64))

    #define SUPPRESS_LLVM_WARNINGS __pragma( \
        warning(                             \
            disable:4100;                    \
            disable:4146;                    \
            disable:4141;                    \
            disable:4267;                    \
            disable:4624;                    \
            disable:4244;                    \
            disable:4291                     \
        )                                    \
    )

    #define RESTORE_LLVM_WARNINGS __pragma( \
        warning(                            \
            default:4100;                   \
            default:4146;                   \
            default:4141;                   \
            default:4267;                   \
            default:4624;                   \
            default:4244;                   \
            default:4291                    \
        )                                   \
    )

#elif (defined(__linux__))

    #define SUPPRESS_LLVM_WARNINGS _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"") \
                                   _Pragma("GCC diagnostic ignored \"-Wmissing-field-initializers\"")

    #define RESTORE_LLVM_WARNINGS _Pragma("GCC diagnostic warning \"-Wunused-parameter\"") \
                                  _Pragma("GCC diagnostic warning \"-Wmissing-field-initializers\"")

#elif (defined(__APPLE__))

    #define SUPPRESS_LLVM_WARNINGS _Pragma("clang diagnostic ignored \"-Wunused-parameter\"")
    #define RESTORE_LLVM_WARNINGS _Pragma("clang diagnostic warning \"-Wunused-parameter\"")

#else

    #error Unknown platform

#endif

#endif
