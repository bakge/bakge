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

#ifndef BAKGE_ENGINE_TESTENGINE_H
#define BAKGE_ENGINE_TESTENGINE_H

#include <bakge/Bakge.h>

namespace bakge
{

/* *
 * A Generic Callback does not take a parameter.
 * It is used for Initialize, Shutdown, Pre/On/Post
 * RenderStage, etc.
 * */
typedef Result (*GenericCallback)();
typedef Result (*UpdateCallback)(Seconds DeltaTime);
typedef Result (*KeyEventCallback)(KeyID K, KeyState S, ScanCode C, ModField M);
typedef Result (*MouseEventCallback)(ButtonID B, ButtonState S, ModField M);
typedef Result (*MotionEventCallback)(Motion X, Motion Y);
typedef Result (*ScrollEventCallback)(Motion X, Motion Y);

class TestEngine : public Engine, public EventHandler
{
    /* *
     * Internal engine bakge::Logic is private. TestEngine will provide
     * callbacks to the user so that different parts of his/her
     * code can be ran independent of the functionality of the
     * entire engine.
     *
     * All callbacks that are provided to TestEngine will be invoked
     * after TestEngine's internal bakge::Logic has executed. If a callback
     * a certain callback is not provided, TestEngine will continue
     * running as if the callback did not exist. The idea is that
     * now the user is able to focus on the parts of the code that
     * require testing, without needing to write unrelated code.
     *
     * Input callbacks can be subscribed to by the user by setting
     * a callback.
     * */

    /* Initialize/Clean up components */
    Result Initialize()
    {
        EngineWindow = Window::Create(600, 400, 0);
        if(EngineWindow == NULL)
            return BGE_FAILURE;

        glClearColor(0, 0.3f, 0.5f, 1);

        EngineWindow->SetEventHandler(this);
        EngineWindow->Bind();

        if(InitCB != NULL)
            InitCB();

        return BGE_SUCCESS;
    }

    Result ShutDown()
    {
        EngineWindow->Unbind();
        delete EngineWindow;

        if(ShutDownCB != NULL)
            return ShutDownCB();

        return BGE_FAILURE;
    }

    /* Run will have no callback */
    int Run()
    {
        int ExitCode = 0;

        while(1)
        {
            Window::PollEvents();

            if(EngineWindow->IsOpen() == false) {
                if(CloseEventCB != NULL)
                    CloseEventCB();

                break;
            }

            Update(0);
            if(UpdateCB != NULL)
                UpdateCB(0);

            PreRenderStage();
            if(PreRenderCB != NULL)
                PreRenderCB();

            RenderStage();
            if(RenderCB != NULL)
                RenderCB();

            PostRenderStage();
            if(PostRenderCB != NULL)
                PostRenderCB();
        }

        StopEngine();

        return ExitCode;
    }

    Result Update(Seconds DeltaTime)
    {
        /* Reserved for future bakge::Logic */
        return BGE_SUCCESS;
    }

    Result PreRenderStage()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        return BGE_SUCCESS;
    }

    Result RenderStage()
    {
        /* Reserved for future bakge::Logic */
        return BGE_SUCCESS;
    }

    Result PostRenderStage()
    {
        EngineWindow->SwapBuffers();

        return BGE_SUCCESS;
    }

    /* Input callbacks */
    /* Key press/release and mouse button click/release events */
    Result KeyEvent(KeyID K, KeyState S, ScanCode C, ModField M)
    {
        if(KeyEventCB != NULL)
            return KeyEventCB(K, S, C, M);

        return BGE_FAILURE;
    }

    Result MouseEvent(ButtonID B, ButtonState S, ModField M)
    {
        if(MouseEventCB != NULL)
            return MouseEventCB(B, S, M);

        return BGE_FAILURE;
    }

    /* Relative device motion events */
    Result MotionEvent(Motion X, Motion Y)
    {
        if(MotionEventCB != NULL)
            MotionEventCB(X, Y);

        return BGE_FAILURE;
    }

    Result ScrollEvent(Motion X, Motion Y)
    {
        if(ScrollEventCB != NULL)
            ScrollEventCB(X, Y);

        return BGE_FAILURE;
    }

    /* Called just before the window is closed */
    Result CloseEvent()
    {
        if(CloseEventCB != NULL)
            return CloseEventCB();

        return BGE_FAILURE;
    }

    Result ResizeEvent(int X, int Y)
    {
        return BGE_FAILURE;
    }

    Result DragEvent(int X, int Y)
    {
        return BGE_FAILURE;
    }

    Window* EngineWindow;

    GenericCallback InitCB;
    GenericCallback ShutDownCB;

    UpdateCallback UpdateCB;

    GenericCallback PreRenderCB;
    GenericCallback RenderCB;
    GenericCallback PostRenderCB;

    KeyEventCallback KeyEventCB;
    MouseEventCallback MouseEventCB;
    MotionEventCallback MotionEventCB;
    ScrollEventCallback ScrollEventCB;
    GenericCallback CloseEventCB;


public:

    TestEngine()
    {
        InitCB = NULL;
        ShutDownCB = NULL;
        UpdateCB = NULL;
        PreRenderCB = NULL;
        RenderCB = NULL;
        PostRenderCB = NULL;
        KeyEventCB = NULL;
        MouseEventCB = NULL;
        MotionEventCB = NULL;
        ScrollEventCB = NULL;
        CloseEventCB = NULL;
    }

    ~TestEngine()
    {
    }

    void StartEngine()
    {
        Initialize();
        Run();
    }

    void StopEngine()
    {
        EngineWindow->Close();
        ShutDown();
    }

    void SetInitializeCallback(GenericCallback Callback)
    {
        InitCB = Callback;
    }

    void SetShutDownCallback(GenericCallback Callback)
    {
        ShutDownCB = Callback;
    }

    void SetUpdateCallback(UpdateCallback Callback)
    {
        UpdateCB = Callback;
    }

    void SetPreRenderCallback(GenericCallback Callback)
    {
        PreRenderCB = Callback;
    }

    void SetRenderCallback(GenericCallback Callback)
    {
        RenderCB = Callback;
    }

    void SetPostRenderCallback(GenericCallback Callback)
    {
        PostRenderCB = Callback;
    }

    void SetKeyEventCallback(KeyEventCallback Callback)
    {
        KeyEventCB = Callback;
    }

    void SetMouseEventCallback(MouseEventCallback Callback)
    {
        MouseEventCB = Callback;
    }

    void SetMotionEventCallback(MotionEventCallback Callback)
    {
        MotionEventCB = Callback;
    }

    void SetScrollEventCallback(ScrollEventCallback Callback)
    {
        ScrollEventCB = Callback;
    }

    void SetCloseEventCallback(GenericCallback Callback)
    {
        CloseEventCB = Callback;
    }

}; /* TestEngine */

} /* bakge */

#endif /* BAKGE_ENGINE_TESTENGINE_H */
