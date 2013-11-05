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

#ifndef BAKGE_GRAPHICS_TEXTURES_ANIMATIONTEXTURE_H
#define BAKGE_GRAPHICS_TEXTURES_ANIMATIONTEXTURE_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief A specialized texture that stores skeletal animation info into an
 * RGBA texture.
 *
 * A specialized texture that stores skeletal animation info into an RGBA
 * texture. This allows bone transformations to be applied to vertices in the
 * vertex shader, instead of streaming transformed vertices into the positions
 * buffer or another (possibly slower) technique.
 *
 * This is part of the GPU Gems 3 Ch. 2: Animated Crowd Rendering technique
 * implementation.
 */
class BGE_API AnimationTexture : public bakge::Texture
{
    AnimationTexture();


public:

    ~AnimationTexture();

}; // AnimationTexture

} // bakge

#endif // BAKGE_GRAPHICS_TEXTURES_ANIMATIONTEXTURE_H
