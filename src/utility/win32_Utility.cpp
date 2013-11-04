/* *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Paul Holden et al. (See AUTHORS)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * */

#include <bakge/Bakge.h>

namespace bakge
{

static LARGE_INTEGER ClockFreq;
static LARGE_INTEGER StartCount;

Result _PlatformInit(int argc, char* argv[])
{
    extern void _BeginClock();
    _BeginClock();

    return BGE_SUCCESS;
}


Result _PlatformDeinit()
{
    return BGE_SUCCESS;
}


void PlatformSystemInfo()
{
    OSVERSIONINFOEX Info;
    SYSTEM_INFO Sys;

    Info.dwOSVersionInfoSize = sizeof(Info);

    GetVersionEx((LPOSVERSIONINFOA)&Info);
    GetNativeSystemInfo(&Sys);

    const char* VerString = NULL;

    int Metric = -1;

    if(Info.dwMajorVersion == 5) {
        switch(Info.dwMinorVersion) {

        case 0:
            VerString = "Windows 2000";
            break;

        case 1:
            VerString = "Windows XP";
            break;

        case 2:
            if(Sys.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 &&
                                    Info.wProductType == VER_NT_WORKSTATION) {
                VerString = "Windows XP Professional x64 Edition";
                break;
            }

            if(Info.wSuiteMask & VER_SUITE_WH_SERVER) {
                VerString = "Windows Home Server";
                break;
            }

            Metric = GetSystemMetrics(SM_SERVERR2);
            if(Metric != 0) {
                VerString = "Windows Server 2003 R2";
            } else {
                VerString = "Windows Server 2003";
            }

            break;

         default:
            VerString = "Unknown";
            break;
        }
    } else if(Info.dwMajorVersion == 6) {
        switch(Info.dwMinorVersion) {

        case 0:
            if(Info.wProductType == VER_NT_WORKSTATION) {
                VerString = "Windows Vista";
            } else {
                VerString = "Windows Server 2008";
            }

            break;

        case 1:
            if(Info.wProductType == VER_NT_WORKSTATION) {
                VerString = "Windows 7";
            } else {
                VerString = "Windows Server 2008 R2";
            }

            break;

        case 2:
            if(Info.wProductType == VER_NT_WORKSTATION) {
                VerString = "Windows 8";
            } else {
                VerString = "Windows Server 2012";
            }

            break;

        case 3:
            if(Info.wProductType == VER_NT_WORKSTATION) {
                VerString = "Windows 8.1";
            } else {
                VerString = "Windows Server 2012 R2";
            }

            break;

        default:
            VerString = "Unknown";
            break;
        }
    }

    Log("%s\n", VerString);
    Log("  - Build %d\n", Info.dwBuildNumber);
    Log("  - PlatformID %d\n", Info.dwPlatformId);
    Log("  - Service Pack %d.%d\n", Info.wServicePackMajor,
                                        Info.wServicePackMinor);

    const char* ProcString = "i486";

    switch(Sys.dwProcessorType) {

    case PROCESSOR_INTEL_386:
        ProcString = "i386";
    case PROCESSOR_INTEL_486:
        if((Sys.wProcessorRevision & 0xFFFF0000) == 0xFF0000) {
            Log("%s %d-%d\n", ProcString, (Sys.wProcessorRevision &
                0x0000FF00) - 0xA, Sys.wProcessorRevision & 0x000000FF);
        } else {
            Log("%s %d-%d\n", ProcString, (Sys.wProcessorRevision &
               0xFFFF0000) + 'A', Sys.wProcessorRevision & 0x0000FFFF);
        }

        break;

    case PROCESSOR_INTEL_PENTIUM:
        Log("Intel Pentium\n");
        break;

    case PROCESSOR_AMD_X8664:
        Log("AMD x86-64\n");
        break;

    default:
        Log("%d\n", Sys.dwProcessorType);
        break;
    }

    Log("  - %d processors\n", Sys.dwNumberOfProcessors);
}

} /* bakge */
