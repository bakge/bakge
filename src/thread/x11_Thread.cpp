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

void* x11_Thread::Entry(void* Data)
{
    x11_Thread* T;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    T = (x11_Thread*)Data;
    T->ExitCode = T->UserEntry(T->UserData);
    T->UserEntry = NULL;
    T->UserData = NULL;

    pthread_exit(NULL);
}

x11_Thread::x11_Thread()
{
    ThreadHandle = 0;
    ExitCode = -1;
    UserEntry = NULL;
    UserData = NULL;
}


x11_Thread::~x11_Thread()
{
    Wait();
}


Result x11_Thread::Kill()
{
    if(pthread_cancel(ThreadHandle) < 0)
        return BGE_FAILURE;
    else
        return BGE_SUCCESS;
}


x11_Thread* x11_Thread::Create(int (*EntryFunc)(void*), void* EntryData)
{
    x11_Thread* T = (x11_Thread*)calloc(1, sizeof(x11_Thread));
    if(T == NULL) {
        return NULL;
    }

    new(T) x11_Thread;

    T->UserEntry = EntryFunc;
    T->UserData = EntryData;

    int Result = pthread_create(&(T->ThreadHandle), NULL, Entry, (void*)T);
    if(Result < 0) {
        delete T;
        return NULL;
    }

    return T;
}


int x11_Thread::Wait()
{
    if(pthread_join(ThreadHandle, NULL) != 0) {
        return -1;
    } else {
        return GetExitCode();
    }
}


int x11_Thread::GetExitCode()
{
    return ExitCode;
}

} /* bakge */
