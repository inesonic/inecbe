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
* This file implements a simple function you can use to determine process memory usage.
***********************************************************************************************************************/

#include <cstdint>

#if (defined(_WIN32) || defined(_WIN64))

    #include <windows.h>
    #include <psapi.h>

#elif (defined(__APPLE__))

    #include <mach/mach.h>

#elif (defined(__linux__))

    #include <cstdio>
    #include <unistd.h>

#else

    #error Unknown platform

#endif

#include "process_memory.h"

std::int64_t processMemory() {
    std::int64_t residentMemory;

    #if (defined(_WIN32) || defined(_WIN64))

        HANDLE processHandle = GetCurrentProcess();

        PROCESS_MEMORY_COUNTERS processCounters;
        bool success = GetProcessMemoryInfo(processHandle, &processCounters, sizeof(processCounters)) ? true : false;

        if (success) {
            residentMemory = processCounters.WorkingSetSize;
        } else {
            residentMemory = -1;
        }

    #elif (defined(__APPLE__))

        /* Code below is based on the blog post at: http://blog.kuriositaet.de/?p=257 */

        struct task_basic_info taskInformation;
        mach_msg_type_number_t informationCount = TASK_BASIC_INFO_COUNT;

        kern_return_t resultCode = task_info(
            mach_task_self(),
            TASK_BASIC_INFO,
            reinterpret_cast<task_info_t>(&taskInformation),
            &informationCount
        );

        if (resultCode != KERN_SUCCESS) {
            residentMemory = -1;
        } else {
            residentMemory = taskInformation.resident_size;
        }

    #elif (defined(__linux__))

        FILE* procFile = std::fopen("/proc/self/statm", "r");

        if (procFile != nullptr) {
            unsigned long long programSize;
            unsigned long long residentSetSize;
            unsigned long long sharedPages;
            unsigned long long codeSize; // text in elf
            unsigned long long librarySize;
            unsigned long long dataSize;
            unsigned long long dirtyPages;

            int numberReadFields = fscanf(
                procFile,
                "%llu %llu %llu %llu %llu %llu %llu",
                &programSize,
                &residentSetSize,
                &sharedPages,
                &codeSize,
                &librarySize,
                &dataSize,
                &dirtyPages
            );

            if (numberReadFields == 7) {
                unsigned long pageSize = sysconf(_SC_PAGESIZE);
                residentMemory = residentSetSize * pageSize;
            } else {
                residentMemory = -1;
            }

            std::fclose(procFile);
        } else {
            residentMemory = -1;
        }

    #else

        #error Unknown platform

    #endif

    return residentMemory;
}
