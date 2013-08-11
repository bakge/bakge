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

Crowd::Crowd()
{
    Population = 0;
    Capacity = 0;
    Members = NULL;
}


Crowd::~Crowd()
{
    Clear();
}


Crowd* Crowd::Create(int ReserveMembers)
{
    Crowd* C = new Crowd;

    C->Reserve(ReserveMembers);

    return C;
}


Result Crowd::Bind() const
{
    /* Node::Bind sets up attribute pointers, etc */
    return Node::Bind();
}


Result Crowd::Unbind() const
{
    return BGE_FAILURE;
}


Result Crowd::Clear()
{
    if(Members == NULL)
        return BGE_FAILURE;

    delete[] Members;
    Members = NULL;

    return BGE_SUCCESS;
}


Result Crowd::Reserve(int NumMembers)
{
    Clear();

    Capacity = NumMembers;
    Population = NumMembers;
    Members = new Matrix[NumMembers];

    glBindBuffer(GL_ARRAY_BUFFER, ModelMatrixBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 16 * NumMembers, Members,
                                                            GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return BGE_SUCCESS;
}


Result Crowd::Translate(int MemberIndex, Scalar X, Scalar Y, Scalar Z)
{
    /* Prevent out-of-bounds transformations */
    if(MemberIndex < 0 || MemberIndex >= Capacity)
        return BGE_FAILURE;

    Members[MemberIndex].Translate(X, Y, Z);

    GLint Stride = sizeof(Scalar) * 16;

    glBindBuffer(GL_ARRAY_BUFFER, ModelMatrixBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, Stride * MemberIndex, Stride,
                            (const GLvoid*)&Members[MemberIndex]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return BGE_SUCCESS;
}


Result Crowd::Rotate(int MemberIndex, Quaternion Rotation)
{
    /* Prevent out-of-bounds transformations */
    if(MemberIndex < 0 || MemberIndex >= Capacity)
        return BGE_FAILURE;

    Members[MemberIndex] *= Rotation.ToMatrix();

    return BGE_SUCCESS;
}


Result Crowd::Scale(int MemberIndex, Scalar X, Scalar Y, Scalar Z)
{
    /* Prevent out-of-bounds transformations */
    if(MemberIndex < 0 || MemberIndex >= Capacity)
        return BGE_FAILURE;

    Members[MemberIndex].Scale(X, Y, Z);

    return BGE_SUCCESS;
}

} /* bakge */
