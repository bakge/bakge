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

GLFWwindow* Window::SharedContext = NULL;

void Window::SetDefaultCallbacks(GLFWwindow* Handle)
{
    glfwSetWindowCloseCallback(Handle, Window::Closed);
    glfwSetWindowSizeCallback(Handle, Window::Resized);
    glfwSetWindowPosCallback(Handle, Window::Moved);
    glfwSetKeyCallback(Handle, Window::Key);
    glfwSetMouseButtonCallback(Handle, Window::Mouse);
    glfwSetCursorPosCallback(Handle, Window::MouseMotion);
    glfwSetScrollCallback(Handle, Window::Scroll);
}


void Window::Moved(GLFWwindow* Handle,  int X, int Y)
{
    Window* Win;
    EventHandler* Handler;

    Win = (Window*)glfwGetWindowUserPointer(Handle);
    Handler = Win->Handler;

    if(Handler != NULL) {
        /* Deploy event here */
    }
}


void Window::Resized(GLFWwindow* Handle, int Width, int Height)
{
    Window* Win;
    EventHandler* Handler;

    Win = (Window*)glfwGetWindowUserPointer(Handle);
    Handler = Win->Handler;

    if(Handler != NULL) {
        /* Deploy event here */
    }
}


void Window::Closed(GLFWwindow* Handle)
{
    Window* Win;
    EventHandler* Handler;

    Win = (Window*)glfwGetWindowUserPointer(Handle);
    Handler = Win->Handler;

    /* Deploy closed event before closing the window */
    if(Handler != NULL) {
        Handler->CloseEvent();
    }

    Win->Close();
}


void Window::Key(GLFWwindow* Handle, int Key, int Code, int Pressed, int Mod)
{
    Window* Win;
    EventHandler* Handler;

    Win = (Window*)glfwGetWindowUserPointer(Handle);
    Handler = Win->Handler;

    if(Handler != NULL) {
        Handler->KeyEvent(Key, Code, Pressed, Mod);
    }
}


void Window::Mouse(GLFWwindow* Handle, int Button, int Pressed, int Mod)
{
    Window* Win;
    EventHandler* Handler;

    Win = (Window*)glfwGetWindowUserPointer(Handle);
    Handler = Win->Handler;

    if(Handler != NULL) {
        /* Ok to pass int, ButtonID, ModField etc are just typedef int */
        Handler->MouseEvent(Button, Pressed, Mod);
    }
}


void Window::MouseMotion(GLFWwindow* Handle, double X, double Y)
{
    Window* Win;
    EventHandler* Handler;

    Win = (Window*)glfwGetWindowUserPointer(Handle);
    Handler = Win->Handler;

    if(Handler != NULL) {
        Handler->MotionEvent(X - Win->MouseCache.X, Y - Win->MouseCache.Y);
        Win->MouseCache.X = X;
        Win->MouseCache.Y = Y;
    }
}


void Window::Scroll(GLFWwindow* Handle, double X, double Y)
{
    Window* Win;
    EventHandler* Handler;

    Win = (Window*)glfwGetWindowUserPointer(Handle);
    Handler = Win->Handler;

    if(Handler != NULL) {
        Handler->ScrollEvent(X - Win->ScrollCache.X, Y - Win->ScrollCache.Y);
        Win->ScrollCache.X = X;
        Win->ScrollCache.Y = Y;
    }
}


Window::Window()
{
    WindowHandle = NULL;
    Handler = NULL;
    MouseCache.X = 0;
    MouseCache.Y = 0;
    ScrollCache.X = 0;
    ScrollCache.Y = 0;
}


Window::~Window()
{
    Close();
}


Window* Window::Create(int Width, int Height)
{
    GLFWwindow* Handle;
    Window* Win;

    Handle = glfwCreateWindow(Width, Height, "Bakge", NULL, SharedContext);
    if(Handle == NULL)
        return NULL;

    /* Set all of our window's GLFW callbacks */
    SetDefaultCallbacks(Handle);

    /* Allocate our bakge Window */
    Win = new Window;
    if(Win == NULL) {
        printf("Error allocating window memory\n");
        return NULL;
    }

    /* Set its window handle and make the context current */
    Win->WindowHandle = Handle;
    Win->Bind();

    /* Bind default shader to new window's context */
    ShaderProgram::GenericShader->Bind();

    /* *
     * Store pointer to Bakge window so global callbacks can access it.
     * They are static Window class methods, so they have full access
     * to the class objects.
     * */
    glfwSetWindowUserPointer(Handle, (void*)Win);

    return Win;
}


