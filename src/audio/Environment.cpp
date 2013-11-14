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

Environment::Environment()
{
    Device = NULL;
    Context = NULL;
}


Environment::~Environment()
{
    if(Context != NULL)
        alcDestroyContext(Context);

    if(Device != NULL)
        alcCloseDevice(Device);
}


Environment* Environment::Create()
{
    Environment* AC = (Environment*)calloc(1, sizeof(Environment));
    if(AC == NULL)
        return NULL;

    new(AC) Environment;

    AC->Device = alcOpenDevice(NULL);
    if(AC->Device == NULL) {
        Log("ERROR: Environment - Error creating OpenAL device\n");
        return NULL;
    }

    AC->Context = alcCreateContext(AC->Device, NULL);
    if(AC->Context == NULL) {
        Log("ERROR: Environment - Error creating OpenAL context\n");
        delete AC;
        return NULL;
    }

    return AC;
}


Result Environment::Bind() const
{
    alcMakeContextCurrent(Context);

    return BGE_SUCCESS;
}


Result Environment::Unbind() const
{
    alcMakeContextCurrent(NULL);

    return BGE_FAILURE;
}


Result Environment::SetListenerPosition(Scalar X, Scalar Y, Scalar Z)
{
#ifdef _DEBUG
    ALenum Error = alGetError();
#endif // _DEBUG

    alListener3f(AL_POSITION, X, Y, Z);

#ifdef _DEBUG
    Error = alGetError();

    if(Error != AL_NO_ERROR) {
        bakge::Log("ERROR: Environment - Unexpected AL error while setting "
                                                    "listener position.\n");
        return BGE_FAILURE;
    }
#endif // _DEBUG

    return BGE_SUCCESS;
}


Result Environment::GetListenerPosition(Scalar* X, Scalar* Y, Scalar* Z)
{
    // Not sure if AL cares when passing NULL to alGetListener3f
    static Scalar Dummy;

    if(X == NULL)
        X = &Dummy;

    if(Y == NULL)
        Y = &Dummy;

    if(Z == NULL)
        Z = &Dummy;

#ifdef _DEBUG
    ALenum Error = alGetError();
#endif // _DEBUG

    alGetListener3f(AL_POSITION, X, Y, Z);

#ifdef _DEBUG
    Error = alGetError();

    if(Error != AL_NO_ERROR) {
        bakge::Log("ERROR: Environment - Unexpected AL error while getting "
                                                    "listener position.\n");
        return BGE_FAILURE;
    }
#endif // _DEBUG

    return BGE_SUCCESS;
}

} /* bakge */
