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

GamePad::GamePad()
{
    Handle = 0;
}


GamePad::~GamePad()
{
}


GamePad* GamePad::Detect()
{
    GamePad* Pad = (GamePad*)calloc(1, sizeof(GamePad));
    for(int i=GLFW_JOYSTICK_1;i<GLFW_JOYSTICK_LAST;++i) {
        if(glfwJoystickPresent(i) == GL_TRUE) {
            Pad->Handle = i;
            Log("GamePad: Detected joystick %d (%s)\n", i,
                                    glfwGetJoystickName(i));
        }
    }

    new(Pad) GamePad;

    Pad->GetButtonPressed(0);
    Pad->GetAxis(0);

    return Pad;
}

} /* bakge */
