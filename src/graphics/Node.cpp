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

Node::Node()
{
}


Node::~Node()
{
}


Result Node::Bind() const
{
    GLint Program, Location;

    /* Retrieve current shader program */
    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0)
        return BGE_FAILURE;

    /* Retrieve location of the bge_Position vec4 */
    Location = glGetUniformLocation(Program, "bge_Position");
    if(Location < 0)
        return BGE_FAILURE;

    /* Assign this node's position as bge_Position */
    glUniform4dv(Location, 4, &Position[0]);

    return BGE_SUCCESS;
}


Result Node::Unbind() const
{
    static const math::Vector4 Origin;
    GLint Program, Location;

    /* Retrieve location of the bge_Position vec4 */
    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0)
        return BGE_FAILURE;

    /* Retrieve location of the bge_Position vec4 */
    Location = glGetUniformLocation(Program, "bge_Position");
    if(Location < 0)
        return BGE_FAILURE;

    /* Assign origin position as bge_Position */
    glUniform4dv(Location, 4, &Origin[0]);

    return BGE_SUCCESS;
}


Result Node::Draw() const
{
    glBegin(GL_POINTS);
    /* Matrix is translated to position after Bind() */
    glVertex3i(0, 0, 0);
    glEnd();
    return BGE_SUCCESS;
}

} /* bakge */

