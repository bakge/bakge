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

Pawn::Pawn()
{
    Scale = Vector4(1, 1, 1, 0);
}


Pawn::~Pawn()
{
}


Pawn* Pawn::Create()
{
    Pawn* P;

    try {
        P = (Pawn*)calloc(1, sizeof(Pawn));
        if(P == NULL)
            throw "Unable to allocate memory";

        new(P) Pawn;

        return P;
    } catch(const char* Message) {
        Log("ERROR: Pawn - %s\n", Message);
        free(P);
        return NULL;
    }
}


Result Pawn::SetScale(Scalar X, Scalar Y, Scalar Z)
{
    Scale[0] = X;
    Scale[1] = Y;
    Scale[2] = Z;

    return BGE_SUCCESS;
}


Vector4 BGE_NCP Pawn::GetScale() const
{
    return Scale;
}


Result Pawn::Bind() const
{
    GLint Program, Location;

    /* Retrieve current shader program */
    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0)
        return BGE_FAILURE;

    // Check if required attribute is present. If not, don't continue
    Location = glGetUniformLocation(Program, BGE_MODEL_UNIFORM);
    if(Location < 0) {
#ifdef _DEBUG
        BGE_WARN_MISSING_UNIFORM(BGE_MODEL_UNIFORM);
#endif // _DEBUG
        return BGE_FAILURE;
    }

    Matrix Transformation;
    Transformation.Scale(Scale[0], Scale[1], Scale[2]);
    Transformation *= Facing.ToMatrix();
    Transformation.Translate(Position[0], Position[1], Position[2]);

    glUniformMatrix4fv(Location, 1, GL_FALSE, &Transformation[0]);

    return BGE_SUCCESS;
}


Result Pawn::Unbind() const
{
    return Node::Unbind();
}


Quaternion BGE_NCP Pawn::SetRotation(Quaternion Rotation)
{
    Facing = Rotation;

    return Facing;
}


Quaternion BGE_NCP Pawn::GetRotation() const
{
    return Facing;
}


Quaternion BGE_NCP Pawn::Rotate(Quaternion BGE_NCP Rot)
{
    Facing *= Rot;

    return Facing;
}


Quaternion BGE_NCP Pawn::RotateGlobal(Quaternion BGE_NCP Rot)
{
    Facing = Rot * Facing;

    return Facing;
}

} /* bakge */
