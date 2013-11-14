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

CompositeTexture::CompositeTexture()
{
    Corner = 0;
}


CompositeTexture::~CompositeTexture()
{
}


CompositeTexture* CompositeTexture::Create(int W, int H, int C,
                                const void* BL, const void* TL,
                                const void* TR, const void* BR,
                                const void* HG, const void* VG,
                                                const void* F)
{
    BeginLogBlock();
    Log("CompositeTexture::Create %dx%d\n", W, H);

    if(W < C * 2 || H < C * 2) {
        Log("  ERROR: Invalid dimensions based on corner size.\n");
        EndLogBlock();
        return NULL;
    }

    CompositeTexture* Tex = (CompositeTexture*)calloc(1,
                                sizeof(CompositeTexture));
    if(Tex == NULL) {
        Log("  ERROR: Couldn't allocate memory.\n");
        EndLogBlock();
        return NULL;
    }

    new(Tex) CompositeTexture;

    // Working memory. We'll fill this with appropriate data before creating
    // the texture
    Byte* Bitmap = (Byte*)malloc(W * H * 4);
    if(Bitmap == NULL) {
        Log("  ERROR: Couldn't allocate working memory.\n");
        EndLogBlock();
        delete Tex;
        return NULL;
    }

    // Cache these values to prevent a bunch of repeated ops
    int C4 = C * 4; // Length of corner segment (horizontal & vertical)
    int CM1 = C - 1; // Offset to end of corner row/column
    int W4 = W * 4; // Horizontal length of bitmap
    int H4 = H * 4; // Vertical length bitmap
    int IW = W - 2 * C; // Width of fill segment
    int IH = H - 2 * C; // Height of fill segment
    int i, j;

    Byte* Offset = Bitmap;

    // Copy bottom-left corner
    for(i=0;i<C;++i)
        memcpy(Offset+(i*W4), (Byte*)BL+((CM1-i)*C4), C4);

    // Copy bottom-right corner
    Offset = Bitmap+((W-C)*4);
    for(i=0;i<C;++i)
        memcpy(Offset+(i*W4), (Byte*)BR+((CM1-i)*C4), C4);

    // Copy top-left corner
    Offset = Bitmap+(W4*(H-C));
    for(i=0;i<C;++i)
        memcpy(Offset+(i*W4), (Byte*)TL+((CM1-i)*C4), C4);

    // Copy top-right corner
    Offset = Bitmap+(W4*(H-C))+((W-C)*4);
    for(i=0;i<C;++i)
        memcpy(Offset+(i*W4), (Byte*)TR+((CM1-i)*C4), C4);

    // Copy vertical gradients
    Offset = Bitmap+C4;
    Byte* Offset2 = Bitmap+(W4*(H-C))+C4;
    for(i=0;i<C;++i) {
        for(j=0;j<IW;++j) {
            memcpy(Offset+(i*W4)+(j*4), (Byte*)VG+(i*4), 4);
            memcpy(Offset2+(i*W4)+(j*4), (Byte*)VG+((CM1-i)*4), 4);
        }
    }

    Offset = Bitmap+(W*C4);
    for(i=0;i<IH;++i) {
        memcpy(Offset+(i*W4), HG, C4);
    }

    // Copy right-edge in reverse
    Offset = Bitmap+(W*C4)+((W-C)*4);
    for(i=0;i<C;++i) {
        memcpy(Offset+(i*4), (Byte*)HG+((CM1-i)*4), 4);
    }

    // Copy through-out the rest of the edge
    for(i=0;i<IH;++i) {
        memcpy(Offset+(i*W4), Offset, C4);
    }

    // Copy in fill
    Offset = Bitmap+(C4*W)+(C4);
    for(i=0;i<IH;++i) {
        for(j=0;j<IW;++j) {
            memcpy(Offset+(i*W4)+(j*4), F, 4);
        }
    }

    glGenTextures(1, &Tex->TextureID);

    Tex->Width = W;
    Tex->Height = H;
    Tex->Corner = C;

    glBindTexture(GL_TEXTURE_2D, Tex->TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA,
                                    GL_UNSIGNED_BYTE, Bitmap);
    glBindTexture(GL_TEXTURE_2D, 0);

    EndLogBlock();

    delete Bitmap;

    return Tex;
}

} // bakge
