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

Geometry::Geometry()
{
    DrawStyle = GL_LINE_STRIP;
    PointsBuffer = 0;
    IndicesBuffer = 0;
    NumPoints = 0;
}


Geometry::~Geometry()
{
    if(PointsBuffer != NULL)
        glDeleteBuffers(1, &PointsBuffer);

    if(IndicesBuffer != NULL)
        glDeleteBuffers(1, &IndicesBuffer);
}


Result Geometry::Draw() const
{
#ifdef _DEBUG
    while(glGetError() != GL_NO_ERROR)
        ;
#endif // _DEBUG

    glDrawElements(DrawStyle, NumPoints, GL_UNSIGNED_INT, (void*)0);

#ifdef _DEBUG
    GLenum Error;
    while(1) {
        Error = glGetError();

        if(Error == GL_NO_ERROR)
            break;

        switch(Error) {

        case GL_INVALID_OPERATION:
            Log("ERROR: Geometry - Error while rendering (geometry shader "
                                    "and draw mode incompatible OR a bound "
                                            "buffer is currently mapped).\n");
            break;

        default:
            Log("ERROR: Geometry - Unexpected error %s while rendering.\n",
                                                    _GetGLErrorName(Error));
            return BGE_FAILURE;
        }
    }
#endif // _DEBUG

    return BGE_FAILURE;
}


Result Geometry::DrawInstanced(int Count) const
{
    glDrawElementsInstancedBaseVertex(DrawStyle, NumPoints,
                  GL_UNSIGNED_INT, (void*)0, Count, 0);

    return BGE_SUCCESS;
}


Result Geometry::Bind() const
{
    GLint Program;

    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0)
        return BGE_FAILURE;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndicesBuffer);

    // Set vertex attribute properties
    GLint Location = glGetAttribLocation(Program, BGE_VERTEX_ATTRIBUTE);
    if(Location >= 0) {
        glBindBuffer(GL_ARRAY_BUFFER, PointsBuffer);
        glEnableVertexAttribArray(Location);
        glVertexAttribPointer(Location, 3, GL_FLOAT, GL_FALSE, 0, 0);
#ifdef _DEBUG
    } else {
        BGE_WARN_MISSING_ATTRIBUTE(BGE_VERTEX_ATTRIBUTE);
#endif // _DEBUG
    }

    return BGE_SUCCESS;
}


Result Geometry::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLint Program;

    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0)
        return BGE_FAILURE;

    // Disable the bge_Vertex pointer
    GLint Location = glGetAttribLocation(Program, BGE_VERTEX_ATTRIBUTE);
    if(Location >= 0) {
        glDisableVertexAttribArray(Location);
#ifdef _DEBUG
    } else {
        BGE_WARN_MISSING_ATTRIBUTE(BGE_VERTEX_ATTRIBUTE);
#endif // _DEBUG
    }

    return BGE_SUCCESS;
}


Result Geometry::SetDrawStyle(GEOMETRY_DRAW_STYLE Style)
{
    switch(Style) {

    case GEOMETRY_DRAW_STYLE_LINES:
        DrawStyle = GL_LINE_STRIP;
        break;

    case GEOMETRY_DRAW_STYLE_POINTS:
        DrawStyle = GL_POINTS;
        break;

    default:
        Log("ERROR: Geometry - Invalid draw style enumeration.\n");
        return BGE_FAILURE;
    }

    return BGE_SUCCESS;
}

} // bakge
