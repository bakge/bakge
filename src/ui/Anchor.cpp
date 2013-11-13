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

Anchor::Anchor()
{
}


Anchor::~Anchor()
{
}


Anchor* Anchor::Create()
{
    Anchor* A = new Anchor;

    glGenBuffers(1, &A->ModelMatrixBuffer);
    if(A->ModelMatrixBuffer == 0) {
        Log("Anchor: Error creating model matrix buffer\n");
        delete A;
        return NULL;
    }

    return A;
}


Vector4 BGE_NCP Anchor::SetAnchor(Scalar X, Scalar Y, Scalar Z)
{
    AnchorOffset[0] = X;
    AnchorOffset[1] = Y;
    AnchorOffset[2] = Z;

    return AnchorOffset;
}


Result Anchor::Bind() const
{
    Result Errors = BGE_SUCCESS;
    GLint Program, Location;

    /* Retrieve current shader program */
    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0)
        Errors = BGE_FAILURE;

    /* Get location of bge_Rotation uniform */
    Location = glGetUniformLocation(Program, BGE_MODEL_UNIFORM);
    if(Location < 0)
        Errors = BGE_FAILURE;

    Matrix Transformation;
    Transformation.Scale(Scale[0], Scale[1], Scale[2]);
    Transformation.Translate(-AnchorOffset[0], -AnchorOffset[1],
                                                -AnchorOffset[2]);
    Transformation *= Facing.ToMatrix();
    Transformation.Translate(Position[0], Position[1], Position[2]);
    Transformation.Translate(AnchorOffset[0], AnchorOffset[1],
                                                AnchorOffset[2]);

    glUniformMatrix4fv(Location, 1, GL_FALSE, &Transformation[0]);

    return Errors;
}

} /* bakge */
