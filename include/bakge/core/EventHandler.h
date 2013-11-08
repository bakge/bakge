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
 * @file EventHandler.h
 * @brief EventHandler interface declaration.
 */

#ifndef BAKGE_CORE_EVENTHANDLER_H
#define BAKGE_CORE_EVENTHANDLER_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief Interface for objects able to process Window events
 *
 * When a window event is processed, the appropriate callback is run
 * if an event handler exists for that window. The event is received
 * and processed internally; users need only implement methods for
 * responding to events.
 *
 * Certain events are only processed for the active window, such as key and
 * mouse input.
 */
class BGE_API EventHandler
{

public:

    /*! @brief Default EventHandler constructor.
     *
     * Default EventHandler constructor.
     */
    EventHandler();

    /*! @brief Pure-virtual EventHandler destructor.
     *
     * EventHandler is an abstract interface class, only inheriting classes
     * will be instantiable.
     */
    virtual ~EventHandler() = 0;

    /*! @brief Called when a keyboard key is pressed or released.
     *
     * Called when a keyboard key is pressed or released.
     *
     * @param[in] K GLFW key identifier.
     * @param[in] S GLFW key state identifier.
     * @param[in] C OS-specific key scan-code.
     * @param[in] M Active modifiers bitfield.
     *
     * @return BGE_FAILURE if any errors occurred while processing the key
     * event, otherwise BGE_SUCCESS.
     *
     * @see http://www.glfw.org/docs/latest/glfw3_8h.html
     * @see ShiftPressed, AltPressed, ControlPressed, SuperPressed
     */
    virtual Result KeyEvent(KeyID K, KeyState S, ScanCode C, ModField M) = 0;

    /*! @brief Called when a mouse button is pressed or released.
     *
     * Called when a mouse button is pressed or released.
     *
     * @param[in] B GLFW mouse button identifier.
     * @param[in] S GLFW button state identifier.
     * @param[in] M Active modifiers bitfield.
     *
     * @return BGE_FAILURE if any errors occurred while processing the mouse
     * button event, otherwise BGE_SUCCESS.
     */
    virtual Result MouseEvent(ButtonID B, ButtonState S, ModField M) = 0;

    /*! @brief Called when mouse motion is detected.
     *
     * Called when mouse motion is detected.
     *
     * @param[in] X Horizontal mouse motion.
     * @param[in] Y Vertical mouse motion.
     *
     * @return BGE_FAILURE if any errors occurred while processing the mouse
     * motion event, otherwise BGE_SUCCESS.
     */
    virtual Result MotionEvent(Motion X, Motion Y) = 0;

    /*! @brief Called when the mouse wheel is scrolled.
     *
     * Called when the mouse wheel is scrolled.
     *
     * @param[in] X Horizontal scroll wheel motion.
     * @param[in] Y Vertical scroll wheel motion.
     *
     * @return BGE_FAILURE if any errors occurred while processing the mouse
     * scroll wheel event, otherwise BGE_SUCCESS.
     *
     * @note It doesn't appear horizontal scroll wheel motion is relevant.
     * Some mouses do have scroll wheels which can "click" left and right.
     * However, it is unclear whether GLFW detects these mouse actions and
     * processes them as scroll wheel events.
     */
    virtual Result ScrollEvent(Motion X, Motion Y) = 0;

    /*! @brief Called just before the Window closes.
     *
     * Typically this event means the entire application has been requested
     * to exit. This function is a great place to do memory clean up and
     * deinitialization of other components.
     *
     * @return BGE_FAILURE if any errors occurred while processing the window
     * close event, otherwise BGE_SUCCESS.
     */
    virtual Result CloseEvent() = 0;

    /*! @brief Called when the Window is resized.
     *
     * Called when the Window is resized.
     *
     * @param[in] X Width of the Window after being resized.
     * @param[in] Y Height of the Window after being resized.
     *
     * @return BGE_FAILURE if any errors occurred while processing the event;
     * BGE_SUCCESS otherwise.
     */
    virtual Result ResizeEvent(int X, int Y) = 0;

    /*! @brief Called when the Window is dragged.
     *
     * Called when Window is dragged.
     *
     * @param[in] X X-position of the Window after being dragged.
     * @param[in] Y Y-position of the Window after being dragged.
     *
     * @return BGE_FAILURE if any errors occurred while processing the event;
     * BGE_SUCCESS otherwise.
     */
    virtual Result DragEvent(int X, int Y) = 0;

}; /* EventHandler */

} /* bakge */

#endif /* BAKGE_CORE_EVENTHANDLER_H */
