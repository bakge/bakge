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
    NumMembers = 0;
    CrowdBuffer = 0;
}


Crowd::~Crowd()
{
    if(CrowdBuffer != 0)
        glDeleteBuffers(1, &CrowdBuffer);
}


Crowd* Crowd::Create(int ReserveMembers)
{
    Crowd* C = new Crowd;

    glGenBuffers(1, &(C->CrowdBuffer));

#ifdef _DEBUG
    if(C->CrowdBuffer == 0) {
        printf("Error allocating crowd vertex buffer\n");
        delete C;
        return NULL;
    }
#endif

    C->NumMembers = ReserveMembers;

    /* *
     * Generate `ReserveMembers` instances' model matrices
     * Will use glBufferSubData in the future, to avoid generating an entire
     * Matrix array
     * */
    Matrix* Transformations = new Matrix[ReserveMembers];
    glBindBuffer(GL_ARRAY_BUFFER, C->CrowdBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 16 * ReserveMembers,
                                    Transformations, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    delete[] Transformations;

    return C;
}


Result Crowd::Bind() const
{
    return BGE_FAILURE;
}


Result Crowd::Unbind() const
{
    return BGE_FAILURE;
}

} /* bakge */
