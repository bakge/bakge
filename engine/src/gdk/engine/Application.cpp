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

namespace bakge
{
namespace gdk
{

Application::Application()
{
    Gui = NULL;
    Win = NULL;
}


Application::~Application()
{
    if(Gui != NULL)
        delete Gui;

    if(Win != NULL)
        delete Win;
}


Application* Application::Create()
{
    Application* App = new Application;

    App->Gui = GUI::Create();
    if(App->Gui == NULL) {
        Log("Application: Error creating GUI\n");
        delete App;
        return NULL;
    } else {
        Log("Application: Successfully created GUI\n");
    }

    App->Win = Window::Create(800, 480, 0);
    if(App->Win == NULL) {
        Log("Application: Error creating Window\n");
        delete App;
        return NULL;
    } else {
        Log("Application: Successfully created Window\n");
    }

    App->Win->SetEventHandler(App);

    return App;
}


Result Application::Initialize()
{
    return BGE_SUCCESS;
}


Result Application::ShutDown()
{
    return BGE_SUCCESS;
}


int Application::Run()
{
    while(Win->IsOpen()) {
        Window::PollEvents();
    }

    return 0;
}


Result Application::Update(Seconds DeltaTime)
{
    return BGE_SUCCESS;
}


Result Application::PreRenderStage()
{
    return BGE_SUCCESS;
}


Result Application::PostRenderStage()
{
    return BGE_SUCCESS;
}


Result Application::RenderStage()
{
    return BGE_SUCCESS;
}


Result Application::KeyEvent(KeyID K, KeyState S, ScanCode C, ModField M)
{
    return BGE_SUCCESS;
}


Result Application::MouseEvent(ButtonID B, ButtonState S, ModField M)
{
    return BGE_SUCCESS;
}


Result Application::MotionEvent(Motion X, Motion Y)
{
    return BGE_SUCCESS;
}


Result Application::ScrollEvent(Motion X, Motion Y)
{
    return BGE_SUCCESS;
}


Result Application::CloseEvent()
{
    return BGE_SUCCESS;
}

} /* gdk */
} /* bakge */
