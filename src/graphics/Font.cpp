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

Font::Font()
{
    NumGlyphs = 0;
    GlyphData = NULL;
    Glyphs = NULL;
    ScaleValue = 0;
}


Font::~Font()
{
    if(GlyphData != NULL)
        delete[] GlyphData;

    if(Glyphs != NULL)
        delete Glyphs;
}


Font* Font::Load(const char* FileName, Scalar FontHeight)
{
    // Verify the font file exists
    if(PHYSFS_exists(FileName) == 0) {
        printf("Unable to locate file %s\n", FileName);
        return NULL;
    }

    // Open the font file for reading
    PHYSFS_file* FontFile = PHYSFS_openRead(FileName);
    if(FontFile == NULL) {
        printf("Unable to load font file\n");
        return NULL;
    }

    // Determine size of data in the file
    PHYSFS_sint64 Size = PHYSFS_fileLength(FontFile);
    if(Size < 0) {
        printf("Unable to get file length\n");
        return NULL;
    }

    // Allocate buffer for the data
    unsigned char* Data = new unsigned char[Size];
    if(Data == NULL) {
        printf("Error allocating file data buffer\n");
        return NULL;
    }

    // Read font file data into buffer
    int BytesRead = PHYSFS_read(FontFile, (void*)Data, 1, Size);
    if(BytesRead < 0) {
        printf("Error reading file data\n");
        return NULL;
    }

    // Fill font info struct
    stbtt_fontinfo FontInfo;
    stbtt_InitFont(&FontInfo, Data, 0);

    // Allocate a new Font instance
    Font* F = new Font;
    if(F == NULL) {
        printf("Error allocating Font memory\n");
        return NULL;
    }

    // Fill data fields of Font instance
    F->NumGlyphs = FontInfo.numGlyphs;
    F->GlyphData = new stbtt_bakedchar[F->NumGlyphs];
    F->ScaleValue = stbtt_ScaleForPixelHeight(&FontInfo, FontHeight);

    // Glyphs are stored in 512x512 textures
    Byte* GlyphsAlpha = new Byte[512 * 512];

    int BakeResult = stbtt_BakeFontBitmap(Data, 0, FontHeight, GlyphsAlpha,
                                                            512, 512, 0,
                                                            F->NumGlyphs,
                                                            (F->GlyphData));
    if(BakeResult == 0) {
        printf("Unable to bake any characters to texture\n");
        delete F;
        return NULL;
    } else if(BakeResult > 0) {
        printf("Baked all %d glyphs to texture\n", F->NumGlyphs);
    } else {
        printf("Baked %d of %d glyphs to texture\n", -BakeResult,
                                                    F->NumGlyphs);
    }

    // Create glyphs alpha-only texture. Use a special shader to render text.
    F->Glyphs = bakge::Texture::Create(512, 512, GL_ALPHA, GL_UNSIGNED_BYTE,
                                                            GlyphsAlpha);
    if(F->Glyphs == NULL) {
        printf("Error creating font glyphs texture\n");
        delete F;
        return NULL;
    }

    // Cleanup
    PHYSFS_close(FontFile);
    delete[] GlyphsAlpha;
    delete[] Data;

    return F;
}

} /* bakge */
