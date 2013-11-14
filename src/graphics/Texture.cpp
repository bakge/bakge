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
#ifdef _DEBUG
#include <bakge/internal/Debug.h>
#endif // _DEBUG

namespace bakge
{

Texture::Texture()
{
    TextureUnit = GL_TEXTURE0;
    TextureID = 0;
    Width = 0;
    Height = 0;
}


Texture::~Texture()
{
    if(TextureID != 0)
        glDeleteTextures(1, &TextureID);
}


Result Texture::Bind() const
{
#ifdef _DEBUG
    while(glGetError() != GL_NO_ERROR)
        ;
#endif // _DEBUG

    glActiveTexture(TextureUnit);

    glBindTexture(GL_TEXTURE_2D, TextureID);

#ifdef _DEBUG
    if(glGetError() == GL_INVALID_VALUE) {
        Log("Texture: Invalid texture name %d\n", TextureID);
        return BGE_FAILURE;
    }
#endif // _DEBUG

    return BGE_SUCCESS;
}


Result Texture::Unbind() const
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);

    return BGE_SUCCESS;
}


Texture* Texture::Create(int Width, int Height, const GLint* Params,
                                            GLint Format, GLenum Type,
                                                            void* Data)
{
    static const GLint DefaultParams[] = {
        GL_TEXTURE_MIN_FILTER, GL_LINEAR,
        GL_TEXTURE_MAG_FILTER, GL_LINEAR,
        GL_TEXTURE_WRAP_S, GL_REPEAT,
        GL_TEXTURE_WRAP_T, GL_REPEAT,
        0
    };

#ifdef _DEBUG
    int Max;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &Max);

    if(Width < 0 || Width >= Max) {
        Log("Texture: Invalid texture width %d\n", Width);
        return NULL;
    }

    if(Height < 0 || Height >= Max) {
        Log("Texture: Invalid texture height %d\n", Height);
        return NULL;
    }

    /* *
     * When a buffer is bound to GL_PIXEL_UNPACK_BUFFER, there are a few
     * misleading GL_INVALID_OPERATION errors that can occur.
     * */
    GLint BufferBinding;
    glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &BufferBinding);
    if(BufferBinding != 0) {
        Log("Texture: Cannot create a texture while a non-zero name is "
                                    "bound to GL_PIXEL_UNPACK_BUFFER\n");
        return NULL;
    }
#endif // _DEBUG

    /* Generate an OpenGL texture */
    GLuint Tex;
    glGenTextures(1, &Tex);

#ifdef _DEBUG
    /* Check if error occured while generating the texture */
    if(Tex == 0) {
        Log("Texture: Error generating GL texture\n");
        return NULL;
    }
#endif /* _DEBUG */

    // Save currently bound texture
    GLint Old;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &Old);
    glBindTexture(GL_TEXTURE_2D, Tex);

    if(Params == NULL)
        Params = DefaultParams;

#ifdef _DEBUG
    while(glGetError() != GL_NO_ERROR)
        ;
#endif //_DEBUG

    // Pick out and set texture parameters/values
    GLint i = 0;
    GLint Pair[2]; // Index 0 is parameter name; 1 is value
    GLint At = 0;
    while(Params[i] != 0) {
        Pair[At] = Params[i];
        // If we just picked a value, set the texture parameter
        if(At == 1)
            glTexParameteri(GL_TEXTURE_2D, Pair[0], Pair[1]);

#ifdef _DEBUG
        if(glGetError() == GL_INVALID_ENUM) {
            Log("Texture: Invalid parameter name %x or value %x\n", Pair[0],
                                                                    Pair[1]);
            return NULL;
        }
#endif // _DEBUG

        At ^= 1; // Flip between 0 and 1
        ++i;
    }

#ifdef _DEBUG
    while(glGetError() != GL_NO_ERROR)
        ;
#endif // _DEBUG

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, Format, Type,
                                                                    Data);

#ifdef _DEBUG
    GLenum Error = glGetError();

    while(1) {
        if(Error == GL_NO_ERROR)
            break;

        switch(Error) {

        case GL_INVALID_ENUM:
            Log("Texture: Invalid pixel data type %x\n", Type);
            break;

        // Some pixel types require specific formats (we only use GL_RGBA)
        case GL_INVALID_OPERATION:
            Log("Texture: Incompatible pixel data type %x\n", Type);
            break;

        case GL_OUT_OF_MEMORY:
            Log("Texture: Not enough memory to create texture\n");
            exit(-1);

        default:
            Log("Texture: Unexpected OpenGL error %x\n", Error);
        }

        Error = glGetError();
    }
#endif // _DEBUG

    // Bind the texture that was previously bound
    glBindTexture(GL_TEXTURE_2D, Old);

    Texture* T = (Texture*)calloc(1, sizeof(Texture));
    if(T == NULL) {
        Log("Texture: Couldn't allocate memory\n");
        return NULL;
    }

    new(T) Texture;

    T->TextureID = Tex;
    T->Width = Width;
    T->Height = Height;

    return T;
}


Result Texture::SetLocation(GLenum Loc)
{
#ifdef _DEBUG
    // Determine max texture units available
    int TexUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &TexUnits);

    if(Loc < GL_TEXTURE0 || Loc >= (GLenum)(GL_TEXTURE0 + TexUnits)) {
        Log("Texture: Invalid texture location %d. Valid range is "
                                    "[GL_TEXTURE0, GL_TEXTURE%d]\n",
                                        TextureUnit, TexUnits - 1);
        return BGE_FAILURE;
    }
#endif // _DEBUG

    TextureUnit = Loc;

    return BGE_SUCCESS;
}

} /* bakge */
