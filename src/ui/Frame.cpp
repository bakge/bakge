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

    Frame* F;

    try {
        F = (Frame*)calloc(1, sizeof(Frame));
        if(F == NULL)
            throw "Unable to allcoate memory";

        new(F) Frame;

        if(F->SetDimensions(Width, Height) == BGE_FAILURE) {
            delete F;
            throw "Unable to set Frame dimensions";
        }

        glBindBuffer(GL_ARRAY_BUFFER, F->GetBuffer(SHAPE_BUFFER_NORMALS));
        glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 12, (GLvoid*)Normals,
                                                            GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, F->GetBuffer(SHAPE_BUFFER_TEXCOORDS));
        glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 8, (GLvoid*)TexCoords,
                                                            GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, F->GetBuffer(SHAPE_BUFFER_INDICES));
        glBufferData(GL_ARRAY_BUFFER, sizeof(uint32) * 6, (GLvoid*)Indices,
                                                            GL_STATIC_DRAW);

        return F;
    } catch(const char* Message) {
        Log("ERROR: Frame - %s\n", Message);
        free(F);
        return NULL;
    }
}


Result Frame::SetDimensions(Scalar Width, Scalar Height)
{
    IRectangle<Scalar>::SetDimensions(Width, Height);

    Scalar Positions[] = {
        0, 0, 0,
        0, Height, 0,
        Width, Height, 0,
        Width, 0, 0
    };

    this->Width = Width;
    this->Height = Height;

    glBindBuffer(GL_ARRAY_BUFFER, GetBuffer(SHAPE_BUFFER_POSITIONS));
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 12, (GLvoid*)Positions,
                                                            GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return BGE_SUCCESS;
}

} /* bakge */
