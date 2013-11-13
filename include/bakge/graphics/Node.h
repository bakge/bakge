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
 * @file Node.h
 * @brief Node class declaration.
 */

#ifndef BAKGE_GRAPHICS_NODE_H
#define BAKGE_GRAPHICS_NODE_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief Nodes are objects that have an arbitrary position in Cartesian
 * space.
 *
 * Nodes are used to represent positions in space. When bound, they set OpenGL
 * state so objects are drawn in that position. Node is an abstract interface
 * class, and is only instantiable by sub-classes.
 */
class BGE_API Node : public Bindable
{

protected:

    Vector4 Position;

    /*! @brief Default constructor.
     *
     * Default constructor.
     */
    Node();


public:

    /*! @brief Pure-virtual destructor.
     *
     * Pure-virtual destructor.
     */
    virtual ~Node();

    /*! @brief Create a new Node at a given position.
     *
     * Create a new Node at a given position.
     *
     * @return Pointer to allocated Node; NULL if any errors occurred.
     */
    BGE_FACTORY Node* Create(Scalar X, Scalar Y, Scalar Z);

    /*! @brief Set OpenGL state so objects are rendered from this node's
     * position.
     *
     * Set OpenGL state so objects are rendered from this node's position.
     */
    virtual Result Bind() const;

    /*! @brief Set OpenGL state so objects are rendered from the origin.
     *
     * Set OpenGL state so objects are rendered from the origin.
     */
    virtual Result Unbind() const;

    /*! @brief Set this object's position in Cartesian space.
     *
     * Set this object's position in Cartesian space.
     *
     * @param[in] X New position along the X axis.
     * @param[in] Y New position along the Y axis.
     * @param[in] Z New position along the Z axis.
     *
     * @return const reference to the Node's position after assignment.
     */
    Vector4 BGE_NCP SetPosition(Scalar X, Scalar Y, Scalar Z);

    /*! @brief Get this object's position in Cartesian space.
     *
     * Get this object's position in Cartesian space.
     *
     * @return const reference to this Node's position.
     */
    Vector4 BGE_NCP GetPosition() const;

    /*! @brief Translate the Node by a given amount along the X, Y and Z axes.
     *
     * Translate the Node by a given amount along the X, Y and Z axes.
     *
     * @param[in] X Amount to translate along the X axis.
     * @param[in] Y Amount to translate along the Y axis.
     * @param[in] Z Amount to translate along the Z axis.
     *
     * @returns constref to Node's position after translation.
     */
    Vector4 BGE_NCP Translate(Scalar X, Scalar Y, Scalar Z);

}; /* Node */

} /* bakge */

#endif /* BAKGE_GRAPHICS_NODE_H */
