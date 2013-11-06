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

AnimationTexture::AnimationTexture()
{
}


AnimationTexture::~AnimationTexture()
{
}


AnimationTexture* AnimationTexture::Create(int NumFrames, int NumBones,
                                                        Matrix** Layout)
{
    // Each matrix is stored in 4 texels (3 for affine matrix, 1 for padding).
    // Each frame has NumBones matrices.
    int NumTexels = NumFrames * NumBones * 4;
    int NumPixels = NumTexels * 4;

    BeginLogBlock();
    Log("AnimationTexture::Create\n");
    Log("  - %d Keyframes\n", NumFrames);
    Log("  - %d Bones\n", NumBones);
    Log("  - %d Texels (%d pixels)\n", NumTexels, NumPixels);

    // Generate a GL texture
    GLuint TexName;
    glGenTextures(1, &TexName);

    GLuint Len = 1;
    while(Len * Len < NumTexels)
        Len <<= 1;

    Log("  - %dx%d texture size\n", Len, Len);

    glBindTexture(GL_TEXTURE_2D, TexName);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Len, Len, 0, GL_RGBA,
                                          GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    return NULL;
}

} // bakge
