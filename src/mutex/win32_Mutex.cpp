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

win32_Mutex::win32_Mutex()
{
}


win32_Mutex::~win32_Mutex()
{
    DeleteCriticalSection(&CriticalSectionHandle);
}


win32_Mutex* win32_Mutex::Create()
{
    win32_Mutex* M = (win32_Mutex*)calloc(1, sizeof(win32_Mutex));
    if(M == NULL) {
        Log("ERROR: win32_Mutex - Couldn't allocate memory.\n");
        return NULL;
    }

    new(M) win32_Mutex;

    InitializeCriticalSection(&M->CriticalSectionHandle);

    return M;
}


Result win32_Mutex::Lock()
{
    // Blocks until available. Mutually exclusive access.
    EnterCriticalSection(&CriticalSectionHandle);

    return BGE_SUCCESS;
}


Result win32_Mutex::TryLock()
{
    if(TryEnterCriticalSection(&CriticalSectionHandle) != 0)
        return BGE_SUCCESS;

    return BGE_FAILURE;
}


Result win32_Mutex::Unlock()
{
    LeaveCriticalSection(&CriticalSectionHandle);

    return BGE_SUCCESS;
}

} /* bakge */
