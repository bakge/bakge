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
#include <bakge/internal/Graphics.h>
#ifdef _DEBUG
#include <bakge/internal/Debug.h>
#endif // _DEBUG

namespace bakge
{

Line::Line()
{
}


Line::~Line()
{
}


Line* Line::Create(Vector3 A, Vector3 B)
{
    static const GLuint Indices[] = {
        0, 1
    };

    Line* L = new Line;
    if(L == NULL) {
        Log("ERROR: Line - Couldn't allocate memory.\n");
        return NULL;
    }

#ifdef _DEBUG
    while(glGetError() != GL_NO_ERROR)
        ;
#endif // _DEBUG

    glGenBuffers(1, &L->Buffers[GEOMETRY_BUFFER_POSITIONS]);

#ifdef _DEBUG
    GLenum Error = glGetError();
    if(Error != GL_NO_ERROR) {
        Log("ERROR: Line - Unexpected error %s while creating points "
                                   "buffer.\n", _GetGLErrorName(Error));
        delete L;
        return NULL;
    }
#endif // _DEBUG

#ifdef _DEBUG
    while(glGetError() != GL_NO_ERROR)
        ;
#endif // _DEBUG

    glGenBuffers(1, &L->Buffers[GEOMETRY_BUFFER_INDICES]);

#ifdef _DEBUG
    Error = glGetError();
    if(Error != GL_NO_ERROR) {
        Log("ERROR: Line - Unexpected error %s while creating points "
                                   "buffer.\n", _GetGLErrorName(Error));
        delete L;
        return NULL;
    }
#endif // _DEBUG

    glBindBuffer(GL_ARRAY_BUFFER, L->Buffers[GEOMETRY_BUFFER_POSITIONS]);

    do {
        // If glUnmapBuffer fails, we'll need to reinitialize the data store
        glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 6, NULL,
                                                GL_DYNAMIC_DRAW);

        // Get a pointer for writing to the data store
        Scalar* BufferMap = (Scalar*)glMapBuffer(GL_ARRAY_BUFFER,
                                                    GL_WRITE_ONLY);

        int Tries = 0;

        if(BufferMap == NULL) {
            Log("ERROR: Line - Failed to map buffer (attempt %d).\n", ++Tries);

            // Sentinel to avoid infinite or long loops
            if(Tries >= BGE_MAP_BUFFER_MAX_ATTEMPTS) {
                Log("ERROR: Line - Couldn't map buffer after %d attempts.\n",
                                               BGE_MAP_BUFFER_MAX_ATTEMPTS);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                delete L;
                return NULL;
            }

            // Keep attempting to map and fill buffer
            continue;
        }

        BufferMap[0] = A[0];
        BufferMap[1] = A[1];
        BufferMap[2] = A[2];
        BufferMap[3] = B[0];
        BufferMap[4] = B[1];
        BufferMap[5] = B[2];

    } while(glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE);

    glBindBuffer(GL_ARRAY_BUFFER, L->Buffers[GEOMETRY_BUFFER_INDICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * 2, (GLvoid*)Indices,
                                                       GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    L->NumPoints = 2;
    L->A = A;
    L->B = B;

    return L;
}


Result Line::TranslateStart(Scalar X, Scalar Y, Scalar Z)
{
    A[0] += X;
    A[1] += Y;
    A[2] += Z;

    //TODO: Fill buffer

    return BGE_SUCCESS;
}


Result Line::TranslateEnd(Scalar X, Scalar Y, Scalar Z)
{
    B[0] += X;
    B[1] += Y;
    B[2] += Z;

    //TODO: Fill buffer

    return BGE_SUCCESS;
}


void Line::GetStartPosition(Scalar* X, Scalar* Y, Scalar* Z) const
{
    if(X != NULL)
        *X = A[0];

    if(Y != NULL)
        *Y = A[1];

    if(Z != NULL)
        *Z = A[2];
}


void Line::GetEndPosition(Scalar* X, Scalar* Y, Scalar* Z) const
{
    if(X != NULL)
        *X = B[0];

    if(Y != NULL)
        *Y = B[1];

    if(Z != NULL)
        *Z = B[2];
}

} /* bakge */
