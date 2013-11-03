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

#ifndef BAKGE_GRAPHICS_VIEWPORT_H
#define BAKGE_GRAPHICS_VIEWPORT_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief A rectangular area in window/screen coordinates to which OpenGL
 * renders to.
 *
 * A rectangular area in window/screen coordinates to which OpenGL renders to.
 * Can be used to implement multiple viewports. This is one of the few Bakge
 * classes that can be stack-allocated through trivial constructors.
 */
class BGE_API Viewport : public Bindable
{
    union
    {
        uint16 Dimensions[4];
        struct
        {
            uint16 X0;
            uint16 Y0;
            uint16 X1;
            uint16 Y1;
        };
    };


public:

    Viewport();

    Viewport(uint16 X0, uint16 Y0, uint16 X1, uint16 Y1);

    ~Viewport();

    Result Bind() const;

    /*! @brief A no-op. No need to call.
     *
     * A no-op. There is no need to unbind a viewport; its dimensions can only
     * be affected by calls to Viewport methods.
     */
    Result Unbind() const
    {
        return BGE_FAILURE;
    }

}; // Viewport

} // bakge

#endif // BAKGE_GRAPHICS_VIEWPORT_H
