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

LineStrip::LineStrip()
{
}


LineStrip::~LineStrip()
{
}


LineStrip* LineStrip::Create(int NumPoints, Scalar* Points)
{
    LineStrip* L = new LineStrip;
    if(L == NULL) {
        Log("ERROR: LineStrip - Couldn't allocate memory.\n");
        return NULL;
    }

#ifdef _DEBUG
    while(glGetError() != GL_NO_ERROR)
        ;
#endif // _DEBUG

    glGenBuffers(1, &L->PointsBuffer);

#ifdef _DEBUG
    GLenum Error = glGetError();
    if(Error != GL_NO_ERROR) {
        Log("ERROR: LineStrip - Unexpected error %s while creating points "
                                       "buffer.\n", _GetGLErrorName(Error));
        delete L;
        return NULL;
    }

    while(glGetError() != GL_NO_ERROR)
        ;
#endif // _DEBUG

    glGenBuffers(1, &L->IndicesBuffer);

#ifdef _DEBUG
    Error = glGetError();
    if(Error != GL_NO_ERROR) {
        Log("ERROR: LineStrip - Unexpected error %s while creating indices "
                                       "buffer.\n", _GetGLErrorName(Error));
        delete L;
        return NULL;
    }
#endif // _DEBUG

#ifdef _DEBUG
    while(glGetError() != GL_NO_ERROR)
        ;
#endif // _DEBUG

    glBindBuffer(GL_ARRAY_BUFFER, L->PointsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 3 * NumPoints,
                               (GLvoid*)Points, GL_DYNAMIC_DRAW);

#ifdef _DEBUG
    Error = glGetError();
    if(Error != GL_NO_ERROR) {
        Log("ERROR: LineStrip - Unexpected error %s while setting postions "
                                        "store.\n", _GetGLErrorName(Error));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        delete L;
        return NULL;
    }
#endif // _DEBUG

    glBindBuffer(GL_ARRAY_BUFFER, L->IndicesBuffer);

    int Tries = 0;

    do {
        // Allocate storage here, in case unmapping fails
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * NumPoints, NULL,
                                                       GL_DYNAMIC_DRAW);

        // Map the indices store instead of making a temp buffer
        GLuint* IndicesMap = (GLuint*)glMapBuffer(GL_ARRAY_BUFFER,
                                                   GL_WRITE_ONLY);
        if(IndicesMap == NULL) {
            Log("ERROR: LineStrip - Failed to map buffer (attempt %d).\n",
                                                                   ++Tries);

            // Sentinel
            if(Tries >= BGE_MAP_BUFFER_MAX_ATTEMPTS) {
                Log("ERROR: LineStrip - Couldn't map buffer after %d "
                           "attempts.\n", BGE_MAP_BUFFER_MAX_ATTEMPTS);
                delete L;
                return NULL;
            }

            // Keep allocating storage and attempting to map and fill buffer
            continue;
        }

        // Fill indices data store
        for(int i=0;i<NumPoints;++i)
            IndicesMap[i] = i;

    } while(glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    L->NumPoints = NumPoints;

    return L;
}

} /* bakge */
