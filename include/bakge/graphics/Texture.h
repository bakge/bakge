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
 * @file Texture.h
 * @brief Texture class declaration.
 */

#ifndef BAKGE_GRAPHICS_TEXTURE_H
#define BAKGE_GRAPHICS_TEXTURE_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief RGBA 2D texture wrapper class.
 *
 * Textures are used to color meshes and other polygons drawn with OpenGL,
 * such as meshes or a GUI. They can also be used to hold framebuffers to
 * which OpenGL renders to. Textures are stored in GL_RGBA format. To use
 * a different color format for your texture, you will have to program
 * directly against the OpenGL API.
 */
class BGE_API Texture : public Bindable, public IRectangle<uint32>
{

protected:

    GLenum TextureUnit;
    GLuint TextureID;


public:

    /*! @brief Default Texture constructor.
     *
     * Default Texture constructor.
     */
    Texture();

    /*! @brief Texture destructor.
     *
     * Texture destructor.
     */
    ~Texture();

    /*! @brief Create a texture instance and upload texture data to OpenGL.
     *
     * This factory method accepts texture data and metadata and creates
     * and uploads to a new OpenGL texture.
     *
     * @param[in] Width Width of the texture in pixels.
     * @param[in] Height Height of the texture in pixels.
     * @param[in] Params Array of texture parameters and corresponding values.
     *                   After the last parameter-value pair, add a 0 to
     *                   designate the end of the array. If NULL is passed, a
     *                   set of sensible defaults is used instead.
     * @param[in] Format OpenGL format of the data to upload.
     * @param[in] Type Type of data being uploaded.
     * @param[in] Data The texture data to upload.
     *
     * @return Pointer to allocated Texture; NULL if any errors occurred.
     *
     * @see http://www.opengl.org/wiki/Texture
     */
    BGE_FACTORY Texture* Create(int Width, int Height, const GLint* Params,
                                                GLint Format, GLenum Type,
                                                                void* Data);

    /*! @brief Bind the texture as active in its assigned location.
     *
     * Binding the texture sets it as the active texture for its assigned
     * location. Color information can be extracted from the texture using
     * the texture (or texture2D) call in your GLSL shader using the
     * appropriate uniform location and texture coordinates.
     *
     * @return BGE_SUCCESS if the texture was successfully bound; BGE_FAILURE
     * if any errors occurred.
     */
    Result Bind() const;

    /*! @brief Unbind the texture from its assigned location.
     *
     * Unbinding a texture is important so future texture-related OpenGL
     * calls don't affect it.
     *
     * @return BGE_SUCCESS if the texture was successfully unbound; BGE_FAILURE
     * if any errors occurred.
     */
    Result Unbind() const;

    /*! @brief Set the texture unit location.
     *
     * Set the texture unit location. The number of available texture units
     * varies across OpenGL implementations and hardware. If the Texture is
     * currently bound it will remain bound to its old texture unit until
     * it has been bound again.
     *
     * @return BGE_SUCCESS if the texture unit location was successfully
     * set; BGE_FAILURE if the location is invalid.
     *
     * @see http://www.opengl.org/sdk/docs/man/xhtml/glActiveTexture.xml
     */
    Result SetLocation(GLenum Loc);

}; /* Texture */

} /* bakge */

#endif /* BAKGE_GRAPHICS_TEXTURE_H */
