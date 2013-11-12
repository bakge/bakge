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
 * @file ICuboid.h
 * @brief Internal interface ICuboid declaration.
 */

#ifndef BAKGE_INTERFACE_ICUBOID_H
#define BAKGE_INTERFACE_ICUBOID_H

#include <bakge/interface/IRectangle.h>

namespace bakge
{

/*! @brief Any object that has a width, height and length (depth).
 *
 * Any object that has a width, height and length (depth). Provides some
 * mutator and accessor API that can be overloaded by inheriting classes.
 * There is some ambiguation when it comes to the definition of the word
 * cuboid; here it represents a convex polyhedron with six rectangular
 * faces whose edges meet in right angles. In layman's terms, it is a box
 * of arbitrary length, width and height.
 */
template<class T> class ICuboid : public IRectangle<T>
{

protected:

    T Length;

    ICuboid() : IRectangle(0, 0)
    {
        Length = 0;
    }

    ICuboid(T W, T H, T L) : IRectangle(W, H)
    {
        Length = L;
    }


public:

    virtual ~ICuboid() = 0
    {
    }

    BGE_INL virtual const T GetLength() const
    {
        return Length;
    }

    BGE_INL virtual void GetDimensions(T *W, T *H, T *L) const
    {
        IRectangle::GetDimension(W, H);

        if(L != NULL)
            *L = Length;
    }

    BGE_INL virtual Result SetDimensions(T W, T H, T L)
    {
        // Don't call overloads of SetDimensions(T, T)
        IRectangle<T>::SetDimensions(W, H);

        Length = L;

        return BGE_SUCCESS;
    }

    BGE_INL virtual const T SetLength(T L)
    {
        Length = L;

        return Length;
    }

}; // ICuboid

} // bakge

#endif // BAKGE_INTERNAL_INTERFACE_ICUBOID_H
