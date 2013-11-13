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

/*!
 * @file BufferList.h
 * @brief BufferList class declaration.
 */

#ifndef BAKGE_GRAPHICS_BUFFERLIST_H
#define BAKGE_GRAPHICS_BUFFERLIST_H

#include <bakge/core/Type.h>
#include <GL/glew.h>

namespace bakge
{

/*! @brief Any object that has associated OpenGL vertex buffers.
 *
 * Any object that has associated OpenGL vertex buffers. This object is only
 * instantiable by deriving a new class from it and using a factory function.
 */
class BGE_API BufferList
{
    uint32 NumBuffers;

    BufferList();

    virtual void PseudoDestructor();


protected:

    GLuint* Buffers;

    // Creates Count buffer names
    BufferList(uint32 Count);

    BGE_INL uint32 GetNumBuffers() const
    {
        return NumBuffers;
    }

    Result SetNumBuffers(uint32 Count);


public:

    virtual ~BufferList();

}; // BufferList

} // bakge

#endif // BAKGE_GRAPHICS_BUFFERLIST_H
