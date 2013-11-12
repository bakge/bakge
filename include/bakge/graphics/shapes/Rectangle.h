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
 * @file Rectangle.h
 * @brief Rectangle class declaration.
 */

#ifndef BAKGE_GRAPHICS_SHAPES_RECTANGLE_H
#define BAKGE_GRAPHICS_SHAPES_RECTANGLE_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief A shape primarily used for 2D rendering.
 *
 * The Rectangle class represents a rectangular shape drawn in the XY plane
 * in 3D Cartesian space. While it is primarily intended to be used for
 * drawing 2D scenes, it could technically be used in a 3D setting. The
 * Rectangle's normals point along the +Z axis.
 */
class BGE_API Rectangle : public Shape, public IRectangle<Scalar>
{

protected:

    Scalar Width;
    Scalar Height;

    /*! @brief Default Rectangle constructor.
     *
     * Default Rectangle constructor.
     */
	Rectangle();


public:

    /*! @brief Rectangle destructor.
     *
     * Rectangle destructor.
     */
    ~Rectangle();

    /*! @brief Create a Rectangle centered at the origin.
     *
     * Create a Rectangle centered at the origin.
     *
     * @return Pointer to allocated Rectangle; NULL if any errors occurred.
     */
    BGE_FACTORY Rectangle* Create(Scalar Width, Scalar Height);

    virtual void SetDimensions(Scalar Width, Scalar Height);

}; /* Rectangle */

} /* bakge */

#endif /* BAKGE_GRAPHICS_SHAPES_RECTANGLE_H */
