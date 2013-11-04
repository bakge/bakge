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

void _GLFWErrorHandler(int Code, const char* Description)
{
    Log("ERROR: GLFW Error (%x) - \"%s\"\n", Description, Code);
}


const char* _GetGLErrorName(GLenum Error)
{
#define BGE_GL_ERROR_CASE(ERR) case ERR: return #ERR;
    switch(Error) {
        BGE_GL_ERROR_CASE(GL_NO_ERROR)
        BGE_GL_ERROR_CASE(GL_INVALID_ENUM)
        BGE_GL_ERROR_CASE(GL_INVALID_VALUE)
        BGE_GL_ERROR_CASE(GL_INVALID_OPERATION)
#ifdef GL_INVALID_FRAMEBUFFER_OPERATION
        BGE_GL_ERROR_CASE(GL_INVALID_FRAMEBUFFER_OPERATION)
#endif // GL_INVALID_FRAMEBUFFER_OPERATION
        BGE_GL_ERROR_CASE(GL_OUT_OF_MEMORY)
        BGE_GL_ERROR_CASE(GL_STACK_UNDERFLOW)
        BGE_GL_ERROR_CASE(GL_STACK_OVERFLOW)
        default:
            return "Unknown error";
    }
}

} // bakge
