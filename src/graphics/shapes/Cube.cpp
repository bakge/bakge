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

Cube::Cube()
{
    NumIndices = 12; // 6 faces, 2 triangles per face
    NumVertices = 24; // 4 vertices per face
}


Cube::~Cube()
{
}


Cube* Cube::Create()
{
    static const Scalar Normals[] = {
        0, 0, +1.0f, // A+Z
        0, -1.0f, 0, // A-Y
        -1.0f, 0, 0, // A-X
        0, 0, +1.0f, // B+Z
        0, -1.0f, 0, // B-Y
        +1.0f, 0, 0, // B+X
        0, 0, +1.0f, // C+Z
        0, +1.0f, 0, // C+Y
        +1.0f, 0, 0, // C+X
        0, 0, +1.0f, // D+Z
        0, +1.0f, 0, // D+Y
        -1.0f, 0, 0, // D-X
        0, 0, -1.0f, // E-Z
        0, -1.0f, 0, // E-Y
        -1.0f, 0, 0, // E-X
        0, 0, -1.0f, // F-Z
        0, -1.0f, 0, // F-Y
        +1.0f, 0, 0, // F+X
        0, 0, -1.0f, // G-Z
        0, +1.0f, 0, // G+Y
        +1.0f, 0, 0, // G+X
        0, 0, -1.0f, // H-Z
        0, +1.0f, 0, // H+Y
        -1.0f, 0, 0  // H-X
    };

    static const uint32 Indices[] = {
        0, 9, 6, // +Z
        0, 3, 6,
        15, 18, 21, // -Z
        15, 12, 21,
        14, 23, 11, // +Y
        14, 2, 11,
        5, 8, 20, // -Y
        5, 17, 20,
        10, 22, 19, // +X
        10, 7, 19,
        1, 13, 16, // -X
        1, 4, 16
    };

    static const Scalar TexCoords[] = {
        0.995f, 0.005f, // A+Z
        0.005f, 0.995f, // A-Y
        0.995f, 0.005f, // A-X
        0.005f, 0.005f, // B+Z
        0.995f, 0.995f, // B-Y
        0.005f, 0.005f, // B+X
        0.005f, 0.995f, // C+Z
        0.995f, 0.005f, // C+Y
        0.005f, 0.995f, // C+X
        0.995f, 0.995f, // D+Z
        0.005f, 0.005f, // D+Y
        0.995f, 0.995f, // D-X
        0.005f, 0.005f, // E-Z
        0.005f, 0.005f, // E-Y
        0.005f, 0.005f, // E-X
        0.995f, 0.005f, // F-Z
        0.995f, 0.005f, // F-Y
        0.995f, 0.005f, // F+X
        0.995f, 0.995f, // G-Z
        0.995f, 0.995f, // G+Y
        0.995f, 0.995f, // G+X
        0.005f, 0.995f, // H-Z
        0.005f, 0.995f, // H+Y
        0.005f, 0.995f  // H-X
    };

    static const Scalar Positions[] = {
        -0.5f, -0.5f, +0.5f,
        -0.5f, -0.5f, +0.5f,
        -0.5f, -0.5f, +0.5f,
        +0.5f, -0.5f, +0.5f,
        +0.5f, -0.5f, +0.5f,
        +0.5f, -0.5f, +0.5f,
        +0.5f, +0.5f, +0.5f,
        +0.5f, +0.5f, +0.5f,
        +0.5f, +0.5f, +0.5f,
        -0.5f, +0.5f, +0.5f,
        -0.5f, +0.5f, +0.5f,
        -0.5f, +0.5f, +0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, -0.5f,
        +0.5f, +0.5f, -0.5f,
        +0.5f, +0.5f, -0.5f,
        +0.5f, +0.5f, -0.5f,
        -0.5f, +0.5f, -0.5f,
        -0.5f, +0.5f, -0.5f,
        -0.5f, +0.5f, -0.5f
    };

    Cube* C = new Cube;

    glGenBuffers(NUM_SHAPE_BUFFERS, C->Buffers);

    glBindBuffer(GL_ARRAY_BUFFER, C->Buffers[SHAPE_BUFFER_POSITIONS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 72, (GLvoid*)Positions,
                                                        GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, C->Buffers[SHAPE_BUFFER_NORMALS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 72, (GLvoid*)Normals,
                                                        GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, C->Buffers[SHAPE_BUFFER_TEXCOORDS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 48, (GLvoid*)TexCoords,
                                                        GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, C->Buffers[SHAPE_BUFFER_INDICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uint32) * 36, (GLvoid*)Indices,
                                                        GL_STATIC_DRAW);
    C->NumIndices = 36;

    C->Unbind();

    return C;
}


Scalar Cube::GetDimensions(Scalar* Width, Scalar* Height, Scalar* Length) const
{
    if(Width != NULL)
        *Width = 1.0f;

    if(Height != NULL)
        *Height = 1.0f;

    if(Length != NULL)
        *Length = 1.0f;

    return 1.0f;
}

} /* bakge */
