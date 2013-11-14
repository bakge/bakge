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
    Ascent = 0;
    Descent = 0;
    Gap = 0;
}


Font::~Font()
{
    if(FontInfo.data != NULL)
        delete[] FontInfo.data;
}


Font* Font::Load(const char* FileName)
{
    // Verify the font file exists
    if(PHYSFS_exists(FileName) == 0) {
        Log("ERROR: Font - Unable to locate file %s\n", FileName);
        return NULL;
    }

    // Open the font file for reading
    PHYSFS_file* FontFile = PHYSFS_openRead(FileName);
    if(FontFile == NULL) {
        Log("ERROR: Font - Unable to load file\n");
        return NULL;
    }

    // Determine size of data in the file
    PHYSFS_sint64 Size = PHYSFS_fileLength(FontFile);
    if(Size < 0) {
        Log("ERROR: Font - Unable to get file length\n");
        return NULL;
    }

    // Allocate buffer for the data
    unsigned char* Data = (unsigned char*)malloc(Size);
    if(Data == NULL) {
        Log("ERROR: Font - Error allocating file data buffer\n");
        return NULL;
    }

    // Read font file data into buffer
    PHYSFS_sint64 BytesRead = PHYSFS_read(FontFile, (void*)Data, 1, (PHYSFS_uint32)Size);
    if(BytesRead < 0) {
        Log("ERROR: Font - Error reading file data\n");
        return NULL;
    }

    // Allocate a new Font instance
    Font* F = (Font*)calloc(1, sizeof(Font));
    if(F == NULL) {
        Log("ERROR: Font - Couldn't allocate memory\n");
        return NULL;
    }

    new(F) Font;

    // Fill font info struct
    stbtt_InitFont(&(F->FontInfo), Data, 0);

    stbtt_GetFontVMetrics(&(F->FontInfo), &(F->Ascent), &(F->Descent),
                                                            &(F->Gap));

    // Cleanup
    PHYSFS_close(FontFile);

    /* *
     * Don't need to cleanup Data, it is stored in FontInfo struct and cleaned
     * up in the destructor.
     * */

    return F;
}


int Font::Bake(GlyphMap** Target, int GlyphStart, int GlyphEnd,
                                                int PixelHeight)
{
    int NumChars = GlyphEnd - GlyphStart;
    if(NumChars < 1) {
        Log("ERROR: Font - Can't bake glyph map (GlyphStart >= GlyphEnd)\n");
        return -1;
    }

    unsigned char* GlyphBitmap = (unsigned char*)malloc(512 * 512);
    if(GlyphBitmap == NULL) {
        Log("ERROR: Font - Error allocating glyph bitmap buffer\n");
        return -1;
    }

    // Temporary. GlyphMap class (TODO!) will hold bakedchar data
    stbtt_bakedchar* GlyphData = (stbtt_bakedchar*)malloc(
                        sizeof(stbtt_bakedchar) * NumChars);

    // Bake into bitmap
    int BakeResult = stbtt_BakeFontBitmap(FontInfo.data, 0, (float)PixelHeight,
                                                    GlyphBitmap, 512, 512,
                                                    GlyphStart, NumChars,
                                                                GlyphData);
    // TODO: Clean this up. Perhaps a goto
    if(BakeResult == 0) {
        delete[] GlyphBitmap;
        delete[] GlyphData;
        return 0;
    }

    // Create temp texture. If error occurs we don't want to change *Target
    Texture* BakedMap = bakge::Texture::Create(512, 512, NULL, GL_ALPHA,
                                                        GL_UNSIGNED_BYTE,
                                                        (void*)GlyphBitmap);
    delete[] GlyphBitmap;

    if(BakedMap == NULL) {
        Log("ERROR: Font - Couldn't creating glyph texture\n");
        return -1;
    }

    *Target = (GlyphMap*)calloc(1, sizeof(GlyphMap));
    if(*Target == NULL) {
        delete[] GlyphData;
        Log("ERROR: Font::Bake - Couldn't allocate GlyphMap memory.\n");
        return 0;
    }

    new(*Target) GlyphMap;

    (*Target)->Tex = BakedMap;
    (*Target)->Data = GlyphData;
    (*Target)->Start = GlyphStart;
    (*Target)->LineHeight = Ascent - Descent + Gap;
    (*Target)->ScaleFactor = stbtt_ScaleForPixelHeight(&FontInfo,
                                                (float)PixelHeight);

    // Negative result is -1 * number of baked glyphs
    if(BakeResult < 0) {
        (*Target)->End = GlyphStart - BakeResult;
        return -BakeResult;
    } else {
        // Baked all glyph
        (*Target)->End = GlyphEnd;
        return NumChars;
    }
}


int Font::GetKerning(int A, int B)
{
    return stbtt_GetCodepointKernAdvance(&FontInfo, A, B);
}

} /* bakge */
