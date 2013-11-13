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

Crowd::Crowd() : BufferList(1)
{
    Population = 0;
    Capacity = 0;
    Positions = NULL;
    Rotations = NULL;
    Scales = NULL;
}


Crowd::~Crowd()
{
    Clear();
}


Crowd* Crowd::Create(int ReserveMembers)
{
    try {
        Crowd* C = new Crowd;
        if(C == NULL)
            throw "Unable to allocate memory";

        glGenBuffers(1, &C->ModelMatrixBuffer);
        if(C->ModelMatrixBuffer == 0) {
            Log("Crowd: Error creating model matrix buffer\n");
            delete C;
            return NULL;
        }

        C->Reserve(ReserveMembers);

        return C;
    } catch(const char* Message) {
        Log("ERROR: Crowd - %s\n", Message);
        return NULL;
    }
}


Result Crowd::Bind() const
{
    GLint Program, Location;

    /* Retrieve current shader program */
    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0)
        return BGE_FAILURE;

    /* Retrieve location of the bge_Translation vec4 */
    Location = glGetUniformLocation(Program, BGE_MODEL_UNIFORM);
    if(Location < 0)
        return BGE_FAILURE;

    Matrix CrowdTransform;
    CrowdTransform *= Facing.ToMatrix();
    CrowdTransform.Translate(Position[0], Position[1], Position[2]);

    glUniformMatrix4fv(Location, 1, GL_FALSE, &CrowdTransform[0]);

    Location = glGetAttribLocation(Program, BGE_INSTANCEMODEL_ATTRIBUTE);
    if(Location < 0) {
#ifdef _DEBUG
        BGE_WARN_MISSING_ATTRIBUTE(BGE_INSTANCEMODEL_ATTRIBUTE);
#endif // _DEBUG
        return BGE_FAILURE;
    }

    glBindBuffer(GL_ARRAY_BUFFER, *Buffers);

    for(int i=0;i<4;++i) {
        glEnableVertexAttribArray(Location + i);
        glVertexAttribPointer(Location + i, 4, GL_FLOAT, GL_FALSE,
                        sizeof(Matrix), (void*)(sizeof(Scalar) * 4 * i));
        glVertexAttribDivisor(Location + i, 1);
    }

    return BGE_SUCCESS;
}


Result Crowd::Unbind() const
{
    GLint Program, Location;

    /* Retrieve current shader program */
    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0)
        return BGE_FAILURE;

    Location = glGetUniformLocation(Program, BGE_MODEL_UNIFORM);
    if(Location < 0)
        return BGE_FAILURE;

    glUniformMatrix4fv(Location, 1, GL_FALSE, &Matrix::Identity[0]);

    Location = glGetAttribLocation(Program, BGE_INSTANCEMODEL_ATTRIBUTE);
    if(Location < 0) {
        return BGE_FAILURE;
    }

    for(int i=0;i<4;++i)
        glDisableVertexAttribArray(Location + i);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return BGE_SUCCESS;
}


Result Crowd::Clear()
{
    delete[] Positions;
    delete[] Rotations;
    delete[] Scales;
    Positions = NULL;
    Rotations = NULL;
    Scales = NULL;

    Population = 0;

    return BGE_SUCCESS;
}


