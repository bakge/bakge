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

Shape::Shape() : BufferList(NUM_SHAPE_BUFFERS)
{
    DrawStyle = GL_TRIANGLES;

    Positions = NULL;
    Normals = NULL;
    TexCoords = NULL;
    Indices = NULL;

    NumVertices = 0;
    NumIndices = 0;
}


Shape::~Shape()
{
    if(Positions != NULL) {
        free(Positions);
        Positions = NULL;
    }

    if(Normals != NULL) {
        free(Normals);
        Normals = NULL;
    }

    if(TexCoords != NULL) {
        free(TexCoords);
        TexCoords = NULL;
    }

    if(Indices != NULL) {
        free(Indices);
        Indices = NULL;
    }
}


Result Shape::Bind() const
{
    GLint Program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0) {
        return BGE_FAILURE;
    }

    /* Check each of our attributes' locations to ensure they exist */
    GLint Location = glGetAttribLocation(Program, BGE_VERTEX_ATTRIBUTE);
    if(Location >= 0) {
        glBindBuffer(GL_ARRAY_BUFFER, GetBuffer(SHAPE_BUFFER_POSITIONS));
        glEnableVertexAttribArray(Location);
        glVertexAttribPointer(Location, 3, GL_FLOAT, GL_FALSE, 0, 0);
#ifdef _DEBUG
    } else {
        BGE_WARN_MISSING_ATTRIBUTE(BGE_VERTEX_ATTRIBUTE);
#endif // _DEBUG
    }

    Location = glGetAttribLocation(Program, BGE_NORMAL_ATTRIBUTE);
    if(Location >= 0) {
        glBindBuffer(GL_ARRAY_BUFFER, GetBuffer(SHAPE_BUFFER_NORMALS));
        glEnableVertexAttribArray(Location);
        glVertexAttribPointer(Location, 3, GL_FLOAT, GL_FALSE, 0, 0);
#ifdef _DEBUG
    } else {
        BGE_WARN_MISSING_ATTRIBUTE(BGE_NORMAL_ATTRIBUTE);
#endif // _DEBUG
    }

    Location = glGetAttribLocation(Program, BGE_TEXCOORD_ATTRIBUTE);
    if(Location >= 0) {
        glBindBuffer(GL_ARRAY_BUFFER, GetBuffer(SHAPE_BUFFER_TEXCOORDS));
        glEnableVertexAttribArray(Location);
        glVertexAttribPointer(Location, 2, GL_FLOAT, GL_FALSE, 0, 0);
#ifdef _DEBUG
    } else {
        BGE_WARN_MISSING_ATTRIBUTE(BGE_TEXCOORD_ATTRIBUTE);
#endif // _DEBUG
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetBuffer(SHAPE_BUFFER_INDICES));

    return BGE_SUCCESS;
}


Result Shape::Unbind() const
{
    GLint Program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0) {
        return BGE_FAILURE;
    }

    GLint Location = glGetAttribLocation(Program, BGE_VERTEX_ATTRIBUTE);
    if(Location >= 0) {
        glDisableVertexAttribArray(Location);
#ifdef _DEBUG
    } else {
        BGE_WARN_MISSING_ATTRIBUTE(BGE_VERTEX_ATTRIBUTE);
#endif // _DEBUG
    }

    Location = glGetAttribLocation(Program, BGE_NORMAL_ATTRIBUTE);
    if(Location >= 0) {
        glDisableVertexAttribArray(Location);
#ifdef _DEBUG
    } else {
        BGE_WARN_MISSING_ATTRIBUTE(BGE_NORMAL_ATTRIBUTE);
#endif // _DEBUG
    }

    Location = glGetAttribLocation(Program, BGE_TEXCOORD_ATTRIBUTE);
    if(Location >= 0) {
        glDisableVertexAttribArray(Location);
#ifdef _DEBUG
    } else {
        BGE_WARN_MISSING_ATTRIBUTE(BGE_TEXCOORD_ATTRIBUTE);
#endif // _DEBUG
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return BGE_SUCCESS;
}


Result Shape::SetDrawStyle(SHAPE_DRAW_STYLE Style)
{
    switch(Style) {

    case SHAPE_DRAW_STYLE_SOLID:
        DrawStyle = GL_TRIANGLES;
        break;

    case SHAPE_DRAW_STYLE_WIREFRAME:
        // We don't actually draw as GL_LINE_LOOP, that connects the first
        // and last vertices. Instead we just check for GL_LINE_LOOP and set
        // polygon draw modes
        DrawStyle = GL_LINE_LOOP;
        break;

    case SHAPE_DRAW_STYLE_POINTS:
        DrawStyle = GL_POINTS;
        break;

    default:
        return BGE_FAILURE;
    }

    return BGE_SUCCESS;
}


Result Shape::Draw() const
{
    if(DrawStyle == GL_LINE_LOOP)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDrawElements(DrawStyle, NumIndices, GL_UNSIGNED_INT, (GLvoid*)0);

    if(DrawStyle == GL_LINE_LOOP)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    return BGE_SUCCESS;
}


Result Shape::DrawInstanced(int Count) const
{
    if(DrawStyle == GL_LINE_LOOP)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDrawElementsInstancedBaseVertex(GL_TRIANGLES, NumIndices,
                            GL_UNSIGNED_INT, (void*)0, Count, 0);

    if(DrawStyle == GL_LINE_LOOP)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    return BGE_SUCCESS;
}

} // bakge
