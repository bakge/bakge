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

DWORD WINAPI win32_Thread::Entry(LPVOID Data)
{
    win32_Thread* T;

    T = (win32_Thread*)Data;
    T->ExitCode = T->UserEntry(T->UserData);
    T->UserEntry = NULL;
    T->UserData = NULL;

    return 0;
}


win32_Thread::win32_Thread()
{
    ThreadHandle = 0;
    ExitCode = -1;
    UserEntry = NULL;
    UserData = NULL;
}


win32_Thread::~win32_Thread()
{
    Wait();
}


win32_Thread* win32_Thread::Create(int (*EntryFunc)(void*), void* Data)
{
    win32_Thread* Thr = (win32_Thread*)calloc(1, sizeof(win32_Thread));
    if(Thr == NULL) {
        return NULL;
    }

    new(Thr) win32_Thread;

    Thr->UserData = Data;
    Thr->UserEntry = EntryFunc;
    Thr->ThreadHandle = CreateThread(NULL, 0, win32_Thread::Entry,
                        (LPVOID)Thr, 0, &(Thr->ThreadIdentifier));
    if(Thr->ThreadHandle == 0) {
        delete Thr;
        return NULL;
    }

    return Thr;
}


Result win32_Thread::Kill()
{
    if(TerminateThread(ThreadHandle, -1) == 0)
        return BGE_FAILURE;
    else
        return BGE_SUCCESS;
}


int win32_Thread::Wait()
{
    if(WaitForSingleObject(ThreadHandle, INFINITE) != WAIT_OBJECT_0)
        return -1;
    else
        return GetExitCode();
}


int win32_Thread::GetExitCode()
{
    return ExitCode;
}

} /* bakge */
