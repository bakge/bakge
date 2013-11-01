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

#include <bakge/Bakge.h>

namespace bakge
{

GlyphMap::GlyphMap()
{
    Tex = NULL;
    Data = NULL;
    Start = 0;
    End = 0;
    ScaleFactor = 0;
}


GlyphMap::~GlyphMap()
{
}


Result GlyphMap::Extract(int Codepoint, Stamp* Target)
{
    if(Codepoint < Start || Codepoint > End) {
        return BGE_FAILURE;
    }

    int i = Codepoint - Start;
    int W = Tex->GetWidth();
    int H = Tex->GetHeight();

    Target->SetOffsets(Data[i].xoff, Data[i].yoff);
    Target->SetAdvance(Data[i].xadvance);
    Target->SetScaleFactor(ScaleFactor);
    Target->SetDimensions(Data[i].x1 - Data[i].x0, Data[i].y1 - Data[i].y0);
    Target->SetTexCoords((Scalar)Data[i].x0 / W, (Scalar)Data[i].y0 / H,
                            (Scalar)Data[i].x1 / W, (Scalar)Data[i].y1 / H);

    return BGE_SUCCESS;
}

} // bakge