Window* Window::CreateFullScreen()
{
    /* *
     * This method does not require Width or Height parameters; GLFW
     * provides methods to automatically adjust to desktop resolutions.
     * */

    GLFWwindow* Handle;
    Window* Win;

    GLFWmonitor* PrimaryMonitor = glfwGetPrimaryMonitor();

    if(PrimaryMonitor == NULL){
        printf("Error getting primary monitor\n");
        return NULL;
    }

    const GLFWvidmode* MonitorVideoMode = glfwGetVideoMode(PrimaryMonitor);

    if(MonitorVideoMode == NULL){
        printf("Error getting monitor video mode\n");
        return NULL;
    }

    Handle = glfwCreateWindow(MonitorVideoMode->width,
                              MonitorVideoMode->height,
                              "bakge",
                               PrimaryMonitor,
                               SharedContext
                             );

    if(Handle == NULL){
        printf("Error creating window\n");
        return NULL;
    }

    SetDefaultCallbacks(Handle);

    /* Allocate our Bakge window */
    Win = new Window;
    if(Win == NULL){
        printf("Error allocating window memory\n");
        return NULL;
    }

    /* Set its window handle and make the context current */
    Win->WindowHandle = Handle;
    Win->Bind();

    /* *
     * Store pointer to Bakge window so global callbacks can access it.
     * They are static Window class methods, so they have full access
     * to the class objects.
     * */
    glfwSetWindowUserPointer(Handle, (void*)Win);

    return Win;
}


Result Window::SwapBuffers()
{
    glfwSwapBuffers(WindowHandle);

    return BGE_SUCCESS;
}


Result Window::Bind() const
{
    glfwMakeContextCurrent(WindowHandle);

    return BGE_SUCCESS;
}


Result Window::Unbind() const
{
    glfwMakeContextCurrent(NULL);

    return BGE_SUCCESS;
}


Result Window::Close()
{
    if(!IsOpen())
        return BGE_FAILURE;

    /* Destroy the GLFW window */
    GLFWwindow* Handle = WindowHandle;
    WindowHandle = NULL;
    glfwDestroyWindow(Handle);

    return BGE_SUCCESS;
}


bool Window::IsOpen()
{
    if(WindowHandle == NULL)
        return false;

    if(glfwWindowShouldClose(WindowHandle))
        return false;

    return true;
}


bool Window::IsActive()
{
    /* Active windows must be open */
    if(!IsOpen())
        return false;

    if(glfwGetWindowAttrib(WindowHandle, GLFW_ICONIFIED))
        return false;

    /* Windows are only active if they have input focus */
    if(glfwGetWindowAttrib(WindowHandle, GLFW_FOCUSED) == 0)
        return false;

    return true;
}


void Window::PollEvents()
{
    glfwPollEvents();
}


Result Window::GetMousePosition(DeviceCoord* X, DeviceCoord* Y)
{
    /* Can't access mouse if window isn't active */
    if(!IsActive())
        return BGE_FAILURE;

    glfwGetCursorPos(WindowHandle, X, Y);

    return BGE_SUCCESS;
}


Result Window::SetMousePosition(DeviceCoord X, DeviceCoord Y)
{
    /* Can't change mouse position if window isn't active */
    if(!IsActive())
        return BGE_FAILURE;

    glfwSetCursorPos(WindowHandle, X, Y);

    return BGE_SUCCESS;
}


void Window::Show()
{
    glfwShowWindow(WindowHandle);
}


void Window::Hide()
{
    glfwHideWindow(WindowHandle);
}


EventHandler* Window::SetEventHandler(EventHandler* Who)
{
    EventHandler* Previous;

    Previous = Handler;
    Handler = Who;

    return Previous;
}

} /* bakge */
