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

/*!
 * @file Window.h
 * @brief Window class declaration.
 */

#ifndef BAKGE_CORE_WINDOW_H
#define BAKGE_CORE_WINDOW_H

#include <bakge/Bakge.h>
#include <bakge/interface/IRectangle.h>

namespace bakge
{

/*! @brief Set the Window to fullscreen.
 *
 * Set the Window to fullscreen.
 */
#define BGE_WINDOW_FULLSCREEN 0x1

/*! @brief Create a borderless (undecorated) Window.
 *
 * Create a borderless (undecorated) Window.
 */
#define BGE_WINDOW_NOBORDER 0x2

/*! @brief Create a fixed-size Window.
 *
 * Create a fixed-size Window.
 */
#define BGE_WINDOW_NORESIZE 0x4

/*! @brief GLFW window wrapper class.
 *
 * Windows in Bakge have their own contexts, but have a shared context
 * across which they can share resources such as meshes, textures and
 * more.
 */
class BGE_API Window : public Bindable, public IRectangle<int>
{
    friend BGE_API Result Init(int argc, char* argv[]);
    friend BGE_API Result Deinit();

    static GLFWwindow* SharedContext;

    /* GLFW callbacks. Private for protection against bogus events */
    static void Moved(GLFWwindow*,  int, int);
    static void Resized(GLFWwindow*, int, int);
    static void Closed(GLFWwindow*);
    static void Key(GLFWwindow*, int, int, int, int);
    static void Mouse(GLFWwindow*, int, int, int);
    static void MouseMotion(GLFWwindow*, double, double);
    static void Scroll(GLFWwindow*, double, double);

    GLFWwindow* WindowHandle;

    char* WinTitle;

    /* Who receives events from the window? */
    EventHandler* Handler;

    /* Store last mouse position */
    struct {
        Coord X;
        Coord Y;
    } MouseCache;

    /* Store last scroll positions */
    struct {
        Coord X;
        Coord Y;
    } ScrollCache;


protected:

    /*! @brief Default Window constructor.
     *
     * Windows are not explicitly instantiable. The factory function Create
     * allocates and configures a new Window instance.
     *
     * @see Window::Create
     */
    Window();


public:

    /*! @brief Virtual Window destructor.
     *
     * Virtual Window destructor
     */
    virtual ~Window();

    /*! @brief Create and initialize a Window.
     *
     * This factory function creates and initializes a window. If any errors
     * occur, Create returns NULL.
     *
     * @param[in] Width The width of the created window.
     * @param[in] Height The height of the created window.
     * @param[in] Properties Bitfield containing window properties.
     *
     * @return Pointer to allocated Window, or NULL if any errors occur.
     *
     * @warning May only be called from the main thread.
     */
    BGE_FACTORY Window* Create(int Width, int Height, int Properties);

    /*! @brief Poll all events for all existing windows.
     *
     * GLFW polls all window events at the same time, calling a given
     * callback for each event.
     *
     * @warning May only be called from the main thread.
     */
    static void PollEvents();

    /*! @brief Check if a window is currently open.
     *
     * An instance of Window doesn't guarantee that the actual window exists.
     * If a window is flagged for closing or doesn't exist, it is considered
     * closed.
     *
     * @return true if the window exists and is not marked for deletion.
     * false otherwise.
     */
    bool IsOpen() const;

    /*! @brief Check if a window is currently visible.
     *
     * Check if a window is currently visible.
     *
     * @return true if window is visible; false if hidden.
     */
    bool IsVisible() const;

    /*! @brief Check if a window is open, visible and in focus.
     *
     * Active windows are open and visible and currently have input focus,
     * meaning key, mouse and other device input is processed only for that
     * window.
     *
     * @return true if window is open, visible and in focus. false otherwise.
     */
    bool IsActive() const;

    /*! @brief Check if a window is currently focused.
     *
     * Check if a window is currently focused.
     *
     * @return true if window has input focus; false otherwise.
     */
    bool IsFocused() const;

    /*! @brief Check if window is iconified.
     *
     * Check if window is iconified.
     *
     * @return true if window is currently iconified; false otherwise.
     */
    bool IsIconified() const;

    /*! @brief Mark a window for closing.
     *
     * @return BGE_SUCCESS if the window was successfully closed, or
     * BGE_FAILURE if the window was unable to be closed.
     */
    Result Close();

    /*! @brief Swap a window's framebuffers.
     *
     * After rendering a scene, call SwapBuffers to flip the buffer onto the
     * screen. If a window is closed or iconified this function will fail.
     *
     * @return BGE_SUCCESS if the window buffers were successfully swapped.
     * BGE_FAILURE if the window is closed, iconified or another error
     * occurred.
     */
    Result SwapBuffers();

    /*! @brief Bind the Window's context to the calling thread.
     *
     * Only one OpenGL context can be current on any given thread. This
     * method sets a window's context as current on the thread from which
     * it is called. Any previous bound window's context will no longer
     * be current on the thread.
     *
     * @return BGE_SUCCESS if the context was successfully made current, and
     * BGE_FAILURE if any error occurred.
     */
    Result Bind() const;

