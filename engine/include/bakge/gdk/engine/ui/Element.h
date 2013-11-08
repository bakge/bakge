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

#ifndef BAKGE_GDK_ENGINE_UI_ELEMENT_H
#define BAKGE_GDK_ENGINE_UI_ELEMENT_H

#include <bakge/gdk/engine/Common.h>

namespace bakge
{
namespace gdk
{
namespace ui
{

class Element : public Hoverable, public Drawable
{

protected:

    Frame* Box;
    Anchor* Pin;
    Texture* Art;

    Element();
    Element(Frame* F, Anchor* A, Texture* T);


public:

    ~Element();

    BGE_INL const Frame* GetFrame() const
    {
        return Box;
    }

    BGE_INL const Anchor* GetAnchor() const
    {
        return Pin;
    }

    BGE_INL const Texture* GetTexture() const
    {
        return Art;
    }

    virtual Result Bind() const;

    virtual Result Unbind() const;

    virtual Result Draw() const;

    virtual bool IsHovered(Coord X, Coord Y) const;

}; // Element

} // ui
} // gdk
} // bakge

#endif // BAKGE_GDK_ENGINE_UI_ELEMENT_H