Result Crowd::Reserve(int NumMembers)
{
    Clear();

    Capacity = NumMembers;
    Population = NumMembers;

    Positions = new Scalar[NumMembers * 3];
    Rotations = new Quaternion[NumMembers];
    Scales = new Scalar[NumMembers * 3];

    for(int i=0;i<NumMembers;++i) {
        Positions[i * 3 + 0] = 0;
        Positions[i * 3 + 1] = 0;
        Positions[i * 3 + 2] = 0;
        Scales[i * 3 + 0] = 1;
        Scales[i * 3 + 1] = 1;
        Scales[i * 3 + 2] = 1;
    }

    /* Allocates the buffer */
    glBindBuffer(GL_ARRAY_BUFFER, *Buffers);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 16 * NumMembers,
                                            NULL, GL_DYNAMIC_DRAW);

    /* Now set each matrix in the buffer to identity */
    GLint Stride = sizeof(Scalar) * 16;
    for(int i=0;i<NumMembers;++i) {
        glBufferSubData(GL_ARRAY_BUFFER, Stride * i, Stride,
                        (const GLvoid*)&Matrix::Identity[0]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return BGE_SUCCESS;
}


Result Crowd::TranslateMember(int MemberIndex, Scalar X, Scalar Y, Scalar Z)
{
    if(MemberIndex < 0 || MemberIndex >= Capacity) {
        Log("ERROR: Crowd - Member index out of range\n");
        return BGE_FAILURE;
    }

    Positions[MemberIndex * 3 + 0] += X;
    Positions[MemberIndex * 3 + 1] += Y;
    Positions[MemberIndex * 3 + 2] += Z;

    SetDataStore(MemberIndex);

    return BGE_SUCCESS;
}


Result Crowd::RotateMember(int MemberIndex, Quaternion BGE_NCP Rotation)
{
    if(MemberIndex < 0 || MemberIndex >= Capacity) {
        Log("ERROR: Crowd - Member index out of range\n");
        return BGE_FAILURE;
    }

    /* Rotate the member */
    Rotations[MemberIndex] *= Rotation;

    SetDataStore(MemberIndex);

    return BGE_SUCCESS;
}


Result Crowd::RotateMemberGlobal(int MemberIndex, Quaternion BGE_NCP Rot)
{
    if(MemberIndex < 0 || MemberIndex >= Capacity) {
        Log("ERROR: Crowd - Member index out of range\n");
        return BGE_FAILURE;
    }

    /* Rotate the member */
    Rotations[MemberIndex] = Rot * Rotations[MemberIndex];

    SetDataStore(MemberIndex);

    return BGE_SUCCESS;
}


Result Crowd::ScaleMember(int MemberIndex, Scalar X, Scalar Y, Scalar Z)
{
    if(MemberIndex < 0 || MemberIndex >= Capacity) {
        Log("ERROR: Crowd - Member index out of range\n");
        return BGE_FAILURE;
    }

    /* Scale the member */
    Scales[MemberIndex * 3 + 0] *= X;
    Scales[MemberIndex * 3 + 1] *= Y;
    Scales[MemberIndex * 3 + 2] *= Z;

    SetDataStore(MemberIndex);

    return BGE_SUCCESS;
}


Quaternion BGE_NCP Crowd::SetMemberRotation(int Index, Quaternion BGE_NCP Rot)
{
    if(Index < 0 || Index >= Capacity) {
        Log("ERROR: Crowd - Member index out of range\n");
        return Quaternion::Identity;
    }

    Rotations[Index] = Rot;

    SetDataStore(Index);

    return Rotations[Index];
}


Quaternion BGE_NCP Crowd::GetMemberRotation(int Index) const
{
    if(Index < 0 || Index >= Capacity) {
        Log("ERROR: Crowd - Member index out of range\n");
        return Quaternion::Identity;
    }

    return Rotations[Index];
}


Result Crowd::SetDataStore(int Index)
{
    /* Create a new model matrix to copy into the buffer */
    Matrix Transformation;
    Transformation.Scale(Scales[Index * 3 + 0],
                        Scales[Index * 3 + 1],
                        Scales[Index * 3 + 2]);
    Transformation *= Rotations[Index].ToMatrix();
    Transformation.Translate(Positions[Index * 3 + 0],
                            Positions[Index * 3 + 1],
                            Positions[Index * 3 + 2]);

    glBindBuffer(GL_ARRAY_BUFFER, *Buffers);
    glBufferSubData(GL_ARRAY_BUFFER, 64 * Index, 64, &Transformation[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return BGE_SUCCESS;
}

} /* bakge */
