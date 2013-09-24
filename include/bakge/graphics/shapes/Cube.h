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

#ifndef BAKGE_GRAPHICS_SHAPES_CUBE_H
#define BAKGE_GRAPHICS_SHAPES_CUBE_H

#include <bakge/Bakge.h>

namespace bakge
{

class BGE_API Cube : public Shape
{

protected:

    Cube();


public:

    ~Cube();

    BGE_FACTORY Cube* Create(Scalar Length, Scalar Width, Scalar Height);

    /*! @brief Get the Cube's dimensions.
     *
     * @param[out] Width Size along the X axis.
     * @param[out] Height Size along the Y axis.
     * @param[out] Length Size along the Z axis.
     *
     * @return Volume of the Cube.
     */
    Scalar GetDimensions(Scalar* Width, Scalar* Height, Scalar* Length) const;

}; /* Cube */

} /* bakge */

#endif /* BAKGE_GRAPHICS_SHAPES_CUBE_H */
