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
 * @file Shape.h
 * @brief Shape class declaration.
 */

#ifndef BAKGE_GRAPHICS_SHAPE_H
#define BAKGE_GRAPHICS_SHAPE_H

#include <bakge/Bakge.h>

namespace bakge
{

enum SHAPE_DRAW_STYLE
{
    SHAPE_DRAW_STYLE_POINTS = 0,
    SHAPE_DRAW_STYLE_WIREFRAME,
    SHAPE_DRAW_STYLE_SOLID,
    NUM_SHAPE_DRAW_STYLES
};

/*! @brief Shape buffer enumeration.
 *
 * Shapes contain several OpenGL buffers used for storing various data such
 * as vertex positions and texture coordinates. For convenience and clarity
 * the triangles of the buffers are enumerated here.
 */
enum SHAPE_BUFFERS
{
    /*! @brief Vertex positions buffer index.
     *
     * Stores vertex positions in 3D Cartesian space.
     */
    SHAPE_BUFFER_POSITIONS = 0,

    /*! @brief Vertex normal vectors buffer index.
     *
     * Stores vertex normals as unit vectors in 3D Cartesian space.
     */
    SHAPE_BUFFER_NORMALS,

    /*! @brief Vertex texture coordinates buffer index.
     *
     * Stores a vertex's texture coordinates in 2D Cartesian space.
     */
    SHAPE_BUFFER_TEXCOORDS,

    /*! @brief Triangle triangles buffer index.
     *
     * Stores triangles' vertex indices. A Shape is drawn as a series of
     * triangles, using every 3 indices in this buffer as one triangle.
     */
    SHAPE_BUFFER_INDICES,

    /*! @brief Total number of mesh buffers for any given Shape.
     *
     * This value can be used to iterate through all Shape buffers.
     */
    NUM_SHAPE_BUFFERS
};

/*! @brief A Shape is an convex enclosed object in 3D Cartesian space.
 *
 * Shapes are usually drawn as a series of triangles, while Geometries are
 * drawn as a series of line segments. An example of a shape would be a cube.
 * Since shapes are solid, convex objects, they can be textured, and therefore
 * require more vertex data than geometries (texture coordinates, vertex
 * normals, etc).
 */
class BGE_API Shape : public Drawable, protected BufferList
{

protected:

    GLenum DrawStyle;

    int NumVertices;
    int NumIndices;

    Scalar* Positions;
    Scalar* Normals;
    Scalar* TexCoords;
    uint32* Indices;

    /*! @brief Default Shape constructor.
     *
     * Default Shape constructor.
     */
    Shape();


public:

    /*! @brief Virtual Shape destructor.
     *
     * Virtual Shape destructor.
     */
    virtual ~Shape();

    virtual Result Bind() const;

    virtual Result Unbind() const;

    virtual Result Draw() const;

    virtual Result DrawInstanced(int Count) const;

    virtual Result SetDrawStyle(SHAPE_DRAW_STYLE Style);

    /*! @brief Get the number of triangles in the Shape.
     *
     * Get the number of triangles in the Shape.
     *
     * @return Number of triangles in the Shape.
     */
    BGE_INL int GetNumIndices() const
    {
        return NumIndices;
    }

    /*! @brief Get the number of vertices in the Shape.
     *
     * Get the number of vertices in the Shape.
     *
     * @return Number of vertices in the Shape.
     */
    BGE_INL int GetNumVertices() const
    {
        return NumVertices;
    }

}; // Shape

} // bakge

#endif // BAKGE_GRAPHICS_SHAPE_H
