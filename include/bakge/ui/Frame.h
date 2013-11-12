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
 * @file Frame.h
 * @brief Frame class declaration.
 */

#ifndef BAKGE_UI_FRAME_H
#define BAKGE_UI_FRAME_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief A Rectangle specifically suited for UI elements.
 *
 * A Rectangle specifically suited for UI elements. The origin of the shape
 * is at the bottom-left corner of the Frame.
 */
class BGE_API Frame : public Rectangle
{

public:

    /*! @brief Default Frame constructor.
     *
     * Default Frame constructor.
     */
    Frame();

    /*! @brief Frame destructor.
     *
     * Frame destructor.
     */
    virtual ~Frame();

    /*! @brief Create a new frame with a given width and height.
     *
     * Create a new frame with a given width and height.
     *
     * @param[in] Width Width of the Frame.
     * @param[in] Height Height of the Frame.
     *
     * @return Pointer to allocated Frame; NULL if any errors occurred.
     */
    BGE_FACTORY Frame* Create(Scalar Width, Scalar Height);

    /*! @brief Set the dimensions of a Frame.
     *
     * Set the dimensions of a Frame.
     *
     * @param[in] Width New width of the Frame.
     * @param[in] Height New height of the Frame.
     */
    virtual void SetDimensions(Scalar Width, Scalar Height);

}; /* Frame */

} /* bakge */

#endif /* BAKGE_UI_FRAME_H */
