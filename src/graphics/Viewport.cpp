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
#ifdef _DEBUG
#include <bakge/internal/Debug.h>
#endif // _DEBUG

namespace bakge
{

Viewport::Viewport()
{
    X0 = 0;
    Y0 = 0;
    X1 = 800;
    Y1 = 600;
}


Viewport::Viewport(uint16 _X0, uint16 _Y0, uint16 _X1, uint16 _Y1)
{
    X0 = _X0;
    Y0 = _Y0;
    X1 = _X1;
    Y1 = _Y1;
}


Viewport::~Viewport()
{
}


Result Viewport::Bind() const
{
    glViewport(X0, Y0, X1, Y1);

    return BGE_SUCCESS;
}


void Viewport::GetOrigin(uint32* X, uint32* Y)
{
    if(X)
        *X = X0;

    if(Y)
        *Y = Y0;
}


void Viewport::GetSpan(uint32* X, uint32* Y)
{
    if(X)
        *X = X1 - X0;

    if(Y)
        *Y = Y1 - Y0;
}


void Viewport::SetOrigin(uint32 X, uint32 Y)
{
    X0 = X;
    Y0 = Y;
}


void Viewport::SetSpan(uint32 X, uint32 Y)
{
    X1 = X0 + X;
    Y1 = Y0 + Y;
}

} /* bakge */
