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

#ifndef BAKGE_UI_CLICKABLE_H
#define BAKGE_UI_CLICKABLE_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief An element that responds to mouse clicks.
 *
 * A UI element that is clickable must implement a callback for when the
 * mouse is clicked over it. A Clickable object is inherently Hoverable.
 */
class BGE_API Clickable : public Hoverable
{

public:

    /*! @brief Default Clickable constructor.
     *
     * Default Clickable constructor.
     */
    Clickable();

    /*! @brief Pure-virtual Clickable destructor.
     *
     * Pure-virtual Clickable destructor.
     */
    virtual ~Clickable() = 0;

    /*! @brief Called when the element is clicked on.
     *
     * Called when the element is clicked on. This callback is called from the
     * GUI manager when it detects an element has been clicked.
     *
     * @param[in] X X-coordinate of the pointer at click occurrence.
     * @param[in] Y Y-coordinate of the pointer at click occurrence.
     */
    virtual void OnClick(Coord X, Coord Y) = 0;

    /*! @brief Called when the element is released.
     *
     * Called when the element is released. This callback is called from the
     * GUI manager when it detects an element has been released. The manager
     * guarantees that the object was clicked prior to the execution of this
     * callback.
     *
     * @param[in] X X-coordinate of the pointer at release occurrence.
     * @param[in] Y Y-coordinate of the pointer at release occurrence.
     */
    virtual void OnRelease(Coord X, Coord Y) = 0;

}; /* Clickable */

} /* bakge */

#endif /* BAKGE_UI_CLICKABLE_H */
