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
 * @file GlyphLibrary.h
 * @brief GlyphLibrary class declaration.
 */

#ifndef BAKGE_GRAPHICS_TEXT_GLYPHLIBRARY_H
#define BAKGE_GRAPHICS_TEXT_GLYPHLIBRARY_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief Archives glyph maps and glyph data.
 *
 * Archives glyph maps and glyph data. It is recommended you keep separate
 * libraries for different fonts. This class manages a series of glyph maps
 * baked from a font, making it easier to check if a glyph is loaded, and
 * to extract that glyph's data and metadata for use in rendering it to
 * screen.
 *
 * GlyphMaps archived into a GlyphLibrary need not be freed, as the library
 * will free them in its destructor.
 */
class BGE_API GlyphLibrary
{
    int NumRecords;


public:

    /*! @brief Default GlyphLibrary constructor.
     *
     * Default GlyphLibrary constructor.
     */
    GlyphLibrary();

    /*! @brief GlyphLibrary destructor.
     *
     * GlyphLibrary destructor.
     */
    ~GlyphLibrary();

}; // GlyphLibrary

} // bakge

#endif // BAKGE_GRAPHICS_TEXT_GLYPHLIBRARY_H
