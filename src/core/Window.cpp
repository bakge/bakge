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

void Window::Moved(GLFWwindow* Handle, int X, int Y)
{
    Window* Win;
    EventHandler* Handler;

    Win = (Window*)glfwGetWindowUserPointer(Handle);
    Handler = Win->Handler;

    if(Handler != NULL) {
        Handler->DragEvent(X, Y);
    }
}


void Window::Resized(GLFWwindow* Handle, int Width, int Height)
{
    Window* Win;
    EventHandler* Handler;

    Win = (Window*)glfwGetWindowUserPointer(Handle);
    Handler = Win->Handler;

    if(Handler != NULL) {
        Handler->ResizeEvent(Width, Height);
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
        Handler->MotionEvent((Motion)X - Win->MouseCache.X,
                            (Motion)Y - Win->MouseCache.Y);
        Win->MouseCache.X = (Coord)X;
        Win->MouseCache.Y = (Coord)Y;
    }
}


void Window::Scroll(GLFWwindow* Handle, double X, double Y)
{
    Window* Win;
    EventHandler* Handler;

    Win = (Window*)glfwGetWindowUserPointer(Handle);
    Handler = Win->Handler;

    if(Handler != NULL) {
        Handler->ScrollEvent((Motion)X - Win->ScrollCache.X,
                            (Motion)Y - Win->ScrollCache.Y);
        Win->ScrollCache.X = (Coord)X;
        Win->ScrollCache.Y = (Coord)Y;
    }
}


Window::Window()
{
    WindowHandle = NULL;
    Handler = NULL;
    WinTitle = NULL;
    MouseCache.X = 0;
    MouseCache.Y = 0;
    ScrollCache.X = 0;
    ScrollCache.Y = 0;
}


Window::~Window()
{
    if(IsOpen())
        Close();

    if(WinTitle != NULL)
        delete[] WinTitle;
}


Window* Window::Create(int Width, int Height, int Properties)
{
    // Check each window property and set corresponding GLFW window hints
    if(Properties & BGE_WINDOW_NOBORDER)
        glfwWindowHint(GLFW_DECORATED, GL_FALSE);

    if(Properties & BGE_WINDOW_NORESIZE)
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* Handle = glfwCreateWindow(Width, Height, "Bakge", NULL,
                                                        SharedContext);

    // Now reset GLFW window hints if necessary
    if(Properties & BGE_WINDOW_NOBORDER)
        glfwWindowHint(GLFW_DECORATED, GL_TRUE);

    if(Properties & BGE_WINDOW_NORESIZE)
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    if(Handle == NULL) {
        Log("ERROR: Unable to create GLFW window.\n");
        return NULL;
    }

    /* Set all of our window's GLFW callbacks */
    glfwSetWindowCloseCallback(Handle, Window::Closed);
    glfwSetWindowSizeCallback(Handle, Window::Resized);
    glfwSetWindowPosCallback(Handle, Window::Moved);
    glfwSetKeyCallback(Handle, Window::Key);
    glfwSetMouseButtonCallback(Handle, Window::Mouse);
    glfwSetCursorPosCallback(Handle, Window::MouseMotion);
    glfwSetScrollCallback(Handle, Window::Scroll);

    /* Allocate our bakge Window */
    Window* Win = new Window;
    if(Win == NULL) {
        Log("ERROR: Window - Couldn't allocate memory\n");
        return NULL;
    }

    /* Set its window handle and make the context current */
    Win->WindowHandle = Handle;
    Win->Bind();

    /* Bind default shader to new window's context */
    if(Shader::GenericShader->Bind() == BGE_FAILURE) {
        Log("ERROR: Window - Unable to bind default shader\n");
        delete Win;
        return NULL;
    }

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
    if(!IsOpen()) {
        Log("WARNING: Called SwapBuffers on closed Window\n");
        return BGE_FAILURE;
    }

    glfwSwapBuffers(WindowHandle);

    return BGE_SUCCESS;
}


Result Window::Bind() const
{
    if(!IsOpen()) {
        Log("WARNING: Called Bind on closed Window\n");
        return BGE_FAILURE;
    }

    glfwMakeContextCurrent(WindowHandle);

    return BGE_SUCCESS;
}


Result Window::Unbind() const
{
    if(!IsOpen()) {
        Log("WARNING: Called Unbind on closed Window\n");
        return BGE_FAILURE;
    }

    glfwMakeContextCurrent(NULL);

    return BGE_SUCCESS;
}


Result Window::Close()
{
    if(!IsOpen()) {
        Log("WARNING: Called Close on closed Window\n");
        return BGE_FAILURE;
    }

    /* Destroy the GLFW window */
    GLFWwindow* Handle = WindowHandle;
    WindowHandle = NULL;
    glfwDestroyWindow(Handle);

    if(Handler != NULL)
        Handler->CloseEvent();

    return BGE_SUCCESS;
}


