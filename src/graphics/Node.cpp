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

Node::Node()
{
}


Node::~Node()
{
}


Result Node::Bind() const
{
    GLint Program, Location;

    /* Retrieve current shader program */
    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0)
        return BGE_FAILURE;

    /* Retrieve location of the bge_Translation vec4 */
    Location = glGetUniformLocation(Program, BGE_MODEL_UNIFORM);
    if(Location < 0) {
#ifdef _DEBUG
        BGE_WARN_MISSING_UNIFORM(BGE_MODEL_UNIFORM);
#endif // _DEBUG
        return BGE_FAILURE;
    }

    Matrix Translation = Matrix::Translation(Position[0], Position[1],
                                                        Position[2]);

    glUniformMatrix4fv(Location, 1, GL_FALSE, &Translation[0]);

    return BGE_SUCCESS;
}


Result Node::Unbind() const
{
    GLint Program, Location;

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Retrieve current shader program */
    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0)
        return BGE_FAILURE;

    Location = glGetUniformLocation(Program, BGE_MODEL_UNIFORM);
    if(Location < 0) {
#ifdef _DEBUG
        BGE_WARN_MISSING_UNIFORM(BGE_MODEL_UNIFORM);
#endif // _DEBUG
        return BGE_FAILURE;
    }

    glUniformMatrix4fv(Location, 1, GL_FALSE, &Matrix::Identity[0]);

    return BGE_SUCCESS;
}


Vector4 BGE_NCP Node::SetPosition(Scalar X, Scalar Y, Scalar Z)
{
    Position[0] = X;
    Position[1] = Y;
    Position[2] = Z;

    return Position;
}


Vector4 BGE_NCP Node::GetPosition() const
{
    return Position;
}


Node* Node::Create(Scalar X, Scalar Y, Scalar Z)
{
    Node* N = (Node*)calloc(1, sizeof(Node));
    if(N == NULL) {
        Log("ERROR: Node - Couldn't allocate memory\n");
        return NULL;
    }

    new(N) Node;

    N->SetPosition(X, Y, Z);

    return N;
}


Vector4 BGE_NCP Node::Translate(Scalar X, Scalar Y, Scalar Z)
{
    Position[0] += X;
    Position[1] += Y;
    Position[2] += Z;

    return Position;
}

} /* bakge */
