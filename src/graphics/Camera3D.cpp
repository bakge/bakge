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

Camera3D::Camera3D()
{
    Near = 0.1f;
    Far = 500.0f;
    Aspect = 1.5f;
    FOV = 80.0f;
}


Camera3D::~Camera3D()
{
}


Vector4 BGE_NCP Camera3D::SetTarget(Scalar X, Scalar Y, Scalar Z)
{
    Target[0] = X;
    Target[1] = Y;
    Target[2] = Z;

    return Target;
}


Vector4 BGE_NCP Camera3D::GetTarget() const
{
    return Target;
}


Result Camera3D::Bind() const
{
    GLint Location, Program = 0;

    glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
    if(Program == 0)
        return BGE_FAILURE;

    Matrix Proj, View;
    Result Res = BGE_SUCCESS;

    Proj.SetPerspective(FOV, Aspect, Near, Far);
    View.SetLookAt(Position, Target, Vector4(0, 1, 0, 0));

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

    glUniformMatrix4fv(Location, 1, GL_FALSE, &Proj[0]);

#ifdef _DEBUG
    GLenum Error;
    while(1) {
        Error = glGetError();
        if(Error == GL_NO_ERROR)
            break;

        Res = BGE_FAILURE;
        Log("WARNING: Unexpected GL error %s while setting uniform "
                                            "%s in Camera3D::Bind\n",
                                                _GetGLErrorName(Error),
                                                BGE_PROJECTION_UNIFORM);
    }
#endif // _DEBUG

    /* Now the view transform */
    Location = glGetUniformLocation(Program, BGE_VIEW_UNIFORM);
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

    glUniformMatrix4fv(Location, 1, GL_FALSE, &View[0]);

#ifdef _DEBUG
    while(1) {
        Error = glGetError();
        if(Error == GL_NO_ERROR)
            break;

        Res = BGE_FAILURE;
        Log("WARNING: Unexpected GL error %s while setting uniform "
                                            "%s in Camera3D::Bind\n",
                                                _GetGLErrorName(Error),
                                                    BGE_VIEW_UNIFORM);
    }
#endif // _DEBUG

    return Res;
}


Result Camera3D::Unbind() const
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
                                            "%s in Camera3D::Unbind\n",
                                                _GetGLErrorName(Error),
                                                BGE_PROJECTION_UNIFORM);
    }
#endif // _DEBUG

    /* Now the view transform */
    Location = glGetUniformLocation(Program, BGE_VIEW_UNIFORM);
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
    while(1) {
        Error = glGetError();
        if(Error == GL_NO_ERROR)
            break;

        Res = BGE_FAILURE;
        Log("WARNING: Unexpected GL error %s while setting uniform "
                                            "%s in Camera3D::Unbind\n",
                                                _GetGLErrorName(Error),
                                                    BGE_VIEW_UNIFORM);
    }
#endif // _DEBUG

    return Res;
}

} /* bakge */
