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

#ifndef BAKGE_MATH_VECTOR3_H
#define BAKGE_MATH_VECTOR3_H

#include <bakge/Bakge.h>

namespace bakge
{
namespace math
{

class Vector2
{

public:

    Vector2();
    Vector2(Scalar X, Scalar Y, Scalar Z);
    Vector2(Vector2 BGE_NCP Other);
    ~Vector2();

    Scalar& operator[](int BGE_NCP At);
    Scalar BGE_NCP operator[](int BGE_NCP At) const;

    Vector2 BGE_NCP operator=(Vector2 BGE_NCP Other);
    bool operator==(Vector2 BGE_NCP Other);

    Vector2 BGE_NCP operator+=(Vector2 BGE_NCP Other);
    Vector2 BGE_NCP operator-=(Vector2 BGE_NCP Other);
    Vector2 BGE_NCP operator*=(Vector2 BGE_NCP Other);
    Vector2 BGE_NCP operator/=(Vector2 BGE_NCP Other);
    Vector2 BGE_NCP operator/=(Scalar Value);

    Vector2 operator+(Vector2 BGE_NCP Other);
    Vector2 operator-(Vector2 BGE_NCP Other);
    Vector2 operator*(Vector2 BGE_NCP Other);
    Vector2 operator/(Vector2 BGE_NCP Other);
    Vector2 operator/(Scalar BGE_NCP Value);

    Vector2 Normalized() const;

    Scalar Length() const;
    Scalar LengthSquared() const;

    static Vector2 Normalize(Vector2 BGE_NCP Other);
    static Scalar Dot(Vector2 BGE_NCP Left, Vector2 BGE_NCP Right);
    static Vector2 UnitVector(Scalar BGE_NCP X, Scalar BGE_NCP Y, Scalar BGE_NCP Z);

private:

    Scalar Val[3];

}; /* Vector2 */

Vector2 operator+(Vector2 BGE_NCP Left, Vector2 BGE_NCP Right);
Vector2 operator-(Vector2 BGE_NCP Left, Vector2 BGE_NCP Right);
Vector2 operator-(Vector2 BGE_NCP This); /* unary */
//Vector2 operator*(Vector2 BGE_NCP Left, Vector2 BGE_NCP Right);
//Vector2 operator*(Vector2 BGE_NCP Left, Scalar BGE_NCP Value);
Vector2 operator/(Vector2 BGE_NCP Left, Vector2 BGE_NCP Right);
Vector2 operator/(Vector2 BGE_NCP Left, Scalar BGE_NCP Value);

} /* math */
} /* bakge */

#endif /* BAKGE_MATH_VECTOR3_H */