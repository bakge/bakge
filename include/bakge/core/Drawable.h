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
 * @file Drawable.h
 * @brief Drawable interface declaration.
 */

#ifndef BAKGE_API_DRAWABLE_H
#define BAKGE_API_DRAWABLE_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief Drawables must be bound before they can be drawn.
 *
 * A Drawable object implements the method Draw. This interface is typically
 * used to pass objects to arbitrary Renderers so they can be drawn into the
 * world.
 */
class BGE_API Drawable : public Bindable
{

public:

    /*! @brief Default Drawable constructor.
     *
     * Default Drawable constructor.
     */
    Drawable();

    /*! @brief Pure-virtual Drawable destructor.
     *
     * Drawable is an abstract interface class. Only inheriting classes will
     * be instantiable.
     */
    virtual ~Drawable() = 0;

    /*! @brief Draw the object after it has been bound.
     *
     * Draws the object to the screen. If other framebuffers are set they are
     * instead drawn there.
     *
     * @return BGE_SUCCESS if the object was successfully drawn, or
     * BGE_FAILURE if any errors occurred.
     */
    virtual Result Draw() const = 0;

    virtual Result DrawInstanced(int Count) const = 0;

}; /* Drawable */

} /* bakge */

#endif /* BAKGE_API_DRAWABLE_H */
