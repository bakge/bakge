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

#ifndef BAKGE_CORE_WINDOW_H
#define BAKGE_CORE_WINDOW_H

#include <bakge/Bakge.h>

namespace bakge
{

class BGE_API Window : public Bindable
{
    friend BGE_API Result Init(int argc, char* argv[]);
    friend BGE_API Result Deinit();

    static GLFWwindow* SharedContext;

    /* GLFW callbacks. Private for protection against bogus events */
    static void SetDefaultCallbacks(GLFWwindow* Handle);
    static void Moved(GLFWwindow*,  int, int);
    static void Resized(GLFWwindow*, int, int);
    static void Closed(GLFWwindow*);
    static void Key(GLFWwindow*, int, int, int, int);
    static void Mouse(GLFWwindow*, int, int, int);
    static void MouseMotion(GLFWwindow*, double, double);
    static void Scroll(GLFWwindow*, double, double);

    GLFWwindow* WindowHandle;

    /* Who receives events from the window? */
    EventHandler* Handler;

    /* Store last mouse position */
    struct {
        double X;
        double Y;
    } MouseCache;

    /* Store last scroll positions */
    struct {
        double X;
        double Y;
    } ScrollCache;

    Window();


public:

    ~Window();

    BGE_FACTORY Window* Create(int Width, int Height);

    /* Call manually to process events for all windows */
    static void PollEvents();

    /* *
     * Open windows are available for use, but not necessarily visible.
     * They may be iconified or not in focus.
     * */
    bool IsOpen();

    /* Active windows are open, visible and in focus. */
    bool IsActive();

    Result Close();

    Result SwapBuffers();

    /* Binding makes this window's context current on the calling thread */
    Result Bind() const;
    Result Unbind() const;

    /* Sets new event handler, returning the old one */
    EventHandler* SetEventHandler(EventHandler* Who);

    Result GetMousePosition(DeviceCoord* X, DeviceCoord* Y);
    Result SetMousePosition(DeviceCoord X, DeviceCoord Y);

    /* *
     * Hide and Show can fail if window is already hidden/shown,
     * but will do so silently
     * */
    void Hide();
    void Show();

}; /* Window */

} /* bakge */

#endif /* BAKGE_CORE_WINDOW_H */
