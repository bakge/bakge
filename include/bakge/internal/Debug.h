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
 * @file Debug.h
 * @brief Internal header with debugging macros and global functions.
 */

#ifndef BAKGE_INTERNAL_DEBUG_H
#define BAKGE_INTERNAL_DEBUG_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief GLFW error handler function.
 *
 * GLFW error handler function.
 *
 * @param[in] Code GLFW error code
 * @param[in] Description String description of error.
 */
void _GLFWErrorHandler(int Code, const char* Description);

/*! @brief Get a string representation of an OpenGL error flag.
 *
 * Get a string representation of an OpenGL error flag.
 *
 * @return String constant of the OpenGL error flag.
 */
const char* _GetGLErrorName(GLenum Error);

#if BGE_WARN_MISSING_VARIABLES
/*! @brief Log a warning that an attribute variable is missing.
 *
 * Log a warning that an attribute variable is missing from the currently
 * bound Shader. This should only be called if the variable is not found
 * while attempting to set its attribute pointer to be used for rendering,
 * e.g. when a Mesh is bound and the bge_Vertex attribute is not found in
 * the current Shader.
 *
 * @param[in] Name Name of the missing variable.
 */
#define BGE_WARN_MISSING_ATTRIBUTE(Name) \
    Log("WARNING: Unable to find attribute '%s' in current shader\n", Name);

/*! @brief Log a warning that a uniform variable is missing.
 *
 * Log a warning that an uniform variable is missing from the currently
 * bound Shader. This should only be called if the variable is not found
 * while attempting to set its value to be used for rendering,
 * e.g. when a Camera2D is bound and the bge_View uniform is not found in
 * the current Shader.
 *
 * @param[in] Name Name of the missing variable.
 */
#define BGE_WARN_MISSING_UNIFORM(Name) \
    Log("WARNING: Unable to find uniform '%s' in current shader\n", Name);
#else
#define BGE_WARN_MISSING_ATTRIBUTE(Name)
#define BGE_WARN_MISSING_UNIFORM(Name)
#endif // BGE_WARN_MISSING_VARIABLES

} // bakge

#endif // BAKGE_INTERNAL_DEBUG_H
