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

#ifndef BAKGE_UI_DRAGGABLE_H
#define BAKGE_UI_DRAGGABLE_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief A UI element that responds when the mouse is clicked and dragged.
 *
 * A Draggable can usually be moved on the screen, but can also be something
 * like a slider bar. A Draggable element is inherently Clickable.
 */
class BGE_API Draggable : public Clickable
{

public:

    /*! @brief Default Draggable constructor.
     *
     * Default Draggable constructor.
     */
    Draggable();

    /*! @brief Pure-virtual Draggable destructor.
     *
     * Pure-virtual Draggable destructor.
     */
    virtual ~Draggable() = 0;

    /*! @brief Drag callback.
     *
     * Drag callback.
     *
     * @param[in] X Distance dragged along the X axis.
     * @param[in] Y Distance dragged along the Y axis.
     */
    virtual void OnDrag(Motion X, Motion Y) = 0;

}; /* Draggable */

} /* bakge */

#endif /* BAKGE_UI_DRAGGABLE_H */
