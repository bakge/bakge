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

BufferList::BufferList()
{
}


static BGE_INL GLuint* _AllocateBufferList(uint32 BGE_NCP Count)
{
    GLuint* Buffers = (GLuint*)calloc(Count, sizeof(GLuint));
    if(Buffers == NULL)
        return NULL;

    glGenBuffers(Count, Buffers);

    //TODO: Make sure all buffer names were allocated successfully

    return Buffers;
}


BufferList::BufferList(uint32 Count)
{
    try {
        Buffers = _AllocateBufferList(Count);
        if(Buffers == NULL)
            throw "Unable to allocate buffer list";

        NumBuffers = Count;
    } catch(...) {
        PseudoDestructor();
        throw;
    }
}


BufferList::~BufferList()
{
    PseudoDestructor();
}


void BufferList::PseudoDestructor()
{
    if(NumBuffers > 0) {
        glDeleteBuffers(NumBuffers, Buffers);
        free(Buffers);
        NumBuffers = 0;
    }
}


Result BufferList::SetNumBuffers(uint32 Count)
{
    if(Count == NumBuffers)
        return BGE_FAILURE;

    if(Count < NumBuffers) {
        // Delete buffer names that are going to be removed by the resize
        glDeleteBuffers(NumBuffers - Count, &Buffers[Count]);
        Buffers = (GLuint*)realloc(Buffers, Count * sizeof(uint32));
    } else {
        // Realloc buffer name list, generate names for new entries
        Buffers = (GLuint*)realloc(Buffers, Count * sizeof(uint32));
        glGenBuffers(Count - NumBuffers, &Buffers[NumBuffers]);
    }

    NumBuffers = Count;

    return BGE_SUCCESS;
}

} // bakge
