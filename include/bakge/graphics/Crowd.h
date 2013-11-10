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
 * @file Crowd.h
 * @brief Crowd class declaration.
 */

#ifndef BAKGE_GRAPHICS_CROWD_H
#define BAKGE_GRAPHICS_CROWD_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief A grouping of Pawns typically used for instanced rendering.
 *
 * Crowds are large groups of pawns that are drawn using instanced rendering
 * techniques, to avoid the large overhead associated with numerous draw calls.
 * The Crowd class provides a way to manage its members as a whole or as
 * individuals through various methods. Internally the Crowd uses a dynamically
 * allocated array to store data about its members.
 *
 * Typically the Crowd is used for instanced rendering, but it could also see
 * use as a storage class for arbitrary groups of Pawns or Nodes that don't
 * necessarily get rendered, such as pathing waypoints.
 *
 * When rendered using Mesh::DrawInstanced, Crowds use a different set of
 * shader attributes than Nodes or Pawns.
 *
 * @see VertexLib
 */
class BGE_API Crowd : public Pawn
{

protected:

    /* Number of active members in Crowd */
    int Population;

    /* Maximum number of members without resizing the buffer */
    int Capacity;

    /* Dynamically allocated arrays - members' transforms */
    Scalar* Positions;
    Quaternion* Rotations;
    Scalar* Scales;

    /* Buffer for the Crowd's members' model matrices */
    GLuint CrowdBuffer;

    /*! @brief Default Crowd constructor.
     *
     * Default Crowd constructor.
     */
    Crowd();

    /*! @brief Set the contents of a member's buffer from array data.
     *
     * Set the contents of a member's buffer from array data. After a member
     * is transformed, its buffer data must be set to the new and correct
     * values. This internal method does so for the buffer at the
     * appropriate offset for the member's index.
     *
     * @param[in] Index Member array index.
     *
     * @return BGE_SUCCESS if the member's buffer data was successfully set;
     * BGE_FAILURE if any errors occurred.
     */
    Result SetDataStore(int Index);


public:

    /*! @brief Virtual Crowd destructor.
     *
     * Virtual Crowd destructor.
     */
    virtual ~Crowd();

    Result StreamDraw(Drawable* Obj);

    /*! @brief Create a Crowd with a given capacity.
     *
     * Allocates a Crowd and creates storage for a given number of members.
     *
     * @param[in] ReserveMembers Capacity for the new Crowd.
     *
     * @return Pointer to allocated Crowd; NULL if any errors occurred.
     */
    BGE_FACTORY Crowd* Create(int ReserveMembers);

    /*! @brief Bind the Crowd for drawing using instanced rendering techniques.
     *
     * Bind the Crowd for drawing using instanced rendering techniques.
     *
     * @return BGE_SUCCESS if the Crowd was successfully bound; BGE_FAILURE
     * if any errors occurred.
     */
    virtual Result Bind() const;

    /*! @brief Unbind the Crowd, setting OpenGL state to arbitrary defaults.
     *
     * Unbind the Crowd, setting OpenGL state to arbitrary defaults.
     *
     * @return BGE_SUCCESS if the Crowd was successfully unbound; BGE_FAILURE
     * if any errors occurred.
     */
    virtual Result Unbind() const;

    /*! @brief Get storange capacity for the Crowd's members.
     *
     * Get storange capacity for the Crowd's members.
     *
     * @return Maximum number of members the Crowd can contain.
     */
    BGE_INL int GetCapacity() const
    {
        return Capacity;
    }

    /*! @brief Get the current number of members in the Crowd.
     *
     * Get the current number of members in the Crowd.
     *
     * @return Number of members active in the Crowd.
     */
    BGE_INL int GetPopulation() const
    {
        return Population;
    }

    /*! @brief Remove all members from the Crowd.
     *
     * Remove all members from the Crowd.
     *
     * @return BGE_SUCCESS if the Crowd was successfully emptied; BGE_FAILURE
     * if any errors occurred.
     */
    Result Clear();

    /*! @brief Reallocate member storage with a given capacity.
     *
     * Reallocate member storage with a given capacity.
     *
     * @return BGE_SUCCESS if storage was successfully reallocated;
     * BGE_FAILURE if any errors occurred.
     */
    Result Reserve(int NumMembers);

    /*! @brief Translate a Crowd member by a given amount along the X, Y and
     * Z axes.
     *
     * Translate a Crowd member by a given amount along the X, Y and Z axes.
     *
     * @param[in] MemberIndex Index of the member to translate.
     * @param[in] X Distance to translate along the X axis.
     * @param[in] Y Distance to translate along the Y axis.
     * @param[in] Z Distance to translate along the Z axis.
     *
     * @return BGE_SUCCESS if member was successfully translated; BGE_FAILURE
     * if any errors occurred.
     */
    Result TranslateMember(int MemberIndex, Scalar X, Scalar Y, Scalar Z);

    /*! @brief Rotate a member locally around an arbitrary axis.
     *
     * Rotate a member locally around an arbitrary axis.
     *
     * @param[in] MemberIndex Index of the member to rotate.
     * @param[in] Rotation Quaternion to rotate the member by.
     *
     * @return BGE_SUCCESS if the member was successfully rotated;
     * BGE_FAILURE if any errors occurred.
     */
    Result RotateMember(int MemberIndex, Quaternion BGE_NCP Rotation);

    /*! @brief Rotate a member globally around an arbitrary axis.
     *
     * Rotate a member globally around an arbitrary axis.
     *
     * @param[in] MemberIndex Index of the member to rotate.
     * @param[in] Rot Quaternion to rotate the member by.
     *
     * @return BGE_SUCCESS if the member was successfully rotated;
     * BGE_FAILURE if any errors occurred.
     */
    Result RotateMemberGlobal(int MemberIndex, Quaternion BGE_NCP Rot);

    /*! @brief Non-uniformly scale a member.
     *
     * Non-uniformly scale a member.
     *
     * @param[in] MemberIndex Index of the member to scale.
     * @param[in] X Amount to scale along the X axis.
     * @param[in] Y Amount to scale along the Y axis.
     * @param[in] Z Amount to scale along the Z axis.
     *
     * @return BGE_SUCCESS if the member was successfully scaled; BGE_FAILURE
     * if any errors occurred.
     */
    Result ScaleMember(int MemberIndex, Scalar X, Scalar Y, Scalar Z);

    /*! @brief Get the rotation of a Crowd member.
     *
     * Get the rotation of a Crowd member.
     *
     * @param[in] Index Member array index.
     *
     * @return constref to member's rotation.
     */
    Quaternion BGE_NCP GetMemberRotation(int Index) const;

    /*! @brief Set the rotation of a Crowd member.
     *
     * Set the rotation of a Crowd member.
     *
     * @param[in] Index Member array index.
     * @param[in] Rot Rotation to set to.
     *
     * @return constref to member's rotation after assignment.
     */
    Quaternion BGE_NCP SetMemberRotation(int Index, Quaternion BGE_NCP Rot);

}; /* Crowd */

} /* bakge */

#endif /* BAKGE_GRAPHICS_CROWD_H */