    /*! @brief Unbind the Window's context from the calling thread.
     *
     * This method removes the window's context as the current context on the
     * thread from which it is called. The current context is set to none,
     * and no code that requires an active OpenGL context can be safely run
     * until a new Window is bound.
     *
     * @return BGE_SUCCESS if the window context was removed as the current
     * context, or BGE_FAILURE if any error occurred.
     */
    Result Unbind() const;

    /*! @brief Set the event handler object for this window.
     *
     * This method sets the window's active event handler. If it is not NULL,
     * the appropriate EventHandler callback is run when the GLFW window
     * receives and processes an event.
     *
     * @param[in] Who Pointer to the new event handler object.
     *
     * @return Pointer to the previous event handler object, or NULL if none
     * existed previously.
     */
    EventHandler* SetEventHandler(EventHandler* Who);

    /*! @brief Get the mouse's position.
     *
     * @param[out] X Sets pointee to mouse's X position relative to the window.
     * @param[out] Y Sets pointee to mouse's Y position relative to the window.
     *
     * @return BGE_SUCCESS if the device coordinates were successfully set. If
     * the window is not currently active, returns BGE_FAILURE.
     *
     * @see Window::IsActive()
     */
    Result GetMousePosition(Coord* X, Coord* Y) const;

    /*! @brief Set the mouse's position.
     *
     * @param[in] X New X position of the mouse, relative to the window.
     * @param[in] Y New Y position of the mouse, relative to the window.
     *
     * @return BGE_SUCCESS if the mouse position was successfully set. If the
     * window is not currently active, returns BGE_FAILURE.
     *
     * @see Window::IsActive()
     */
    Result SetMousePosition(Coord X, Coord Y);

    /*! @brief Hide a window, making it invisible.
     *
     * Hide a window, making it invisible.
     *
     * @return BGE_SUCCESS if window was successfully hidden, or BGE_FAILURE
     * if the window is already hidden or any errors occurred.
     *
     * @warning May only be called from the main thread.
     */
    Result Hide();

    /*! @brief Show a window, making it visible.
     *
     * Show a window, making it visible.
     *
     * @return BGE_SUCCESS if window was successfully shown, or BGE_FAILURE
     * if the window is already shown or any errors occurred.
     *
     * @warning May only be called from the main thread.
     */
    Result Show();

    /*! @brief Iconify (minimize) the window.
     *
     * Iconify (minimize) the window.
     *
     * @return BGE_SUCCESS if window was iconified; BGE_FAILURE if already
     * iconified or any errors occurred.
     *
     * @warning May only be called from the main thread.
     */
    Result Iconify();

    /*! @brief Deiconify (restore) the window.
     *
     * Deiconify (restore) the window.
     *
     * @return BGE_SUCCESS if window was iconified; BGE_FAILURE if window
     * is not currently iconified or any errors occurred.
     *
     * @warning May only be called from the main thread.
     */
    Result Deiconify();

    /*! @brief Set the Window's screen position.
     *
     * Set the Window's screen position. The origin (0, 0) is the top-left of
     * the screen.
     *
     * @param[in] X X position of the screen.
     * @param[in] Y Y position of the screen.
     *
     * @return BGE_SUCCESS if the Window's position was successfully changed;
     * BGE_FAILURE if any errors occurred.
     */
    Result SetPosition(Coord X, Coord Y);

    /*! @brief Get the Window's screen position.
     *
     * Get the Window's screen position. The origin (0, 0) is the top-left of
     * the screen. The parameters are not changed if any errors occur.
     *
     * @param[in] X Pointer to Coord; set to Window's X position.
     * @param[in] Y Pointer to Coord; set to Window's Y position.
     *
     * @return BGE_FAILURE if any errors occurred; BGE_SUCCESS otherwise.
     */
    Result GetPosition(Coord* X, Coord* Y) const;

    /*! @brief Set the Window's client area size.
     *
     * Set the Window's client area size. This does not include borders or
     * other window decorations.
     *
     * @param[in] X New width of the Window.
     * @param[in] Y New height of the Window.
     *
     * @return BGE_SUCCESS if the Window size was successfully set; BGE_FAILURE
     * otherwise.
     */
    virtual void SetDimensions(int X, int Y);

    /*! @brief Get the Window's client area size.
     *
     * Get the Windnow's client area size. This does not include borders or
     * other window decorations.
     *
     * @param[out] X Pointee set to width of the Window.
     * @param[out] Y Pointee set to height of the Window.
     *
     * @return BGE_SUCCESS if the Window size was successfully retrieved;
     * BGE_FAILURE otherwise.
     */
    virtual void GetDimensions(int* X, int* Y) const;

    /*! @brief Set the title of the window.
     *
     * Set the title of the window.
     *
     * @return BGE_SUCCESS if window title was successfully changed;
     * BGE_FAILURE if any errors occurred.
     */
    Result SetTitle(const char* Title);

}; /* Window */

} /* bakge */

#endif /* BAKGE_CORE_WINDOW_H */
