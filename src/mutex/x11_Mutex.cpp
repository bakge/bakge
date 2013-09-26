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

x11_Mutex::x11_Mutex()
{
}


x11_Mutex::~x11_Mutex()
{
    pthread_mutex_destroy(&MutexHandle);
}


x11_Mutex* x11_Mutex::Create()
{

    x11_Mutex* M = new x11_Mutex;

    if(pthread_mutex_init(&(M->MutexHandle), NULL) != 0) {
        /* Mutex creation failed */
        printf("Mutex creation failed");
        exit(1);
    }

    return M;
}


int x11_Mutex::Lock()
{
    /* 0 == Success, else returns error */
    return pthread_mutex_lock(&MutexHandle);
}


int x11_Mutex::Unlock()
{
    return pthread_mutex_unlock(&MutexHandle);
}

} /* bakge */