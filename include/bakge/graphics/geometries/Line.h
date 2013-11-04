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
 * @file Line.h
 * @brief Line class declaration.
 */

#ifndef BAKGE_GRAPHICS_GEOMETRY_LINE_H
#define BAKGE_GRAPHICS_GEOMETRY_LINE_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief A single line segment connecting two points in 3D Cartesian space.
 *
 * A single line segment connecting two points in 3D Cartesian space. Line
 * width can be modified using glLineWidth.
 */
class BGE_API Line : public Geometry
{
    Vector3 A;
    Vector3 B;

    /*! @brief Default Line constructor.
     *
     * Default Line constructor.
     */
    Line();


public:

    /*! @brief Line destructor.
     *
     * Line destructor.
     */
    ~Line();

    BGE_FACTORY Line* Create(Vector3 A, Vector3 B);

    Result TranslateStart(Scalar X, Scalar Y, Scalar Z);

    Result TranslateEnd(Scalar X, Scalar Y, Scalar Z);

    void GetStartPosition(Scalar* X, Scalar* Y, Scalar* Z) const;

    void GetEndPosition(Scalar* X, Scalar* Y, Scalar* Z) const;

}; /* Line */

} /* bakge */

#endif // BAKGE_GRAPHICS_GEOMETRY_LINE_H
