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

Camera2D::Camera2D()
{
}


Camera2D::~Camera2D()
{
}


Result Camera2D::Bind() const
{
    GLint Location, Program = 0;
    Matrix Mat;
    Result Res = BGE_SUCCESS;

    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0)
        return BGE_FAILURE;

    /* First we'll set the perspective */
    Location = glGetUniformLocation(Program, BGE_PROJECTION_UNIFORM);
    if(Location < 0) {
        Res = BGE_FAILURE;
#ifdef _DEBUG
        BGE_WARN_MISSING_UNIFORM(BGE_PROJECTION_UNIFORM);
#endif // _DEBUG
    }

    Mat.SetOrthographic(Position[0], Position[0] + Span[0], Position[1],
                                    Position[1] + Span[1], Position[2],
                                                Position[2] + Span[2]);

#ifdef _DEBUG
    while(glGetError() != GL_NO_ERROR)
        ;
#endif // _DEBUG

    glUniformMatrix4fv(Location, 1, GL_FALSE, &Mat[0]);

#ifdef _DEBUG
    GLenum Error;
    while(1) {
        Error = glGetError();
        if(Error == GL_NO_ERROR)
            break;

        Res = BGE_FAILURE;
        Log("WARNING: Unexpected GL error %s while setting uniform "
                                            "%s in Camera2D::Bind\n",
                                                _GetGLErrorName(Error),
                                                BGE_PROJECTION_UNIFORM);
    }
#endif // _DEBUG

    /* Now the view transform */
    Location = glGetUniformLocation(Program, BGE_VIEW_UNIFORM);
    if(Location < 0) {
        Res = BGE_FAILURE;
#ifdef _DEBUG
        BGE_WARN_MISSING_UNIFORM(BGE_VIEW_UNIFORM);
#endif // _DEBUG
    }

#ifdef _DEBUG
    while(glGetError() != GL_NO_ERROR)
        ;
#endif // _DEBUG

    glUniformMatrix4fv(Location, 1, GL_FALSE, &Matrix::Identity[0]);

#ifdef _DEBUG
    while(1) {
        Error = glGetError();
        if(Error == GL_NO_ERROR)
            break;

        Res = BGE_FAILURE;
        Log("WARNING: Unexpected GL error %s while setting uniform "
                                            "%s in Camera2D::Bind\n",
                                                _GetGLErrorName(Error),
                                                    BGE_VIEW_UNIFORM);
    }
#endif // _DEBUG

    return Res;
}


Result Camera2D::Unbind() const
{
    GLint Location, Program = 0;
    Result Res = BGE_SUCCESS;

    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0)
        return BGE_FAILURE;

    /* First we'll set the perspective */
    Location = glGetUniformLocation(Program, BGE_PROJECTION_UNIFORM);
    if(Location < 0) {
        Res = BGE_FAILURE;
#ifdef _DEBUG
        BGE_WARN_MISSING_UNIFORM(BGE_PROJECTION_UNIFORM);
#endif // _DEBUG
    }

#ifdef _DEBUG
    while(glGetError() != GL_NO_ERROR)
        ;
#endif // _DEBUG

    glUniformMatrix4fv(Location, 1, GL_FALSE, &Matrix::Identity[0]);

#ifdef _DEBUG
    GLenum Error;
    while(1) {
        Error = glGetError();
        if(Error == GL_NO_ERROR)
            break;

        Res = BGE_FAILURE;
        Log("WARNING: Unexpected GL error %s while setting uniform "
                                            "%s in Camera2D::Unbind\n",
                                                _GetGLErrorName(Error),
                                                BGE_PROJECTION_UNIFORM);
    }
#endif // _DEBUG

    /* Now the view transform */
    Location = glGetUniformLocation(Program, BGE_VIEW_UNIFORM);
    if(Location < 0) {
        Res = BGE_FAILURE;
#ifdef _DEBUG
        BGE_WARN_MISSING_UNIFORM(BGE_VIEW_UNIFORM);
#endif // _DEBUG
    }

#ifdef _DEBUG
    while(glGetError() != GL_NO_ERROR)
        ;
#endif // _DEBUG

    glUniformMatrix4fv(Location, 1, GL_FALSE, &Matrix::Identity[0]);

#ifdef _DEBUG
    while(1) {
        Error = glGetError();
        if(Error == GL_NO_ERROR)
            break;

        Res = BGE_FAILURE;
        Log("WARNING: Unexpected GL error %s while setting uniform "
                                            "%s in Camera2D::Unbind\n",
                                                _GetGLErrorName(Error),
                                                    BGE_VIEW_UNIFORM);
    }
#endif // _DEBUG

    return Res;
}

} /* bakge */
