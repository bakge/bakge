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
#include <bakge/internal/Error.h>
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
    LineStrip* L;

    try {
        L = (LineStrip*)calloc(1, sizeof(LineStrip));
        if(L == NULL)
            throw BGE_FACTORY_MESSAGE_BADALLOC;

        new(L) LineStrip;

        glBindBuffer(GL_ARRAY_BUFFER, L->GetBuffer(GEOMETRY_BUFFER_POSITIONS));
        glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 3 * NumPoints,
                                   (GLvoid*)Points, GL_DYNAMIC_DRAW);

#ifdef _DEBUG
        GLenum Error = glGetError();
        if(Error != GL_NO_ERROR) {
            Log("ERROR: LineStrip - Unexpected error %s while setting postions "
                                            "store.\n", _GetGLErrorName(Error));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            delete L;
            return NULL;
        }
#endif // _DEBUG

        glBindBuffer(GL_ARRAY_BUFFER, L->GetBuffer(GEOMETRY_BUFFER_INDICES));

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
    } catch(const char* Message) {
        Log("ERROR: LineStrip - %s\n", Message);
        free(L);
        return NULL;
    }
}


void LineStrip::GetPointAt(int Index, Scalar* X, Scalar* Y, Scalar* Z) const
{
    if(Index < 0 || Index >= NumPoints)
        return;

    //TODO: Copy position data into ptrs
}


void LineStrip::TranslatePointAt(int Index, Scalar X, Scalar Y, Scalar Z)
{
    if(Index < 0 || Index >= NumPoints)
        return;

    //TODO: Translate point, fill buffer
}

} /* bakge */
