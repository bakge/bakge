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

#ifndef BAKGE_GRAPHICS_PAWN_H
#define BAKGE_GRAPHICS_PAWN_H

#include <bakge/Bakge.h>

namespace bakge
{

class BGE_API Pawn : public Node
{

public:

    Pawn();
    virtual ~Pawn();

    virtual Result Bind() const;
    virtual Result Unbind() const;

    virtual Result SetScale(Scalar X, Scalar Y, Scalar Z);
    virtual Vector4 BGE_NCP GetScale() const;

    Quaternion BGE_NCP SetRotation(Quaternion Rotation);
    Quaternion BGE_NCP GetRotation() const;


protected:

    Quaternion Facing;
    Vector4 Scale;

}; /* Pawn */

} /* bakge */

#endif /* BAKGE_GRAPHICS_PAWN_H */
