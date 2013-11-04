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

#ifndef BAKGE_GDK_ENGINE_UI_COMPOSITETEXTURE_H
#define BAKGE_GDK_ENGINE_UI_COMPOSITETEXTURE_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief A special texture whose pixel data is built using four corner
 * textures, two gradients and a fill color. Best used for static UI elements
 * that share the same texture, like buttons or window bars.
 *
 * A special texture whose pixel data is built using 4 corner textures and
 * two gradients. The two gradients are used to fill the spaces directly
 * between the corner pieces (horizontally and vertically), and the fill
 * color fills the empty space left in the middle of the texture. There are
 * restrictions on the dimensions of these textures. The corner pieces must
 * be identical in scale, with their width equal to their height. The
 * horizontal and vertical gradients must be identical in scale with their
 * length equal to the width (and height) of the corner pieces and their
 * height equal to 1. The fill color must a single pixel (width 1, height 1).
 *
 * These textures area great for UI, so that when they are used to draw
 * static elements of different sizes, the texture is clear and crisp since
 * each texture is dynamically filled to be the same pixel size as the element
 * appears on-screen.
 *
 * For dynamic elements that change sizes often, it is better to instead split
 * the frame and texture's pieces (corners, gradients and fill) into 7
 * unique frames or textures, and bind and draw each of those individually
 * according to the proper scale, using the GL_NEAREST magnification filters
 * to maintain a clear, crisp texture on the element. This approach is a bit
 * more complicated since it requires managing the scales and positions of
 * not only the UI element as a whole, but each of its 7 "subframes". This
 * does provide some benefits over using a CompositeTexture in that you load
 * far less texture data into OpenGL and you can easily "re-skin" the element.
 */
class BGE_API CompositeTexture : public bakge::Texture
{
    int Corner;

    CompositeTexture();


public:

    ~CompositeTexture();

    /*! @brief Create a CompositeTexture with the given subtextures.
     *
     * Create a CompositeTexture with the given subtextures. The data must be
     * given in RGBA format.
     *
     * @param[in] W Width of the entire texture.
     * @param[in] H Height of the entire texture.
     * @param[in] C Corner width & height, gradient width.
     * @param[in] BL Bottom-left corner RGBA texture data.
     * @param[in] TL Top-left corner RGBA texture data.
     * @param[in] TR Top-right corner RGBA texture data.
     * @param[in] BR Bottom-right corner RGBA texture data.
     * @param[in] HG Horizontal gradient RGBA texture data.
     * @param[in] VG Vertical gradient RGBA texture data.
     * @param[in] F Fill RGBA texture data (one pixel).
     *
     * @return Pointer to allocated CompositeTexture; NULL if any errors
     * occurred.
     */
    BGE_FACTORY CompositeTexture* Create(int W, int H, int C, const void* BL,
                                            const void* TL, const void* TR,
                                            const void* BR, const void* HG,
                                            const void* VG, const void* F);

    /*! @brief Get the height & width of the corner texture.
     *
     * Get the height & width of the corner texture.
     *
     * @return Width (same as height) of the corner texture.
     */
    BGE_INL int GetCornerDimension() const
    {
        return Corner;
    }

}; // CompositeTexture

} // bakge

#endif // BAKGE_GDK_ENGINE_UI_COMPOSITETEXTURE_H
