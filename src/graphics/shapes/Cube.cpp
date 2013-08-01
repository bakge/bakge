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
    Dimensions[0] = 1.0f;
    Dimensions[1] = 1.0f;
    Dimensions[2] = 1.0f;
}


Cube::~Cube()
{
}


Cube* Cube::Create(Scalar Length, Scalar Width, Scalar Height)
{
    Cube* C = new Cube;
    if(C->SetDimensions(Width, Height, Length) != BGE_SUCCESS) {
        delete C;
        return NULL;
    }

    return C;
}


Result Cube::SetDimensions(Scalar X, Scalar Y, Scalar Z)
{
    /* Pass < 0 to leave dimension unchanged */
    if(X < 0)
        X = Dimensions[0];

    if(Y < 0)
        Y = Dimensions[1];

    if(Z < 0)
        Z = Dimensions[2];

    Dimensions = Vector4(X, Y, Z, 0);

    if(CreateBuffers() != BGE_SUCCESS) {
        return BGE_FAILURE;
    }

    /* So following changes affect our cube's VAO */
    BindVAO();

    /* *
     * A cube has 6 faces, each composed of two triangles.
     * We only need 8 vertices (3 components for each vertex, 24 buffer size)
     * The indices buffer will create our triangles
     * */
    NumFaces = 6;
    NumVertices = 4 * 6; /* 4 vertices per face */
    NumIndices = 36; /* 2 triangles per face, 3 vertices per triangle */
    Scalar Vertices[72];
    Scalar Normals[72];
    Scalar TexCoords[48];
    unsigned int Indices[36];

    Scalar Width = X / 2;
    Scalar Height = Y / 2;
    Scalar Length = Z / 2;

    /* *
     *       H__________G
     *      /|         /|
     *    D/_|_______C/ |         +Y HEIGHT
     *    |  |       |  |         |            MODEL COORDINATE
     *    |  |       |  |         |
     *    |  |E______|__|F        |_____ +X WIDTH
     *    | /        | /         /
     *    |/_________|/         /
     *    A          B         +Z LENGTH
     *
     * Each of A through H corresponds to 3 separate vertices.
     * They are separate so normals can be constant across each
     * of the cube's faces.
     *
     * e.g. A corresponds to vertices A+Z, A-Y and A-X
     *
     * Texcoords
     * Z and X faces: When the face normal is pointing towards you and
     * the Y axis pointing up, texcoords are as follows
     *    BL 0, 0    BR 1, 0
     *    TL 0, 1    TR 1, 1
     *
     *  e.g. +Z face:
     *        A 0, 0
     *        B 1, 0
     *        D 0, 1
     *        C 1, 1
     *
     * Y faces:
     *   +Y:
     *     D 0, 0
     *     C 1, 0
     *     H 0, 1
     *     G 1, 1
     *
     *   -Y:
     *     E 0, 0
     *     F 1, 0
     *     A 0, 1
     *     B 1, 1
     * */

    memset((void*)Normals, 0, sizeof(Normals[0]) * 72);
    memset((void*)TexCoords, 0, sizeof(TexCoords[0]) * 48);

    /* +Z */
    Indices[0] = 0;
    Indices[1] = 9;
    Indices[2] = 6;
    Indices[3] = 0;
    Indices[4] = 3;
    Indices[5] = 6;

    /* -Z */
    Indices[6] = 15;
    Indices[7] = 18;
    Indices[8] = 21;
    Indices[9] = 15;
    Indices[10] = 12;
    Indices[11] = 21;

    /* -X */
    Indices[12] = 14;
    Indices[13] = 23;
    Indices[14] = 11;
    Indices[15] = 14;
    Indices[16] = 2;
    Indices[17] = 11;

    /* +X */
    Indices[18] = 5;
    Indices[19] = 8;
    Indices[20] = 20;
    Indices[21] = 5;
    Indices[22] = 17;
    Indices[23] = 20;

    /* +Y */
    Indices[24] = 10;
    Indices[25] = 22;
    Indices[26] = 19;
    Indices[27] = 10;
    Indices[28] = 7;
    Indices[29] = 19;

    /* -Y */
    Indices[30] = 1;
    Indices[31] = 13;
    Indices[32] = 16;
    Indices[33] = 1;
    Indices[34] = 4;
    Indices[35] = 16;

    /* A+Z */
    Vertices[0] = -Width;
    Vertices[1] = -Height;
    Vertices[2] = +Length;
    /* A-Y */
    Vertices[3] = -Width;
    Vertices[4] = -Height;
    Vertices[5] = +Length;
    /* A-X */
    Vertices[6] = -Width;
    Vertices[7] = -Height;
    Vertices[8] = +Length;

    /* B+Z */
    Vertices[9] = +Width;
    Vertices[10] = -Height;
    Vertices[11] = +Length;
    /* B-Y */
    Vertices[12] = +Width;
    Vertices[13] = -Height;
    Vertices[14] = +Length;
    /* B+X */
    Vertices[15] = +Width;
    Vertices[16] = -Height;
    Vertices[17] = +Length;

    /* C+Z */
    Vertices[18] = +Width;
    Vertices[19] = +Height;
    Vertices[20] = +Length;
    /* C+Y */
    Vertices[21] = +Width;
    Vertices[22] = +Height;
    Vertices[23] = +Length;
    /* C+X */
    Vertices[24] = +Width;
    Vertices[25] = +Height;
    Vertices[26] = +Length;

    /* D+Z */
    Vertices[27] = -Width;
    Vertices[28] = +Height;
    Vertices[29] = +Length;
    /* D+Y */
    Vertices[30] = -Width;
    Vertices[31] = +Height;
    Vertices[32] = +Length;
    /* D-X */
    Vertices[33] = -Width;
    Vertices[34] = +Height;
    Vertices[35] = +Length;

    /* E-Z */
    Vertices[36] = -Width;
    Vertices[37] = -Height;
    Vertices[38] = -Length;
    /* E-Y */
    Vertices[39] = -Width;
    Vertices[40] = -Height;
    Vertices[41] = -Length;
    /* E-X */
    Vertices[42] = -Width;
    Vertices[43] = -Height;
    Vertices[44] = -Length;

    /* F-Z */
    Vertices[45] = +Width;
    Vertices[46] = -Height;
    Vertices[47] = -Length;
    /* F-Y */
    Vertices[48] = +Width;
    Vertices[49] = -Height;
    Vertices[50] = -Length;
    /* F+X */
    Vertices[51] = +Width;
    Vertices[52] = -Height;
    Vertices[53] = -Length;

    /* G-Z */
    Vertices[54] = +Width;
    Vertices[55] = +Height;
    Vertices[56] = -Length;
    /* G+Y */
    Vertices[57] = +Width;
    Vertices[58] = +Height;
    Vertices[59] = -Length;
    /* G+X */
    Vertices[60] = +Width;
    Vertices[61] = +Height;
    Vertices[62] = -Length;

    /* H-Z */
    Vertices[63] = -Width;
    Vertices[64] = +Height;
    Vertices[65] = -Length;
    /* H+Y */
    Vertices[66] = -Width;
    Vertices[67] = +Height;
    Vertices[68] = -Length;
    /* H-X */
    Vertices[69] = -Width;
    Vertices[70] = +Height;
    Vertices[71] = -Length;

    Normals[2] = +1.0f;
    Normals[4] = -1.0f;
    Normals[6] = -1.0f;
    Normals[11] = +1.0f;
    Normals[13] = -1.0f;
    Normals[15] = + 1.0f;
    Normals[20] = +1.0f;
    Normals[22] = + 1.0f;
    Normals[24] = +1.0f;
    Normals[29] = +1.0f;
    Normals[31] = +1.0f;
    Normals[33] = -1.0f;
    Normals[38] = -1.0f;
    Normals[40] = -1.0f;
    Normals[42] = -1.0f;
    Normals[47] = -1.0f;
    Normals[49] = -1.0f;
    Normals[51] = +1.0f;
    Normals[56] = -1.0f;
    Normals[58] = +1.0f;
    Normals[60] = +1.0f;
    Normals[65] = -1.0f;
    Normals[67] = +1.0f;
    Normals[69] = -1.0f;

    TexCoords[3] = 1;
    TexCoords[4] = 1;
    TexCoords[6] = 1;
    TexCoords[8] = 1;
    TexCoords[9] = 1;
    TexCoords[12] = 1;
    TexCoords[13] = 1;
    TexCoords[14] = 1;
    TexCoords[17] = 1;
    TexCoords[19] = 1;
    TexCoords[22] = 1;
    TexCoords[23] = 1;
    TexCoords[24] = 1;
    TexCoords[32] = 1;
    TexCoords[34] = 1;
    TexCoords[37] = 1;
    TexCoords[38] = 1;
    TexCoords[39] = 1;
    TexCoords[40] = 1;
    TexCoords[41] = 1;
    TexCoords[42] = 1;
    TexCoords[43] = 1;
    TexCoords[45] = 1;
    TexCoords[47] = 1;

    glBindBuffer(GL_ARRAY_BUFFER, MeshBuffers[MESH_BUFFER_POSITIONS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * 72, Vertices,
                                                        GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, MeshBuffers[MESH_BUFFER_NORMALS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * 72, Normals,
                                                        GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, MeshBuffers[MESH_BUFFER_TEXCOORDS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * 48, TexCoords,
                                                        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MeshBuffers[MESH_BUFFER_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * 36,
                                            Indices, GL_STATIC_DRAW);

    Unbind();

    return BGE_SUCCESS;
}

} /* bakge */