bool Window::IsOpen() const
{
    if(WindowHandle == NULL)
        return false;

    if(glfwWindowShouldClose(WindowHandle))
        return false;

    return true;
}


bool Window::IsActive() const
{
    /* Active windows must be open */
    if(!IsOpen())
        return false;

    if(IsIconified())
        return false;

    /* Windows are only active if they have input focus */
    if(!IsFocused())
        return false;

    return true;
}


bool Window::IsVisible() const
{
    if(!IsOpen()) {
        Log("WARNING: Called IsVisible on closed Window\n");
        return false;
    }

    return glfwGetWindowAttrib(WindowHandle, GLFW_VISIBLE) != 0;
}


bool Window::IsIconified() const
{
    if(!IsOpen()) {
        Log("WARNING: Called IsIconified on closed Window\n");
        return false;
    }

    return glfwGetWindowAttrib(WindowHandle, GLFW_ICONIFIED) != 0;
}


bool Window::IsFocused() const
{
    if(!IsOpen()) {
        Log("WARNING: Called IsFocused on closed Window\n");
        return false;
    }

    return glfwGetWindowAttrib(WindowHandle, GLFW_FOCUSED) != 0;
}


void Window::PollEvents()
{
    glfwPollEvents();
}


Result Window::GetMousePosition(Coord* X, Coord* Y) const
{
    if(!IsOpen()) {
        Log("WARNING: Called GetMousePosition on closed Window\n");
        return BGE_FAILURE;
    }

    /* Can't access mouse if window isn't active */
    if(!IsActive())
        return BGE_FAILURE;

    double PX, PY;
    glfwGetCursorPos(WindowHandle, &PX, &PY);

    *X = (Coord)PX;
    *Y = (Coord)PY;

    return BGE_SUCCESS;
}


Result Window::SetMousePosition(Coord X, Coord Y)
{
    if(!IsOpen()) {
        Log("WARNING: Called SetMousePosition on closed Window\n");
        return BGE_FAILURE;
    }

    /* Can't change mouse position if window isn't active */
    if(!IsActive())
        return BGE_FAILURE;

    glfwSetCursorPos(WindowHandle, X, Y);

    return BGE_SUCCESS;
}


Result Window::Show()
{
    if(!IsOpen()) {
        Log("WARNING: Called Show on closed Window\n");
        return BGE_FAILURE;
    }

    if(IsVisible())
        return BGE_FAILURE;

    glfwShowWindow(WindowHandle);

    return BGE_SUCCESS;
}


Result Window::Hide()
{
    if(!IsOpen()) {
        Log("WARNING: Called Hide on closed Window\n");
        return BGE_FAILURE;
    }

    if(!IsVisible())
        return BGE_FAILURE;

    glfwHideWindow(WindowHandle);

    return BGE_SUCCESS;
}


EventHandler* Window::SetEventHandler(EventHandler* Who)
{
    if(!IsOpen()) {
        Log("WARNING: Called SetEventHandler on closed Window\n");
        return NULL;
    }

    EventHandler* Previous;

    Previous = Handler;
    Handler = Who;

    return Previous;
}


Result Window::Iconify()
{
    if(!IsOpen()) {
        Log("WARNING: Called Iconify on closed Window\n");
        return BGE_FAILURE;
    }

    if(IsIconified())
        return BGE_FAILURE;

    glfwIconifyWindow(WindowHandle);

    return BGE_SUCCESS;
}


Result Window::Deiconify()
{
    if(!IsOpen()) {
        Log("WARNING: Called Deiconify on closed Window\n");
        return BGE_FAILURE;
    }

    if(!IsIconified())
        return BGE_FAILURE;

    glfwRestoreWindow(WindowHandle);

    return BGE_SUCCESS;
}


Result Window::SetPosition(Coord X, Coord Y)
{
    if(!IsOpen()) {
        Log("WARNING: Called SetPosition on closed Window\n");
        return BGE_FAILURE;
    }

    glfwSetWindowPos(WindowHandle, (int)X, (int)Y);

    return BGE_SUCCESS;
}


Result Window::GetPosition(Coord* X, Coord* Y) const
{
    if(!IsOpen()) {
        Log("WARNING: Called GetPosition on closed Window\n");
        return BGE_FAILURE;
    }

    int PX, PY;

    glfwGetWindowPos(WindowHandle, &PX, &PY);

    *X = (float)PX;
    *Y = (float)PY;

    return BGE_SUCCESS;
}


Result Window::SetTitle(const char* Title)
{
    if(!IsOpen()) {
        Log("WARNING: Called SetTitle on closed Window\n");
        return BGE_FAILURE;
    }

    glfwSetWindowTitle(WindowHandle, Title);

    if(WinTitle != NULL) {
        delete[] WinTitle;
        WinTitle = new char[strlen(Title)];
        strcpy(WinTitle, Title);
    }

    return BGE_SUCCESS;
}

} /* bakge */
