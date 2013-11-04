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

// Cache these values during initialization (only occurs once) and make them
// available internally through simple getter API.

static int32 _ViewportMaxDimensions[2];
static int _MaxTextureUnits;
static int _MaxTextureSize;
static int _RecMaxIndices;
static int _RecMaxVertices;

Result _GraphicsInit(int argc, char* argv[])
{
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, _ViewportMaxDimensions);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_MaxTextureUnits);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_MaxTextureSize);
    glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &_RecMaxIndices);
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &_RecMaxVertices);

    return BGE_SUCCESS;
}

int _GetMaxViewportWidth()
{
    return _ViewportMaxDimensions[0];
}

int _GetMaxViewportHeight()
{
    return _ViewportMaxDimensions[1];
}

int _GetMaxTextureUnits()
{
    return _MaxTextureUnits;
}

int _GetMaxTextureSize()
{
    return _MaxTextureSize;
}

int _GetRecMaxIndices()
{
    return _RecMaxIndices;
}

int _GetRecMaxVertices()
{
    return _RecMaxVertices;
}

} // bakge
