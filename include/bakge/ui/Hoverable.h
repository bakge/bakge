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
 * @file Hoverable.h
 * @brief Hoverable interface declaration.
 */

#ifndef BAKGE_UI_HOVERABLE_H
#define BAKGE_UI_HOVERABLE_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief An object that responds in some fashion when the mouse is over it.
 *
 * An object that responds in some fashion when the mouse is over it.
 * Typically interactive elements will implement Hoverable.
 */
class BGE_API Hoverable
{

public:

    /*! @brief Default Hoverable constructor.
     *
     * Default Hoverable constructor.
     */
    Hoverable();

    /*! @brief Pure-virtual Hoverable destructor.
     *
     * Pure-virtual Hoverable destructor.
     */
    virtual ~Hoverable() = 0;

    /*! @brief Check if the mouse is over a Hoverable element.
     *
     * Check if the mouse is over a Hoverable element.
     *
     * @param[in] X X-position of the mouse pointer.
     * @param[in] Y Y-position of the mouse pointer.
     *
     * @return true if the element is hovered over; false otherwise.
     */
    virtual bool IsHovered(Coord X, Coord Y) const = 0;

    /*! @brief Called by the GUI manager when the element is hovered
     * over.
     *
     * @param[in] X X-position of the mouse pointer at occurrence.
     * @param[in] Y Y-position of the mouse pointer at occurrence.
     *
     * Called by the GUI manager when the element is hovered over.
     */
    virtual void OnHover(Coord X, Coord Y) = 0;

}; /* Hoverable */

} /* bakge */

#endif /* BAKGE_UI_HOVERABLE_H */
