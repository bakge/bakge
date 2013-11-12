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
 * @file Stamp.h
 * @brief Stamp class declaration.
 */

#ifndef BAKGE_GRAPHICS_TEXT_STAMP_H
#define BAKGE_GRAPHICS_TEXT_STAMP_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief A Rectangle specifically used for rending text glyphs.
 *
 * The dimensions of the quad (two triangles) drawn on screen are
 * changed to match the properties of the currently picked glyph.
 */
class BGE_API Stamp : public Rectangle
{
    struct {
        Scalar X;
        Scalar Y;
    } Start, Current, Offset;

    Scalar Adv;
    Scalar ScaleFactor;
    Scalar LineAdv;


protected:

    /*! @brief Default Stamp constructor.
     *
     * Default Stamp constructor.
     */
    Stamp();


public:

    /*! @brief Stamp destructor.
     *
     * Stamp destructor.
     */
    ~Stamp();

    BGE_FACTORY Stamp* Create();

    /*! @brief Set the vertex positions of the Stamp based on the currently
     * loaded glyph and the current position of the stamp.
     *
     * Set the vertex positions of the Stamp based on the currently
     * loaded glyph and the current position of the stamp. Called internally
     * by GlyphMap::Extract. It is recommended you do not call this function.
     *
     * @param[in] Width Width of the Stamp quad.
     * @param[in] Height Height of the Stamp quad.
     *
     * @return BGE_SUCCESS if the Stamp's vertex positions were successfully
     * set; BGE_FAILURE if any errors occurred.
     */
    virtual void SetDimensions(Scalar Width, Scalar Height);

    /*! @brief Set the texture coordinates of the Stamp quad.
     *
     * Set the texture coordinates of the stamp quad. This is called
     * internally by GlyphMap::Extract. It is recommended you do not call
     * this function.
     *
     * @return BGE_SUCCESS if the texture coordinates of the stamp's vertices
     * were successfully set; BGE_FAILURE if any errors occurred.
     */
    Result SetTexCoords(Scalar S0, Scalar T0, Scalar S1, Scalar T1);

    virtual Result Bind() const;

    /*! @brief Advance the current x-position by the current glyph's
     * x-advance.
     *
     * Advance the current x-position by the current glyph's
     * x-advance.
     *
     * @return BGE_SUCCESS if the stamp's x-position was successfully
     * advanced; BGE_FAILURE if any errors occurred.
     */
    Result Advance();

    /*! @brief Set the Stamp start position and reset current position.
     *
     * Set the Stamp start position and reset current position.
     *
     * @param[in] X New X-position of the Stamp.
     * @param[in] Y New Y-position of the Stamp.
     *
     * @return BGE_SUCCESS if the Stamp position was successfully set;
     * BGE_FAILURE otherwise.
     */
    Result Begin(Scalar X, Scalar Y);

    /*! @brief Advance x-position, for kerning purposes.
     *
     * Advances the x-position, for kerning purposes. One can also use it
     * to add a custom amount of space between characters.
     *
     * @brief Amount Height-scaled kerning advance.
     *
     * @return BGE_SUCCESS if kerning was successful; BGE_FAILURE if any
     * errors occurred.
     */
    Result Kern(Scalar Amount);

    /*! @brief Set the glyph X-advance value. Used by GlyphMap::Extract.
     *
     * Set the glyph X-advance value. Used by GlyphMap::Extract. You likely
     * won't ever need to use this, but it is available if you want to
     * stylize your text rendering.
     *
     * @return X-position advance, after being set.
     */
    BGE_INL Scalar SetAdvance(Scalar A)
    {
        Adv = A;

        return A;
    }

    BGE_INL Scalar SetLineAdvance(Scalar A)
    {
        LineAdv = A;

        return A;
    }

    /*! @brief Get the glyph X-advance value.
     *
     * You can use this in tandem with SetAdvance to stylize your text
     * rendering.
     *
     * @return X-position advance of the currently picked glyph.
     */
    BGE_INL Scalar GetAdvance() const
    {
        return Adv;
    }

    BGE_INL Scalar GetLineAdvance() const
    {
        return LineAdv;
    }

    /*! @brief Set the glyph origin offsets, relative to the rendering
     * baseline.
     *
     * Set the glyph origin offsets, relative to the rendering baseline. Used
     * internally by GlyphMap::Extract. You can use this to stylize your
     * text rendering.
     *
     * @param[in] X X-offset of the glyph, from the baseline.
     * @param[in] Y Y-offset of the glyph, from the baseline.
     */
    BGE_INL void SetOffsets(Scalar X, Scalar Y)
    {
        Offset.X = X;
        Offset.Y = Y;
    }

    /*! @brief Get the current glyph origin offsets, relative to the rendering
    * baseline.
    *
    * Get the current glyph origin offsets, relative to the rendering
    * baseline. You can use this in tandem with SetOffsets to stylize your
    * text rendering.
    *
    * @param[out] X Pointee set to the current glyph's X-offset, relative to
    *               the baseline.
    * @param[out] Y Pointee set to the current glyph's Y-offset, relative to
    *               the baseline.
    */
    BGE_INL void GetOffsets(Scalar* X , Scalar* Y)
    {
        *X = Offset.X;
        *Y = Offset.Y;
    }

    /*! @brief Set the scale factor for the glyph.
     *
     * Set the scale factor for the glyph. Used internally by
     * GlyphMap::Extract. You can use this to stylize your text rendering.
     *
     * @param[in] F Scale factor to set to.
     *
     * @return Scale factor of the current glyph, after assignment.
     */
    BGE_INL Scalar SetScaleFactor(Scalar F)
    {
        ScaleFactor = F;

        return F;
    }

    /*! @brief Get the scale factor for the glyph.
     *
     * Get the scale factor for the glyph. You can use this in tandem with
     * SetScaleFactor to stylize your text rendering.
     *
     * @return Scale factor of the current glyph.
     */
    BGE_INL Scalar GetScaleFactor() const
    {
        return ScaleFactor;
    }

    Result EndLine();

}; // Stamp

} // bakge

#endif // BAKGE_GRAPHICS_TEXT_STAMP_H
