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
 * @file GlyphMap.h
 * @brief GlyphMap class declaration.
 */

#ifndef BAKGE_GRAPHICS_TEXT_GLYPHMAP_H
#define BAKGE_GRAPHICS_TEXT_GLYPHMAP_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief Consists of a texture of baked glyphs and their metadata.
 *
 * Consists of a texture of baked glyphs and their metadata. When you bake a
 * GlyphMap from a Font, you should archive it into a GlyphLibrary to make it
 * easier to manage and keep track of which glyphs are available for rendering
 * and which ones you'll have to bake first.
 *
 * The GlyphMap caches various metadata about the glyphs, such as pixel-height
 * scale factor, the range of glyph indices baked and some raw glyph data
 * extracted by stb_truetype.
 */
class BGE_API GlyphMap : public Bindable
{
    friend int Font::Bake(GlyphMap**, int, int, int);

    Texture* Tex;

    Scalar ScaleFactor;

    stbtt_bakedchar* Data;

    // NumGlyphs is End - Start
    uint32 Start;
    uint32 End;


public:

    /*! @brief Default GlyphMap constructor.
     *
     * Default GlyphMap constructor.
     */
    GlyphMap();

    /*! @brief GlyphMap destructor.
     *
     * GlyphMap destructor.
     */
    ~GlyphMap();

    BGE_INL Result Bind() const
    {
        return Tex->Bind();
    }

    BGE_INL Result Unbind() const
    {
        return Tex->Unbind();
    }

    BGE_INL uint32 GetNumGlyphs() const
    {
        if(End < Start)
            return 0;

        return End - Start;
    }

    BGE_INL uint32 GetStartGlyph() const
    {
        return Start;
    }

    BGE_INL uint32 GetEndGlyph() const
    {
        return End;
    }

    BGE_INL Scalar GetScaleFactor() const
    {
        return ScaleFactor;
    }

    /*! @brief Fill the target Glyph struct with data about the given
     * codepoint.
     *
     * Fill the target Glyph struct with data about the given codepoint.
     * If the codepoint is not part of this GlyphMap, Target->Codepoint is
     * set to 0 and the function returns BGE_FAILURE.
     *
     * @param[in] Codepoint Unicode codepoint to extract data about.
     * @param[out] Target Filled with codepoint data if found.
     *
     * @return BGE_SUCCESS if the codepoint was found and data was
     * successfully filled into Target; BGE_FAILURE if any errors occurred or
     * the codepoint was not found.
     */
    Result Extract(uint32 Codepoint, Stamp* Target);

}; // GlyphMap

} // bakge

#endif // BAKGE_GRAPHICS_TEXT_GLYPHMAP_H
