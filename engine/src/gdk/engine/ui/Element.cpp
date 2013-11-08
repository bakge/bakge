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

#include <bakge/gdk/engine/Common.h>

namespace bakge
{
namespace gdk
{
namespace ui
{

Element::Element()
{
    Box = NULL;
    Pin = NULL;
    Art = NULL;
}


Element::Element(Frame* F, Anchor* A, Texture* T)
{
    Box = F;
    Pin = A;
    Art = T;
}


Element::~Element()
{
}


bool Element::IsHovered(Coord X, Coord Y) const
{
    Vector4 Pos = Pin->GetPosition();

    Box->GetDimensions(&Pos[2], &Pos[3]);

    if(X < Pos[0] || X > Pos[0] + Pos[2])
        return false;

    if(Y < Pos[1] || Y > Pos[1] + Pos[3])
        return false;

    return true;
}


Result Element::Bind() const
{
    Result Res = BGE_SUCCESS;

    if(Pin->Bind() == BGE_FAILURE)
        Res = BGE_FAILURE;

    if(Box->Bind() == BGE_FAILURE)
        Res = BGE_FAILURE;

    if(Art->Bind() == BGE_FAILURE)
        Res = BGE_FAILURE;

    return Res;
}


Result Element::Unbind() const
{
    Result Res = BGE_SUCCESS;

    if(Pin->Unbind() == BGE_FAILURE)
        Res = BGE_FAILURE;

    if(Box->Unbind() == BGE_FAILURE)
        Res = BGE_FAILURE;

    if(Art->Unbind() == BGE_FAILURE)
        Res = BGE_FAILURE;

    return Res;
}


Result Element::Draw() const
{
    return Box->Draw();
}

} // ui
} // gdk
} // bakge
