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
* This header provides macros shared across the entire compiler back-end API.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_CBE_COMMON_H
#define CBE_CBE_COMMON_H

#include <QtGlobal>

/** \def CBE_PUBLIC_API
 *
 * Macro used to define the interface to the compiler back-end library.  Resolves to __declspec(dllexport) or
 * __declspec(dllimport) on Windows.
 */
#if (defined(CBE_BUILD))

    #define CBE_PUBLIC_API Q_DECL_EXPORT

#else

    #define CBE_PUBLIC_API Q_DECL_IMPORT

#endif

/**
 * \def CBE_PUBLIC_TEMPLATE_CLASS
 *
 * Macro you can use to define a specialization of a template class as part of the public API.  The macro exists to
 * work around limitations in Windows.
 *
 * To use this for std::vector<int>, you would write the macro as: \code
   CBE_PUBLIC_TEMPLATE_CLASS(std::vector<int>) \endcode
 *
 * On Windows, this will resolve to a template specialization with the correct __declspec(dllimport) or
 * __declspec(dllexport) values as well as other cruft needed by Windows to make the template specialization visible
 * and accessible across a DLL boundary.
 */
#if (defined(_WIN32) || defined(_WIN64))
    #if (defined(CBE_BUILD))

        #define CBE_PUBLIC_TEMPLATE_CLASS(_x) template class __declspec(dllexport) _x;

    #else

        #define CBE_PUBLIC_TEMPLATE_CLASS(_x) extern template class __declspec(dllimport) _x;

    #endif
#else

    #define CBE_PUBLIC_TEMPLATE_CLASS(_x)

#endif

#endif
