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
 * @file IRectangle.h
 * @brief Internal interface IRectangle declaration.
 */

#ifndef BAKGE_INTERFACE_IRECTANGLE_H
#define BAKGE_INTERFACE_IRECTANGLE_H

namespace bakge
{

/*! @brief Any object that has a width and height.
 *
 * Any object that has a width and height. Provides some mutator and accessor
 * API that can be overloaded by inheriting classes.
 */
class IRectangle
{

protected:

    int Width;
    int Height;

    IRectangle()
    {
        Width = 0;
        Height = 0;
    }

    IRectangle(int W, int H)
    {
        Width = W;
        Height = H;
    }

    virtual ~IRectangle() = 0
    {
    }

    BGE_INL virtual const int GetWidth() const
    {
        return Width;
    }

    BGE_INL virtual const int GetHeight() const
    {
        return Height;
    }

    BGE_INL virtual void GetDimensions(int *W, int *H) const
    {
        if(W != NULL)
            *W = Width;

        if(H != NULL)
            *H = Height;
    }

    BGE_INL virtual void SetDimensions(int W, int H)
    {
        Width = W;
        Height = H;
    }

    BGE_INL virtual const int SetWidth(int W)
    {
        Width = W;

        return Width;
    }

    BGE_INL virtual const int SetHeight(int H)
    {
        Height = H;

        return Height;
    }

}; // IRectangle

} // bakge

#endif // BAKGE_INTERFACE_IRECTANGLE_H
