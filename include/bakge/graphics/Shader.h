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
 * @file Shader.h
 * @brief Shader class declaration.
 */

#ifndef BAKGE_GRAPHICS_SHADER_H
#define BAKGE_GRAPHICS_SHADER_H

#include <bakge/Bakge.h>

#define BGE_MODEL_UNIFORM "bge_Model"
#define BGE_VIEW_UNIFORM "bge_View"
#define BGE_PROJECTION_UNIFORM "bge_Projection"
#define BGE_DIFFUSE_UNIFORM "bge_Diffuse"

#define BGE_INSTANCEMODEL_ATTRIBUTE "bge_InstanceModel"
#define BGE_VERTEX_ATTRIBUTE "bge_Vertex"
#define BGE_NORMAL_ATTRIBUTE "bge_Normal"
#define BGE_TEXCOORD_ATTRIBUTE "bge_TexCoord"

namespace bakge
{

/*! @brief GLSL shader wrapper class.
 *
 * Bakge uses shaders for all rendering. When binding and drawing with any
 * resource such as a Mesh or Texture, shaders attributes or uniforms are set,
 * providing you with the necessary variables to build GLSL shaders such
 * as model and view transforms, or vertex data like positions or normals.
 *
 * This class serves as a wrapper with some auxiliary API for loading shaders
 * that get compiled with the variables required by Bakge classes to properly
 * render the scene.
 */
class BGE_API Shader : public Bindable
{
    static Shader* GenericShader;
    static GLuint VertexLib;
    static GLuint FragmentLib;
    friend Window* Window::Create(int, int, int);

    static Result InitShaderLibrary();
    static Result DeinitShaderLibrary();
    friend BGE_API Result Init(int, char*[]);
    friend BGE_API Result Deinit();

    GLuint Vertex;
    GLuint Fragment;
    GLuint Program;

    Result Link();
    Result Unlink();
    Result DeleteShaders();
    Result DeleteProgram();

    /* *
     * Compile a shader and report any errors or warnings
     * Returns BGE_FAILURE if compilation failed
     * */
    static Result Compile(GLuint Handle);


public:

    /*! @brief Default Shader constructor.
     *
     * Default Shader constructor.
     */
    Shader();

    /*! @brief Shader destructor.
     *
     * Shader destructor.
     */
    ~Shader();

    /*! @brief Create a shader using a variable number of null-terminated
     * source strings.
     *
     * This factory method compiles a variable number of fragment and vertex
     * shader sources along with the Bakge "header files" that contain required
     * variables for rendering objects. The source strings are expected to be
     * null-terminated.
     *
     * @param[in] NumVertex Number of vertex shader source strings.
     * @param[in] NumFragment Number of fragment shader source strings.
     * @param[in] VertexShaders Array of null-terminated vertex shader source
     *            strings.
     * @param[in] FragmentShaders Array of null-terminated fragment shader
     *            source strings.
     *
     * @return Pointer to allocated Shader; NULL if any errors occurred.
     */
    BGE_FACTORY Shader* LoadFromStrings(int NumVertex, int NumFragment,
                                            const char** VertexShaders,
                                            const char** FragmentShaders);

    /*! @brief Bind the GLSL shader as the current shader program.
     *
     * Bind the GLSL shader as the current shader program. Sets various
     * uniforms used for rendering to sensible defaults. As a result one
     * should always bind a Shader before a Texture, Pawn or any other
     * object used for rendering.
     *
     * @return BGE_SUCCESS if the shader was successfully bound; BGE_FAILURE
     * if any errors occurred.
     */
    Result Bind() const;

    /*! @brief Unbind a Shader and binds in its place a generic Shader.
     *
     * Unbind a Shader and binds in its place a generic Shader.
     *
     * @return BGE_SUCCESS if the default shader was successfully bound;
     * BGE_FAILURE if any errors occurred.
     */
    Result Unbind() const;

}; /* Shader */

} /* bakge */

#endif /* BAKGE_GRAPHICS_SHADER_H */
