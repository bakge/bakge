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
 * @file Graphics.h
 * @brief Internal graphics code and macro symbols not exposed as public API.
 */

#ifndef BAKGE_INTERNAL_GRAPHICS_H
#define BAKGE_INTERNAL_GRAPHICS_H

/*!
 * If mapping and filling data fails more than this number of times, consider
 * the entire buffer and data store allocation a failure, and return NULL
 * for the resource being allocated.
 */
#define BGE_MAP_BUFFER_MAX_ATTEMPTS 5

/*! @brief Simple struct to cache current GL state.
 *
 * This is used internally to avoid unnecessary calls to the expensive
 * glGet family of functions.
 */
typedef struct
{
    // Textures is an array of size NumTextureUnits. This is a dynamically
    // allocated buffer.
    GLint NumTextureUnits;
    GLuint* Textures;

    GLuint CurrentProgram;

} ShadowState;

#endif // BAKGE_INTERNAL_GRAPHICS_H
