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
 * @file Resizable.h
 * @brief Resizable interface declaration.
 */

#ifndef BAKGE_UI_RESIZEABLE_H
#define BAKGE_UI_RESIZEABLE_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief A UI element that is resizable, like a window.
 *
 * UI classes inheriting this will be considered resizable, and must
 * implement a callback for when they have been resized to a given 2D size.
 * A Resizable object is inherently Draggable.
 */
class BGE_API Resizable : public Draggable
{

public:

    /*! @brief Default Resizable constructor.
     *
     * Default Resizable constructor.
     */
    Resizable();

    /*! @brief Pure-virtual Resizable destructor.
     *
     * Pure-virtual Resizable destructor.
     */
    virtual ~Resizable() = 0;

    /*! @brief Called when the element is resized.
     *
     * Called when the element is resized. The GUI manager calls this
     * method when it detects an element has been resized. There is no
     * guarantee this is a result of a click-and-drag input from the
     * user. It could be due to the insertion of a new element which
     * requires another to shift its position, for example. The manager
     * calls SetDimensions to the Frame and/or SetPosition for the Anchor as
     * appropriate immediately prior to calling this function. As such, you
     * can retrieve the new position (scale is already provided) by calling
     * GetPosition on the Anchor.
     *
     * @param[in] X New X size of the element.
     * @param[in] Y New Y size of the element.
     */
    virtual void OnResize(Coord X, Coord Y) = 0;

}; /* Resizable */

} /* bakge */

#endif /* BAKGE_UI_RESIZEABLE_H */
