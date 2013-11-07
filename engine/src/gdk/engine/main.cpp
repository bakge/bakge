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

#include <bakge/gdk/engine/Common.h>

int main(int argc, char* argv[])
{
    if(bakge::Init(argc, argv) != BGE_SUCCESS)
        return -1;

    // Log some info about the engine
    bakge::Log("Bakge GDK Engine v%d.%d.%d\n", BGE_GDK_ENGINE_VER_MAJ,
                                                BGE_GDK_ENGINE_VER_MIN,
                                                BGE_GDK_ENGINE_VER_REV);

    bakge::gdk::Application* App = bakge::gdk::Application::Create();

    int Code;
    if(App->Initialize() == BGE_FAILURE) {
        Code = -1;
        goto ExitEngine;
    }

    Code = App->Run();

    App->ShutDown();

ExitEngine:

    bakge::Log("Engine exited with code %x\n", Code);

    bakge::Deinit();

    return Code;
}
