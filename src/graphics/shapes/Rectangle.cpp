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
#include <bakge/internal/Error.h>

namespace bakge
{

Rectangle::Rectangle()
{
    Width = 0;
    Height = 0;
}


Rectangle::~Rectangle()
{
}


Rectangle* Rectangle::Create(Scalar Width, Scalar Height)
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

    Rectangle* R;

    try {
        R = (Rectangle*)calloc(1, sizeof(Rectangle));
        if(R == NULL)
            throw BGE_FACTORY_MESSAGE_BADALLOC;

        new(R) Rectangle;

        R->Width = Width;
        R->Height = Height;
        R->NumIndices = 6;

        if(R->SetDimensions(Width, Height) != BGE_SUCCESS) {
            delete R;
            throw "Unable to set Rectangle dimensions";
        }

        glBindBuffer(GL_ARRAY_BUFFER, R->GetBuffer(SHAPE_BUFFER_NORMALS));
        glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 12, (GLvoid*)Normals,
                                                            GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, R->GetBuffer(SHAPE_BUFFER_TEXCOORDS));
        glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 8, (GLvoid*)TexCoords,
                                                            GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, R->GetBuffer(SHAPE_BUFFER_INDICES));
        glBufferData(GL_ARRAY_BUFFER, sizeof(uint32) * 6, (GLvoid*)Indices,
                                                            GL_STATIC_DRAW);

        R->Unbind();

        return R;
    } catch(const char* Message) {
        Log("ERROR: Rectangle - %s\n", Message);
        free(R);
        return NULL;
    }
}


Result Rectangle::SetDimensions(Scalar Width, Scalar Height)
{
    IRectangle<Scalar>::SetDimensions(Width, Height);

    Width /= 2;
    Height /= 2;

    Scalar L = -Width;
    Scalar R = Width;
    Scalar T = Height;
    Scalar B = -Height;

    Scalar Positions[] = {
        L, B, 0,
        L, T, 0,
        R, T, 0,
        R, B, 0
    };

    glBindBuffer(GL_ARRAY_BUFFER, GetBuffer(SHAPE_BUFFER_POSITIONS));
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 12, (GLvoid*)Positions,
                                                            GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return BGE_SUCCESS;
}

} /* bakge */
