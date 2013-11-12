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

Frame::Frame()
{
}


Frame::~Frame()
{
}


Frame* Frame::Create(Scalar Width, Scalar Height)
{
    static const Scalar Normals[] = {
        0, 0, +1.0f,
        0, 0, +1.0f,
        0, 0, +1.0f,
        0, 0, +1.0f
    };

    static const uint32 Indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    static const Scalar TexCoords[] = {
        0, 0,
        0, 1,
        1, 1,
        1, 0
    };

    Frame* U = new Frame;

    glGenBuffers(NUM_SHAPE_BUFFERS, U->Buffers);

    if(U->SetDimensions(Width, Height) == BGE_FAILURE) {
        delete U;
        return NULL;
    }

    glBindBuffer(GL_ARRAY_BUFFER, U->Buffers[SHAPE_BUFFER_NORMALS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 12, (GLvoid*)Normals,
                                                        GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, U->Buffers[SHAPE_BUFFER_TEXCOORDS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 8, (GLvoid*)TexCoords,
                                                        GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, U->Buffers[SHAPE_BUFFER_INDICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uint32) * 6, (GLvoid*)Indices,
                                                        GL_STATIC_DRAW);

    return U;
}


Result Frame::SetDimensions(Scalar Width, Scalar Height)
{
    Scalar Positions[] = {
        0, 0, 0,
        0, Height, 0,
        Width, Height, 0,
        Width, 0, 0
    };

    this->Width = Width;
    this->Height = Height;

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[SHAPE_BUFFER_POSITIONS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 12, (GLvoid*)Positions,
                                                            GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return BGE_SUCCESS;
}

} /* bakge */
